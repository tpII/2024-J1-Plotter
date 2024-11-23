variable "repository_name" {
  description = "Name of the ECR repository for Lambda Docker images"
  type        = string
}

variable "tags" {
  description = "Tags to apply to the ECR repository"
  type        = map(string)
  default     = {
    Environment = "Production"
    Project     = "UNLP Plotter"
  }
}
