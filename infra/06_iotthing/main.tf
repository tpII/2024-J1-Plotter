# IoT Thing Resource
resource "aws_iot_thing" "robot_thing" {
  name = var.thing_name
}

# IoT Certificate
resource "aws_iot_certificate" "robot_cert" {
  active = true
}

# Attach the Certificate to the Thing
resource "aws_iot_thing_principal_attachment" "robot_attachment" {
  thing     = aws_iot_thing.robot_thing.name
  principal = aws_iot_certificate.robot_cert.arn
}

# IoT Policy
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

# Attach the Policy to the Certificate
resource "aws_iot_policy_attachment" "robot_policy_attachment" {
  policy   = aws_iot_policy.robot_policy.name
  target   = aws_iot_certificate.robot_cert.arn
}

# Data source to get the account ID
data "aws_caller_identity" "current" {}
