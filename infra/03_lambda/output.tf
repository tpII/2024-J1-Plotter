output "lambda_function_name" {
  description = "The name of the Lambda function"
  value       = aws_lambda_function.drawing_lambda.function_name
}

output "lambda_function_arn" {
  description = "The ARN of the Lambda function"
  value       = aws_lambda_function.drawing_lambda.arn
}

output "lambda_execution_role_arn" {
  description = "The ARN of the IAM execution role for the Lambda function"
  value       = aws_iam_role.lambda_execution_role.arn
}
