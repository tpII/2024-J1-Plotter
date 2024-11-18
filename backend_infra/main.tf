provider "aws" {
  region = var.aws_region
}

# Reference an existing S3 bucket
data "aws_s3_bucket" "existing" {
  bucket = var.s3_bucket_name
}

# Lambda module for the backend function
module "lambda" {
  source              = "./modules/lambda"
  function_name       = var.lambda_function_name
  s3_bucket_name      = var.s3_bucket_name # Referencia directa al bucket existente
  s3_object_key       = var.s3_object_key
  environment_variables = var.lambda_environment_variables
}



# API Gateway module for exposing Lambda as an HTTP endpoint
module "api_gateway" {
  source              = "./modules/api_gateway"
  api_name            = "drawing-robot-api"
  aws_region          = var.aws_region
  lambda_function_arn = module.lambda.function_arn
}

