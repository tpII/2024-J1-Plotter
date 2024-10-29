variable "dynamodb_table" {
  description = "DynamoDB table name for storing IoT data"
  type        = string
}

variable "role_arn" {
  description = "ARN of the IAM role for IoT access to DynamoDB"
  type        = string
}
