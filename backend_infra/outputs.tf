output "lambda_function_name" {
  description = "The name of the Lambda function"
  value       = module.lambda.function_name
}

output "s3_bucket_name" {
  description = "The name of the existing S3 bucket"
  value       = data.aws_s3_bucket.existing.bucket
}

output "lambda_function_arn" {
  description = "The ARN of the Lambda function"
  value       = module.lambda.function_arn
}

output "api_gateway_url" {
  description = "The URL of the API Gateway"
  value       = module.api_gateway.invoke_url
}