variable "api_name" {
  description = "The name of the API Gateway"
  type        = string
  default     = "drawing-robot-api"
}

variable "aws_region" {
  description = "The AWS region for the API Gateway"
  type        = string
}

variable "lambda_function_arn" {
  description = "The ARN of the Lambda function to integrate with the API Gateway"
  type        = string
}
