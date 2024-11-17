exports.handler = async (event) => {
    console.log("Request received:", JSON.stringify(event, null, 2));

    // Extract HTTP method and query parameters
    const httpMethod = event.httpMethod || "UNKNOWN";
    const queryParams = event.queryStringParameters || {};
    const body = event.body ? JSON.parse(event.body) : {};

    // Define a simple response object
    let responseMessage = "Hello from Lambda!";

    // Example logic based on HTTP method
    if (httpMethod === "GET") {
        responseMessage = "GET request received!";
        if (Object.keys(queryParams).length > 0) {
            responseMessage += ` Query parameters: ${JSON.stringify(queryParams)}`;
        }
    } else if (httpMethod === "POST") {
        responseMessage = `POST request received! Body: ${JSON.stringify(body)}`;
    } else {
        responseMessage = `HTTP method ${httpMethod} is not supported.`;
    }

    // Response back to API Gateway
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
