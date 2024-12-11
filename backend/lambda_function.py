import boto3
import json
import os

# Create IoT Data client
iot_client = boto3.client('iot-data', region_name='us-east-1')

# Environment variable for the MQTT topic
MQTT_TOPIC = os.environ.get('MQTT_TOPIC', 'robot/draw')

def lambda_handler(event, context):
    try:
        # Parse the request body
        if isinstance(event['body'], str):
            payload = json.loads(event['body'])
        else:
            payload = event['body']

        # Validate the payload structure
        if 'command' not in payload:
            raise ValueError("Missing 'command' in payload")

        command = payload['command']
        data = payload.get('data', "")

        # Prepare the message for MQTT
        mqtt_message = {
            "command": command,
            "data": data
        }

        # Publish the message to the MQTT topic
        iot_client.publish(
            topic=MQTT_TOPIC,
            qos=1,
            payload=json.dumps(mqtt_message)
        )

        # Successful response
        return {
            'statusCode': 200,
            'body': json.dumps({
                'message': f"Command '{command}' published successfully!"
            })
        }
    except ValueError as ve:
        # Handle missing 'command' key
        print(f"Validation Error: {str(ve)}")
        return {
            'statusCode': 400,
            'body': json.dumps({'error': str(ve)})
        }
    except Exception as e:
        # Handle any other error
        print(f"Error: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
