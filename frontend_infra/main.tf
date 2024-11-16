provider "aws" {
  region = "us-east-1" # Adjust based on your preference
}

# S3 Bucket for static site hosting
resource "aws_s3_bucket" "frontend" {
  bucket = "2024-unlp-plotter-frontend"
  force_destroy = true 

  tags = {
    Environment = "Development"
    Name        = "StaticSiteBucket"
  }
}

# S3 Bucket Policy to allow public read access
resource "aws_s3_bucket_policy" "frontend_policy" {
  bucket = aws_s3_bucket.frontend.id
  policy = jsonencode({
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Principal": "*",
      "Action": "s3:GetObject",
      "Resource": "arn:aws:s3:::2024-unlp-plotter-frontend/*"
    }
  ]
})
}

# CloudFront Distribution for HTTPS
resource "aws_cloudfront_distribution" "frontend_cdn" {
  origin {
    domain_name = aws_s3_bucket.frontend.bucket_regional_domain_name
    origin_id   = "S3-static-site-origin"
  }

  enabled             = true
  default_root_object = "index.html"

  default_cache_behavior {
    allowed_methods  = ["GET", "HEAD"]
    cached_methods   = ["GET", "HEAD"]
    target_origin_id = "S3-static-site-origin"

    viewer_protocol_policy = "redirect-to-https"

    forwarded_values {
      query_string = false
      cookies {
        forward = "none"
      }
    }
  }

  viewer_certificate {
    cloudfront_default_certificate = true # Use default SSL
  }

  restrictions {
    geo_restriction {
      restriction_type = "none"
    }
  }

  tags = {
    Environment = "Development"
    Name        = "StaticSiteDistribution"
  }
}

# Output CloudFront URL
output "cloudfront_url" {
  value       = aws_cloudfront_distribution.frontend_cdn.domain_name
  description = "URL of the static website served via CloudFront"
}
