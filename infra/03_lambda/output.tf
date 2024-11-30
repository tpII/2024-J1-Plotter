output "lambda_function_name" {
  description = "El nombre de la función Lambda"
  value       = aws_lambda_function.drawing_lambda.function_name
}

output "lambda_function_arn" {
  description = "El ARN de la función Lambda"
  value       = aws_lambda_function.drawing_lambda.arn
}

output "lambda_execution_role_arn" {
  description = "El ARN del rol de ejecución de Lambda"
  value       = aws_iam_role.lambda_execution_role.arn
}
