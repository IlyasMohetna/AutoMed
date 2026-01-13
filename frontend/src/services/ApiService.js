class ApiService {
  constructor() {
    this.baseUrl = "http://localhost:8080/api";
  }

  async healthCheck() {
    try {
      const response = await fetch(`${this.baseUrl}/health`);
      if (!response.ok) throw new Error("Health check failed");
      return await response.json();
    } catch (error) {
      console.error("❌ Health check error:", error);
      throw error;
    }
  }

  async getInfo() {
    try {
      const response = await fetch(`${this.baseUrl}/info`);
      if (!response.ok) throw new Error("Failed to get info");
      return await response.json();
    } catch (error) {
      console.error("❌ Get info error:", error);
      throw error;
    }
  }

  async echo(data) {
    try {
      const response = await fetch(`${this.baseUrl}/echo`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      });
      if (!response.ok) throw new Error("Echo failed");
      return await response.json();
    } catch (error) {
      console.error("❌ Echo error:", error);
      throw error;
    }
  }
}

export default new ApiService();
