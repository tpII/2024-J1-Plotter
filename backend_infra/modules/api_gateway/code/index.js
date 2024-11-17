exports.handler = async (event) => {
    console.log("Request received:", event);

    // Extract query or body parameters from the request
    const { action } = event.queryStringParameters || {};

    // Process the request (for now, just log the action)
    const responseMessage = action ? `Action ${action} processed!` : "No action provided";

    return {
        statusCode: 200,
        body: JSON.stringify({ message: responseMessage }),
    };
};
