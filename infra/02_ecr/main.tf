resource "aws_ecr_repository" "lambda_repository" {
  name = var.repository_name

  image_scanning_configuration {
    scan_on_push = true
  }

  tags = var.tags
}
