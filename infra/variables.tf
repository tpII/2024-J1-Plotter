variable "s3_bucket_name" {
  description = "Unique name for the S3 bucket"
  type        = string
  default     = "canvas-app-bucket-1234"
}

variable "dynamodb_table_name" {
  description = "DynamoDB table name for storing drawing data"
  type        = string
  default     = "DrawingData"
}

variable "s3_bucket" {
  description = "S3 bucket where Lambda code is stored"
  type        = string
  default     = "lambda-code-bucket"
}

variable "s3_key" {
  description = "S3 key for the Lambda code zip file"
  type        = string
  default     = "process_drawing.zip"
}
