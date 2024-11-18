resource "aws_apigatewayv2_api" "this" {
  name          = var.api_name
  protocol_type = "HTTP"
}

resource "aws_apigatewayv2_integration" "lambda" {
  api_id           = aws_apigatewayv2_api.this.id
  integration_type = "AWS_PROXY"
  integration_uri  = "arn:aws:apigateway:${var.aws_region}:lambda:path/2015-03-31/functions/${var.lambda_function_arn}/invocations"
  integration_method = "POST"
}


resource "aws_apigatewayv2_route" "default_route" {
  api_id    = aws_apigatewayv2_api.this.id
  route_key = "ANY /{proxy+}"

  target = "integrations/${aws_apigatewayv2_integration.lambda.id}"

  depends_on = [
    aws_apigatewayv2_integration.lambda
  ]
}

resource "aws_apigatewayv2_stage" "default" {
  api_id      = aws_apigatewayv2_api.this.id
  name        = "$default"
  auto_deploy = true
}
