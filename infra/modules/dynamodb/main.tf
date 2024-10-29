resource "aws_dynamodb_table" "drawing_data" {
  name         = var.table_name
  billing_mode = "PAY_PER_REQUEST"
  hash_key     = "drawing_id"

  attribute {
    name = "drawing_id"
    type = "S"
  }
}
