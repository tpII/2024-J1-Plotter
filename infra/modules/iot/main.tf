resource "aws_iot_topic_rule" "send_command_to_esp32" {
  name = "SendCommandToESP32"
  sql  = "SELECT * FROM 'drawing_topic'"

  dynamodb {
    table_name = var.dynamodb_table
    role_arn   = var.role_arn
  }
}
