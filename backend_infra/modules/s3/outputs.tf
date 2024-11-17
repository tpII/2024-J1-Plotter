output "bucket_name" {
  description = "The name of the existing S3 bucket"
  value       = data.aws_s3_bucket.existing.bucket
}

output "bucket_arn" {
  description = "The ARN of the existing S3 bucket"
  value       = data.aws_s3_bucket.existing.arn
}
