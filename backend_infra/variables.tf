variable "aws_region" {
  description = "The AWS region to deploy resources"
  type        = string
  default     = "us-east-1"
}

variable "s3_bucket_name" {
  description = "Nombre del bucket S3 existente para el código Lambda"
  type        = string
  default     = "2024-unlp-plotter-backend"
}


variable "s3_object_key" {
  description = "The S3 object key (path) for the Lambda code"
  type        = string
  default     = "lambda.zip"
}

variable "lambda_function_name" {
  description = "The name of the Lambda function"
  type        = string
  default     = "drawing-robot-lambda"
}

variable "lambda_environment_variables" {
  description = "Environment variables for the Lambda function"
  type        = map(string)
  default     = {
    ENV_KEY1 = "value1"
    ENV_KEY2 = "value2"
  }
}

variable "environment" {
  description = "Environment for resource tagging (e.g., Development, Staging, Production)"
  type        = string
  default     = "Development"
}
