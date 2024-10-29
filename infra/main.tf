module "s3" {
  source = "./modules/s3"
  bucket_name = var.s3_bucket_name
}

module "dynamodb" {
  source = "./modules/dynamodb"
  table_name = var.dynamodb_table_name
}

module "iam" {
  source = "./modules/iam"
}

module "lambda" {
  source          = "./modules/lambda"
  dynamodb_table  = module.dynamodb.table_name
  role_arn        = module.iam.lambda_role_arn
  s3_bucket       = var.s3_bucket
  s3_key          = var.s3_key
}


module "api_gateway" {
  source         = "./modules/api_gateway"
  lambda_arn     = module.lambda.lambda_arn
}

module "iot" {
  source         = "./modules/iot"
  dynamodb_table = module.dynamodb.table_name
  role_arn       = module.iam.lambda_role_arn
}
