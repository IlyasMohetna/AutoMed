import { useState, useEffect } from "react";
import ApiService from "./services/ApiService";
import "./index.css";

function App() {
  const [connected, setConnected] = useState(false);
  const [serverInfo, setServerInfo] = useState(null);
  const [messages, setMessages] = useState([]);
  const [inputMessage, setInputMessage] = useState("");
  const [isChecking, setIsChecking] = useState(false);

  useEffect(() => {
    checkHealth();
    const healthInterval = setInterval(checkHealth, 5000);
    return () => clearInterval(healthInterval);
  }, []);

  const checkHealth = async () => {
    try {
      await ApiService.healthCheck();
      setConnected(true);
      if (messages.length === 0) {
        addMessage("✅ Serveur accessible", "system");
      }
      if (!serverInfo) {
        const info = await ApiService.getInfo();
        setServerInfo(info);
      }
    } catch (error) {
      setConnected(false);
      if (
        messages.length === 0 ||
        messages[messages.length - 1].text !== "❌ Serveur non accessible"
      ) {
        addMessage("❌ Serveur non accessible", "error");
      }
    }
  };

  const addMessage = (text, type) => {
    setMessages((prev) => [...prev, { text, type, timestamp: new Date() }]);
  };

  const sendMessage = async () => {
    if (!inputMessage.trim() || !connected) return;
    setIsChecking(true);
    try {
      const response = await ApiService.echo({ message: inputMessage });
      addMessage(`Envoyé: ${inputMessage}`, "sent");
      addMessage(`Réponse: ${JSON.stringify(response.received)}`, "received");
      setInputMessage("");
    } catch (error) {
      addMessage("⚠️ Erreur d'envoi", "error");
    } finally {
      setIsChecking(false);
    }
  };

  const handleKeyPress = (e) => {
    if (e.key === "Enter") sendMessage();
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-100">
      <header className="bg-white shadow-md">
        <div className="max-w-7xl mx-auto px-4 py-6">
          <div className="flex items-center justify-between">
            <div>
              <h1 className="text-3xl font-bold text-gray-900">
                🏥 AutoMed - Simulateur de Blocs Opératoires
              </h1>
              <p className="text-gray-600 mt-1">
                {serverInfo ? serverInfo.name : "Backend C++ REST API"}
              </p>
            </div>
            <div className="flex items-center gap-2">
              <div
                className={`h-3 w-3 rounded-full ${
                  connected ? "bg-green-500 animate-pulse" : "bg-red-500"
                }`}
              ></div>
              <span
                className={`font-semibold ${
                  connected ? "text-green-600" : "text-red-600"
                }`}
              >
                {connected ? "Connecté" : "Déconnecté"}
              </span>
            </div>
          </div>
        </div>
      </header>

      <main className="max-w-7xl mx-auto px-4 py-8">
        <div className="bg-white rounded-lg shadow-lg p-6">
          <h2 className="text-2xl font-bold text-gray-800 mb-4">
            🔌 Test de Communication REST API
          </h2>

          <div className="bg-gray-50 rounded-lg p-4 h-96 overflow-y-auto mb-4 border border-gray-200">
            {messages.length === 0 ? (
              <p className="text-gray-400 text-center mt-20">
                Aucun message. Envoyez un message pour tester l'API !
              </p>
            ) : (
              messages.map((msg, idx) => (
                <div
                  key={idx}
                  className={`mb-3 p-3 rounded-lg ${
                    msg.type === "sent"
                      ? "bg-blue-100 ml-auto max-w-md"
                      : msg.type === "received"
                      ? "bg-green-100 mr-auto max-w-md"
                      : msg.type === "system"
                      ? "bg-gray-200 text-center"
                      : "bg-red-100 text-center"
                  }`}
                >
                  <p className="text-sm font-medium text-gray-800">
                    {msg.text}
                  </p>
                  <p className="text-xs text-gray-500 mt-1">
                    {msg.timestamp.toLocaleTimeString()}
                  </p>
                </div>
              ))
            )}
          </div>

          <div className="flex gap-2">
            <input
              type="text"
              value={inputMessage}
              onChange={(e) => setInputMessage(e.target.value)}
              onKeyPress={handleKeyPress}
              placeholder="Tapez un message..."
              disabled={!connected || isChecking}
              className="flex-1 px-4 py-2 border border-gray-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500 disabled:bg-gray-100 disabled:cursor-not-allowed"
            />
            <button
              onClick={sendMessage}
              disabled={!connected || !inputMessage.trim() || isChecking}
              className="px-6 py-2 bg-blue-600 text-white rounded-lg hover:bg-blue-700 disabled:bg-gray-400 disabled:cursor-not-allowed font-semibold transition"
            >
              {isChecking ? "Envoi..." : "Envoyer"}
            </button>
          </div>

          {!connected && (
            <p className="text-yellow-600 text-sm mt-3 text-center">
              ⚠️ En attente de connexion au serveur backend...
            </p>
          )}
        </div>

        <div className="mt-6 bg-blue-50 border border-blue-200 rounded-lg p-4">
          <h3 className="font-bold text-blue-900 mb-2">ℹ️ Informations</h3>
          <ul className="text-sm text-blue-800 space-y-1">
            <li>✓ Backend C++ avec REST API (Crow) sur le port 8080</li>
            <li>✓ Frontend React avec Vite sur le port 3000</li>
            <li>✓ Communication HTTP avec health check automatique</li>
            <li>✓ Hot-reload activé sur les deux containers</li>
          </ul>
          {serverInfo && (
            <div className="mt-3 p-2 bg-white rounded border border-blue-300">
              <p className="text-xs text-blue-700">
                <strong>Backend:</strong> {serverInfo.name} v
                {serverInfo.version}
              </p>
            </div>
          )}
        </div>
      </main>
    </div>
  );
}

export default App;
