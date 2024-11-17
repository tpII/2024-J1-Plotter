# Reference an existing S3 bucket
data "aws_s3_bucket" "existing" {
  bucket = var.bucket_name
}

# Optional: Add encryption configuration
resource "aws_s3_bucket_server_side_encryption_configuration" "this" {
  bucket = data.aws_s3_bucket.existing.id

  rule {
    apply_server_side_encryption_by_default {
      sse_algorithm = "AES256"
    }
  }
}

# Optional: Add versioning configuration
resource "aws_s3_bucket_versioning" "this" {
  bucket = data.aws_s3_bucket.existing.id

  versioning_configuration {
    status = "Enabled"
  }
}