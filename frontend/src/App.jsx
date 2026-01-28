import { useState, useEffect } from "react";
import ApiService from "./services/ApiService";
import Header from "./components/Header";
import ConnectionAlert from "./components/ConnectionAlert";
import SimulationsHeader from "./components/SimulationsHeader";
import EmptyState from "./components/EmptyState";
import SimulationsList from "./components/SimulationsList";
import SimulationForm from "./components/SimulationForm";
import SimulationDetails from "./components/SimulationDetails";
import Modal from "./components/ui/Modal";
import Notification from "./components/ui/Notification";
import "./index.css";

function App() {
  const [connected, setConnected] = useState(false);
  const [serverInfo, setServerInfo] = useState(null);
  const [simulations, setSimulations] = useState([]);
  const [simulationStatuses, setSimulationStatuses] = useState({});
  const [showForm, setShowForm] = useState(false);
  const [selectedSimulation, setSelectedSimulation] = useState(null);
  const [notification, setNotification] = useState(null);

  useEffect(() => {
    checkHealth();
    const healthInterval = setInterval(checkHealth, 5000);
    return () => clearInterval(healthInterval);
  }, []);

  useEffect(() => {
    if (connected) {
      loadSimulations();
      const simInterval = setInterval(loadSimulations, 3000); // Refresh toutes les 3 secondes
      return () => clearInterval(simInterval);
    }
  }, [connected]);

  useEffect(() => {
    if (simulations.length > 0) {
      loadAllStatuses();
    }
  }, [simulations]);

  const checkHealth = async () => {
    try {
      await ApiService.healthCheck();
      setConnected(true);
      if (!serverInfo) {
        const info = await ApiService.getInfo();
        setServerInfo(info);
      }
    } catch (error) {
      setConnected(false);
    }
  };

  const loadSimulations = async () => {
    try {
      const data = await ApiService.listSimulations();
      setSimulations(data.simulations || []);
    } catch (error) {
      console.error("Error loading simulations:", error);
    }
  };

  const loadAllStatuses = async () => {
    const statuses = {};
    for (const simId of simulations) {
      try {
        const status = await ApiService.getSimulationStatus(simId);
        statuses[simId] = status;
      } catch (error) {
        console.error(`Error loading status for sim ${simId}:`, error);
      }
    }
    setSimulationStatuses(statuses);
  };

  const showNotification = (message, type = "success") => {
    setNotification({ message, type });
    setTimeout(() => setNotification(null), 5000);
  };

  const handleCreateSimulation = async (config) => {
    try {
      const result = await ApiService.createSimulation(config);
      showNotification(
        `Simulation #${result.simulationId} créée avec succès`,
        "success"
      );
      setShowForm(false);
      loadSimulations();
    } catch (error) {
      showNotification("Erreur lors de la création de la simulation", "error");
    }
  };

  const handleStartSimulation = async (simId) => {
    try {
      await ApiService.startSimulation(simId);
      showNotification(`Simulation #${simId} démarrée`, "success");
      loadSimulations();
    } catch (error) {
      showNotification(`Erreur lors du démarrage`, "error");
    }
  };

  const handlePauseSimulation = async (simId) => {
    try {
      await ApiService.pauseSimulation(simId);
      showNotification(`Simulation #${simId} mise en pause`, "success");
      loadSimulations();
    } catch (error) {
      showNotification(`Erreur lors de la mise en pause`, "error");
    }
  };

  const handleResumeSimulation = async (simId) => {
    try {
      await ApiService.resumeSimulation(simId);
      showNotification(`Simulation #${simId} reprise`, "success");
      loadSimulations();
    } catch (error) {
      showNotification(`Erreur lors de la reprise`, "error");
    }
  };

  const handleStopSimulation = async (simId) => {
    try {
      await ApiService.stopSimulation(simId);
      showNotification(`Simulation #${simId} arrêtée`, "success");
      loadSimulations();
    } catch (error) {
      showNotification(`Erreur lors de l'arrêt`, "error");
    }
  };

  const handleDeleteSimulation = async (simId) => {
    if (!confirm(`Êtes-vous sûr de vouloir supprimer la simulation #${simId} ?`)) {
      return;
    }
    try {
      await ApiService.deleteSimulation(simId);
      showNotification(`Simulation #${simId} supprimée`, "success");
      loadSimulations();
    } catch (error) {
      showNotification(`Erreur lors de la suppression`, "error");
    }
  };

  return (
    <div className="min-h-screen bg-slate-50">
      <Header connected={connected} serverInfo={serverInfo} />

      <Notification
        message={notification?.message}
        type={notification?.type}
        onClose={() => setNotification(null)}
      />

      <main className="max-w-7xl mx-auto px-4 py-8">
        {!connected && <ConnectionAlert />}

        {connected && (
          <>
            <SimulationsHeader
              simulationsCount={simulations.length}
              onCreateClick={() => setShowForm(true)}
            />

            {simulations.length === 0 && !showForm && (
              <EmptyState onCreateClick={() => setShowForm(true)} />
            )}

            {simulations.length > 0 && (
              <SimulationsList
                simulations={simulations}
                simulationStatuses={simulationStatuses}
                onStart={handleStartSimulation}
                onPause={handlePauseSimulation}
                onResume={handleResumeSimulation}
                onStop={handleStopSimulation}
                onDelete={handleDeleteSimulation}
                onView={(id) => setSelectedSimulation(id)}
              />
            )}
          </>
        )}
      </main>

      <Modal
        isOpen={showForm}
        onClose={() => setShowForm(false)}
        size="lg"
      >
        <SimulationForm
          onSimulationCreated={handleCreateSimulation}
          onCancel={() => setShowForm(false)}
        />
      </Modal>

      {selectedSimulation && (
        <SimulationDetails
          simulationId={selectedSimulation}
          onClose={() => setSelectedSimulation(null)}
        />
      )}

      <footer className="bg-white border-t border-gray-200 mt-12 py-6">
        <div className="max-w-7xl mx-auto px-4 text-center text-gray-600 text-sm">
          <p>
            <span className="font-semibold">AutoMed</span> - Simulateur de Blocs
            Opératoires avec C++ Backend
          </p>
          <p className="mt-1">
            Backend: {serverInfo?.type || "REST API"} • Frontend: React + Vite +
            Tailwind CSS
          </p>
        </div>
      </footer>
    </div>
  );
}

export default App;
