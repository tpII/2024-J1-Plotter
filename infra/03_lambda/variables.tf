variable "function_name" {
  description = "The name of the Lambda function"
  type        = string
}

variable "ecr_repository_url" {
  description = "The URL of the ECR repository containing the Lambda Docker image"
  type        = string
}

variable "image_tag" {
  description = "The tag of the Docker image to use for the Lambda function"
  type        = string
}

variable "memory_size" {
  description = "The amount of memory allocated to the Lambda function"
  type        = number
  default     = 128
}

variable "timeout" {
  description = "The timeout for the Lambda function in seconds"
  type        = number
  default     = 10
}

variable "environment_variables" {
  description = "Environment variables for the Lambda function"
  type        = map(string)
  default     = {
    IOT_TOPIC = "robot/draw"
  }
}

variable "tags" {
  description = "Tags to apply to the Lambda function"
  type        = map(string)
  default     = {
    Environment = "Production"
    Project     = "UNLP Plotter"
  }
}
