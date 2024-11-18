# Hola mundo 
exports.handler = async (event) => {
    console.log("Event received:", JSON.stringify(event, null, 2));

    // Extract HTTP method and request body
    const httpMethod = event.httpMethod || "UNKNOWN";
    const queryParams = event.queryStringParameters || {};
    const body = event.body ? JSON.parse(event.body) : {};

    let responseMessage;

    // Handle HTTP methods
    if (httpMethod === "GET") {
        responseMessage = `GET request received! Query parameters: ${JSON.stringify(queryParams)}`;
    } else if (httpMethod === "POST") {
        responseMessage = `POST request received! Body: ${JSON.stringify(body)}`;
    } else {
        responseMessage = `HTTP method ${httpMethod} is not supported.`;
    }

    // Return a response
    return {
        statusCode: 200,
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify({
            message: responseMessage,
            receivedEvent: event,
        }),
    };
};
