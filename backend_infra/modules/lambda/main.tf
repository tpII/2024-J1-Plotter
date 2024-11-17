resource "aws_lambda_function" "this" {
  function_name = var.function_name
  runtime       = "nodejs18.x"
  handler       = "index.handler" # The entry point in the "index.js" file
  role          = aws_iam_role.lambda_exec.arn

  # Use the S3 bucket and key to specify the Lambda code
  s3_bucket = var.s3_bucket_name
  s3_key    = var.s3_object_key
  environment {
    variables = var.environment_variables
  }
}

resource "aws_iam_role" "lambda_exec" {
  name = "${var.function_name}_execution_role"

  assume_role_policy = jsonencode({
    Version = "2012-10-17",
    Statement = [
      {
        Action = "sts:AssumeRole",
        Effect = "Allow",
        Principal = {
          Service = "lambda.amazonaws.com"
        }
      }
    ]
  })
}

resource "aws_iam_role_policy_attachment" "lambda_exec_policy" {
  role       = aws_iam_role.lambda_exec.name
  policy_arn = "arn:aws:iam::aws:policy/service-role/AWSLambdaBasicExecutionRole"
}

resource "aws_iam_policy" "lambda_s3_access" {
  name = "${var.function_name}_s3_access"

  policy = jsonencode({
    Version = "2012-10-17",
    Statement = [
      {
        Effect   = "Allow",
        Action   = "s3:GetObject",
        Resource = "arn:aws:s3:::${var.s3_bucket_name}/${var.s3_object_key}"
      }
    ]
  })
}

resource "aws_iam_role_policy_attachment" "lambda_s3_access_policy" {
  role       = aws_iam_role.lambda_exec.name
  policy_arn = aws_iam_policy.lambda_s3_access.arn
}
