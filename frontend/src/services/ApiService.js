import axios from 'axios';

class ApiService {
  constructor() {
    this.api = axios.create({
      baseURL: "http://localhost:8080/api",
      headers: {
        "Content-Type": "application/json",
      },
      timeout: 10000,
    });
  }

  // ========== HEALTH & INFO ==========
  async healthCheck() {
    try {
      const response = await this.api.get('/health');
      return response.data;
    } catch (error) {
      console.error("❌ Health check error:", error);
      throw error;
    }
  }

  async getInfo() {
    try {
      const response = await this.api.get('/info');
      return response.data;
    } catch (error) {
      console.error("❌ Get info error:", error);
      throw error;
    }
  }

  async echo(data) {
    try {
      const response = await this.api.post('/echo', data);
      return response.data;
    } catch (error) {
      console.error("❌ Echo error:", error);
      throw error;
    }
  }

  // ========== SIMULATION MANAGEMENT ==========
  
  async createSimulation(config) {
    try {
      const response = await this.api.post('/simulation/create', config);
      return response.data;
    } catch (error) {
      console.error("❌ Create simulation error:", error);
      throw error;
    }
  }

  async listSimulations() {
    try {
      const response = await this.api.get('/simulations');
      return response.data;
    } catch (error) {
      console.error("❌ List simulations error:", error);
      throw error;
    }
  }

  async startSimulation(simId) {
    try {
      const response = await this.api.post(`/simulation/${simId}/start`);
      return response.data;
    } catch (error) {
      console.error("❌ Start simulation error:", error);
      throw error;
    }
  }

  async pauseSimulation(simId) {
    try {
      const response = await this.api.post(`/simulation/${simId}/pause`);
      return response.data;
    } catch (error) {
      console.error("❌ Pause simulation error:", error);
      throw error;
    }
  }

  async resumeSimulation(simId) {
    try {
      const response = await this.api.post(`/simulation/${simId}/resume`);
      return response.data;
    } catch (error) {
      console.error("❌ Resume simulation error:", error);
      throw error;
    }
  }

  async stopSimulation(simId) {
    try {
      const response = await this.api.post(`/simulation/${simId}/stop`);
      return response.data;
    } catch (error) {
      console.error("❌ Stop simulation error:", error);
      throw error;
    }
  }

  async deleteSimulation(simId) {
    try {
      const response = await this.api.delete(`/simulation/${simId}`);
      return response.data;
    } catch (error) {
      console.error("❌ Delete simulation error:", error);
      throw error;
    }
  }

  // ========== SIMULATION DATA ==========

  async getSimulationStatus(simId) {
    try {
      const response = await this.api.get(`/simulation/${simId}/status`);
      return response.data;
    } catch (error) {
      console.error("❌ Get simulation status error:", error);
      throw error;
    }
  }

  async getSimulationStats(simId) {
    try {
      const response = await this.api.get(`/simulation/${simId}/stats`);
      return response.data;
    } catch (error) {
      console.error("❌ Get simulation stats error:", error);
      throw error;
    }
  }

  async getSimulationEvents(simId) {
    try {
      const response = await this.api.get(`/simulation/${simId}/events`);
      return response.data;
    } catch (error) {
      console.error("❌ Get simulation events error:", error);
      throw error;
    }
  }
}

export default new ApiService();
