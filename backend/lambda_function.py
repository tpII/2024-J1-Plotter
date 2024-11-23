import boto3
import json
import os

# Create IoT Data client
iot_client = boto3.client('iot-data', region_name='us-east-1')

def lambda_handler(event, context):
    try:
        # Check if the body is already a dictionary
        if isinstance(event['body'], str):
            drawing_data = json.loads(event['body'])
        else:
            drawing_data = event['body']

        # Publish to MQTT topic
        response = iot_client.publish(
            topic=os.environ['MQTT_TOPIC'],
            qos=1,
            payload=json.dumps(drawing_data)
        )

        return {
            'statusCode': 200,
            'body': json.dumps({'message': 'Drawing sent to robot successfully!'})
        }
    except Exception as e:
        print(f"Error: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
