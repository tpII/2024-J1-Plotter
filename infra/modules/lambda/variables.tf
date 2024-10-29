variable "dynamodb_table" {
  description = "Name of the DynamoDB table for Lambda function environment variable"
  type        = string
}

variable "role_arn" {
  description = "ARN of the IAM role for Lambda execution"
  type        = string
}

variable "s3_bucket" {
  description = "S3 bucket storing the Lambda deployment package"
  type        = string
}

variable "s3_key" {
  description = "S3 object key for the Lambda deployment package"
  type        = string
}
