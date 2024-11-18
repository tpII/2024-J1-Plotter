output "api_id" {
  description = "The ID of the API Gateway"
  value       = aws_apigatewayv2_api.this.id
}

output "integration_id" {
  description = "The ID of the API Gateway integration"
  value       = aws_apigatewayv2_integration.lambda.id
}

output "invoke_url" {
  description = "The URL to invoke the API"
  value       = aws_apigatewayv2_stage.default.invoke_url
}
