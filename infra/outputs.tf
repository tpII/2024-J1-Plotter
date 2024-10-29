output "api_gateway_url" {
  value = module.api_gateway.invoke_url
}

output "s3_website_url" {
  value = module.s3.website_endpoint
}

output "dynamodb_table_name" {
  value = module.dynamodb.table_name
}
