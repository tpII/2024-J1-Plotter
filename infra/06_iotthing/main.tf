# Fetch the IoT Core endpoint
data "aws_iot_endpoint" "endpoint" {
  endpoint_type = "iot:Data-ATS"
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
        "arn:aws:iot:${var.region}:${data.aws_caller_identity.current.account_id}:topic/*"
      ]
    }
  ]
}
EOF
}

resource "aws_iot_policy_attachment" "robot_policy_attachment" {
  policy   = aws_iot_policy.robot_policy.name
  target   = aws_iot_certificate.robot_cert.arn
}

# Data source to get the account ID
data "aws_caller_identity" "current" {}
