output "s3_bucket_name" {
  description = "The name of the S3 bucket for Lambda code storage"
  value       = module.s3_lambda_code.bucket_name
}

output "lambda_function_name" {
  description = "The name of the Lambda function"
  value       = module.lambda.function_name
}

output "lambda_function_arn" {
  description = "The ARN of the Lambda function"
  value       = module.lambda.function_arn
}

output "api_gateway_url" {
  description = "The invoke URL of the API Gateway"
  value       = module.api_gateway.invoke_url
}
