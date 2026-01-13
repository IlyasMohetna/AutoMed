class WebSocketService {
  constructor() {
    this.ws = null;
    this.reconnectInterval = 3000;
    this.reconnectTimer = null;
    this.listeners = {
      onOpen: [],
      onMessage: [],
      onClose: [],
      onError: [],
    };
  }

  connect(url = "ws://localhost:8080") {
    try {
      console.log("🔌 Connexion au serveur WebSocket...", url);
      this.ws = new WebSocket(url);

      this.ws.onopen = (event) => {
        console.log("✅ Connexion WebSocket établie");
        this.clearReconnectTimer();
        this.listeners.onOpen.forEach((callback) => callback(event));
      };

      this.ws.onmessage = (event) => {
        console.log("📩 Message reçu:", event.data);
        this.listeners.onMessage.forEach((callback) => callback(event.data));
      };

      this.ws.onclose = (event) => {
        console.log("🔌 Connexion WebSocket fermée");
        this.listeners.onClose.forEach((callback) => callback(event));
        this.scheduleReconnect(url);
      };

      this.ws.onerror = (error) => {
        console.error("❌ Erreur WebSocket:", error);
        this.listeners.onError.forEach((callback) => callback(error));
      };
    } catch (error) {
      console.error("❌ Erreur lors de la connexion:", error);
      this.scheduleReconnect(url);
    }
  }

  scheduleReconnect(url) {
    if (this.reconnectTimer) return;

    console.log(`⏳ Reconnexion dans ${this.reconnectInterval / 1000}s...`);
    this.reconnectTimer = setTimeout(() => {
      this.reconnectTimer = null;
      this.connect(url);
    }, this.reconnectInterval);
  }

  clearReconnectTimer() {
    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer);
      this.reconnectTimer = null;
    }
  }

  send(data) {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      const message = typeof data === "string" ? data : JSON.stringify(data);
      console.log("📤 Envoi:", message);
      this.ws.send(message);
      return true;
    } else {
      console.warn(
        "⚠️ WebSocket non connecté. Impossible d'envoyer le message."
      );
      return false;
    }
  }

  on(event, callback) {
    if (this.listeners[event]) {
      this.listeners[event].push(callback);
    }
  }

  off(event, callback) {
    if (this.listeners[event]) {
      this.listeners[event] = this.listeners[event].filter(
        (cb) => cb !== callback
      );
    }
  }

  disconnect() {
    this.clearReconnectTimer();
    if (this.ws) {
      this.ws.close();
      this.ws = null;
    }
  }

  isConnected() {
    return this.ws && this.ws.readyState === WebSocket.OPEN;
  }
}

export default new WebSocketService();
