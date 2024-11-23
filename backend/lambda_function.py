import boto3
import json
import os

# Create IoT Data client
iot_client = boto3.client('iot-data', region_name='us-east-1')

def lambda_handler(event, context):
    # Parse incoming request
    drawing_data = json.loads(event['body'])

    # Publish to MQTT topic
    response = iot_client.publish(
        topic='robot/draw',
        qos=1,
        payload=json.dumps(drawing_data)
    )

    return {
        'statusCode': 200,
        'body': json.dumps({'message': 'Drawing sent to robot!'})
    }
