output "bucket_name" {
  description = "The name of the S3 bucket"
  value       = aws_s3_bucket.frontend_bucket.bucket
}

output "website_url" {
  description = "The URL for the S3 static website"
  value       = aws_s3_bucket_website_configuration.frontend_website.website_endpoint
}
