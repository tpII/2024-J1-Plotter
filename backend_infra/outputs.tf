output "api_url" {
  value = aws_apigatewayv2_stage.default_stage.invoke_url
}

output "lambda_function_name" {
  value = aws_lambda_function.hello_lambda.function_name
}
