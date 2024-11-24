resource "aws_cognito_user_pool" "user_pool" {
  name = "${var.api_name}-user-pool"
}

resource "aws_cognito_user_pool_client" "user_pool_client" {
  name         = "${var.api_name}-client"
  user_pool_id = aws_cognito_user_pool.user_pool.id

  allowed_oauth_flows_user_pool_client = true
  allowed_oauth_flows                 = ["implicit"]
  allowed_oauth_scopes                = ["email", "openid"]
  generate_secret                     = false
  callback_urls                       = ["http://localhost:3000"] # Cambia según tu frontend
}

resource "aws_apigatewayv2_api" "api" {
  name          = var.api_name
  protocol_type = "HTTP"

  cors_configuration {
    allow_origins = var.cors_allow_origins
    allow_methods = var.cors_allow_methods
    allow_headers = var.cors_allow_headers
    expose_headers = var.cors_expose_headers
    max_age = var.cors_max_age
  }
}

resource "aws_apigatewayv2_authorizer" "cognito_authorizer" {
  name            = "${var.api_name}-auth"
  api_id          = aws_apigatewayv2_api.api.id
  authorizer_type = "JWT"

  identity_sources = ["$request.header.Authorization"]

  jwt_configuration {
    audience = [aws_cognito_user_pool_client.user_pool_client.id]
    issuer   = "https://cognito-idp.us-east-1.amazonaws.com/us-east-1_cURWxGRcK"
  }
}


resource "aws_apigatewayv2_integration" "lambda_integration" {
  api_id           = aws_apigatewayv2_api.api.id
  integration_type = "AWS_PROXY"
  integration_uri  = var.lambda_function_arn

  depends_on = [aws_lambda_permission.api_gateway_permission]
}

resource "aws_apigatewayv2_route" "default_route" {
  api_id    = aws_apigatewayv2_api.api.id
  route_key = "$default"
  target    = "integrations/${aws_apigatewayv2_integration.lambda_integration.id}"
  authorization_type = "JWT"
  authorizer_id      = aws_apigatewayv2_authorizer.cognito_authorizer.id
}


resource "aws_apigatewayv2_stage" "default_stage" {
  api_id      = aws_apigatewayv2_api.api.id
  name        = "$default"
  auto_deploy = true

  access_log_settings {
    destination_arn = aws_cloudwatch_log_group.api_logs.arn
    format          = jsonencode({
      requestId    = "$context.requestId"
      ip           = "$context.identity.sourceIp"
      requestTime  = "$context.requestTime"
      httpMethod   = "$context.httpMethod"
      resourcePath = "$context.resourcePath"
      status       = "$context.status"
    })
  }
}

resource "aws_cloudwatch_log_group" "api_logs" {
  name              = "/aws/api-gateway/${aws_apigatewayv2_api.api.name}"
  retention_in_days = var.log_retention_in_days
}

resource "aws_lambda_permission" "api_gateway_permission" {
  statement_id  = "AllowAPIGatewayInvoke"
  action        = "lambda:InvokeFunction"
  function_name = var.lambda_function_name
  principal     = "apigateway.amazonaws.com"
  source_arn    = "${aws_apigatewayv2_api.api.execution_arn}/*"
}
