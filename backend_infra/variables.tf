variable "aws_region" {
  default = "us-east-1"
}

variable "aws_profile" {
  default = "default"
}

variable "lambda_image_uri" {
  description = "The ECR URI of the Docker image for the Lambda function"
  type        = string
}

variable "function_name" {
  description = "The name of the Lambda function"
  default     = "hello-lambda"
}

variable "api_gateway_name" {
  description = "The name of the API Gateway"
  default     = "hello-api"
}
