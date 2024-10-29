resource "aws_apigatewayv2_api" "canvas_api" {
  name          = "CanvasAPI"
  protocol_type = "HTTP"
}

resource "aws_apigatewayv2_stage" "canvas_api_stage" {
  api_id      = aws_apigatewayv2_api.canvas_api.id
  name        = "$default"
  auto_deploy = true
}

resource "aws_lambda_permission" "api_gateway_invoke" {
  statement_id  = "AllowAPIGatewayInvoke"
  action        = "lambda:InvokeFunction"
  function_name = var.lambda_arn
  principal     = "apigateway.amazonaws.com"
  source_arn    = "${aws_apigatewayv2_api.canvas_api.execution_arn}/*"
}
