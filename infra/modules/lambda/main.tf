resource "aws_lambda_function" "process_drawing" {
  function_name = "ProcessDrawing"
  runtime       = "nodejs14.x"
  handler       = "index.handler"
  role          = var.role_arn

  s3_bucket = var.s3_bucket
  s3_key    = var.s3_key

  environment {
    variables = {
      DYNAMODB_TABLE = var.dynamodb_table
    }
  }
}
