variable "function_name" {
  description = "Name of the Lambda function"
  type        = string
}

variable "s3_bucket_name" {
  description = "The name of the S3 bucket containing the Lambda code"
  type        = string
}

variable "s3_object_key" {
  description = "The S3 object key (path) for the Lambda code"
  type        = string
}

variable "environment_variables" {
  description = "Environment variables for the Lambda function"
  type        = map(string)
  default     = {}
}
