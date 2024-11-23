variable "api_name" {
  description = "The name of the API Gateway"
  type        = string
  default     = "drawing-api"
}

variable "lambda_function_arn" {
  description = "The ARN of the Lambda function to integrate with API Gateway"
  type        = string
}

variable "lambda_function_name" {
  description = "The name of the Lambda function to integrate with API Gateway"
  type        = string
}

# CORS Configuration
variable "cors_allow_origins" {
  description = "List of origins allowed for CORS"
  type        = list(string)
  default     = ["*"]
}

variable "cors_allow_methods" {
  description = "List of HTTP methods allowed for CORS"
  type        = list(string)
  default     = ["GET", "POST", "OPTIONS"]
}

variable "cors_allow_headers" {
  description = "List of HTTP headers allowed for CORS"
  type        = list(string)
  default     = ["Content-Type", "Authorization"]
}

variable "cors_expose_headers" {
  description = "List of HTTP headers exposed to the client"
  type        = list(string)
  default     = ["x-amzn-RequestId"]
}

variable "cors_max_age" {
  description = "Maximum age for CORS preflight requests (in seconds)"
  type        = number
  default     = 3600
}

# Logging Configuration
variable "log_retention_in_days" {
  description = "Number of days to retain logs in CloudWatch"
  type        = number
  default     = 7
}
