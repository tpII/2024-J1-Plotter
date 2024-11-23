# Get AWS Account ID
data "aws_caller_identity" "current" {}

# Get IoT Core endpoint
data "aws_iot_endpoint" "endpoint" {
  endpoint_type = "iot:Data-ATS"
}

resource "aws_iot_policy" "robot_policy" {
  name   = var.policy_name
  policy = <<EOF
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "iot:Connect",
        "iot:Subscribe",
        "iot:Publish",
        "iot:Receive"
      ],
      "Resource": [
        "arn:aws:iot:${var.region}:${data.aws_caller_identity.current.account_id}:topic/robot/draw"
      ]
    }
  ]
}
EOF
}

resource "aws_iot_thing" "robot_thing" {
  name = var.thing_name
}

resource "aws_iot_certificate" "robot_cert" {
  active = true
}

resource "aws_iot_thing_principal_attachment" "robot_attachment" {
  thing     = aws_iot_thing.robot_thing.name
  principal = aws_iot_certificate.robot_cert.arn
}

resource "aws_iot_policy_attachment" "robot_policy_attachment" {
  policy   = aws_iot_policy.robot_policy.name
  target   = aws_iot_certificate.robot_cert.arn
}

resource "aws_iot_topic_rule" "log_to_cloudwatch" {
  name        = "log_messages"
  description = "Log messages from the robot/draw topic to CloudWatch Logs"
  enabled     = true

  sql         = "SELECT * FROM 'robot/draw'"
  sql_version = "2016-03-23"

  cloudwatch_logs {
    log_group_name = aws_cloudwatch_log_group.iot_log_group.name
    role_arn       = aws_iam_role.iot_log_role.arn
  }
}

resource "aws_cloudwatch_log_group" "iot_log_group" {
  name              = "/aws/iot/robot-draw"
  retention_in_days = var.log_retention_in_days
}

resource "aws_iam_role" "iot_log_role" {
  name = "iot_log_role"

  assume_role_policy = jsonencode({
    Version = "2012-10-17",
    Statement = [
      {
        Action    = "sts:AssumeRole",
        Effect    = "Allow",
        Principal = {
          Service = "iot.amazonaws.com"
        }
      }
    ]
  })
}

resource "aws_iam_role_policy" "iot_log_policy" {
  name   = "iot_log_policy"
  role   = aws_iam_role.iot_log_role.id
  policy = jsonencode({
    Version = "2012-10-17",
    Statement = [
      {
        Action   = ["logs:CreateLogGroup", "logs:CreateLogStream", "logs:PutLogEvents"],
        Effect   = "Allow",
        Resource = "arn:aws:logs:${var.region}:${data.aws_caller_identity.current.account_id}:log-group:/aws/iot/robot-draw:*"
      }
    ]
  })
}
