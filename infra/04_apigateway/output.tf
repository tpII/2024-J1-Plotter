output "api_url" {
  description = "The URL of the API Gateway"
  value       = aws_apigatewayv2_stage.default_stage.invoke_url
}

output "api_logs_group_name" {
  description = "The name of the CloudWatch Log Group for API Gateway logs"
  value       = aws_cloudwatch_log_group.api_logs.name
}

output "api_id" {
  description = "The ID of the API Gateway"
  value       = aws_apigatewayv2_api.api.id
}

output "user_pool_client_id" {
  value = aws_cognito_user_pool_client.user_pool_client.id
}

output "user_pool_id" {
  value = aws_cognito_user_pool.user_pool.id
}
