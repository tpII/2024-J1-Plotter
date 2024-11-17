async function sendAction(action) {
  const apiUrl = "https://<your-api-gateway-id>.execute-api.us-east-1.amazonaws.com/dev/action";

  try {
      const response = await fetch(`${apiUrl}?action=${action}`, {
          method: "GET", // or "POST" if you’re using a POST route
      });

      if (!response.ok) {
          throw new Error(`HTTP error! Status: ${response.status}`);
      }

      const data = await response.json();
      console.log("API response:", data);

      return data;
  } catch (error) {
      console.error("Error calling API:", error);
      throw error;
  }
}
