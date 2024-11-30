variable "bucket_name" {
  description = "Name of the S3 bucket for frontend files"
  type        = string
}

variable "index_document" {
  description = "The index document for the S3 website"
  type        = string
  default     = "index.html"
}

variable "error_document" {
  description = "The error document for the S3 website"
  type        = string
  default     = "error.html"
}

variable "tags" {
  description = "Tags to apply to the S3 bucket"
  type        = map(string)
  default     = {
    Environment = "Production"
    Project     = "UNLP Plotter"
  }
}
