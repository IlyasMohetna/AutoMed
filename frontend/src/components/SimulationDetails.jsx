import { useState, useEffect } from "react";
import StatCard from "./ui/StatCard";
import ProgressBar from "./ui/ProgressBar";
import Button from "./ui/Button";

const SimulationDetails = ({ simulationId, onClose }) => {
  const [activeTab, setActiveTab] = useState("status");
  const [status, setStatus] = useState(null);
  const [stats, setStats] = useState(null);
  const [events, setEvents] = useState([]);
  const [loading, setLoading] = useState(true);
  const [autoRefresh, setAutoRefresh] = useState(true);

  useEffect(() => {
    loadData();
    let interval;
    if (autoRefresh) {
      interval = setInterval(loadData, 2000);
    }
    return () => clearInterval(interval);
  }, [simulationId, autoRefresh, activeTab]);

  const loadData = async () => {
    try {
      const ApiService = (await import("../services/ApiService")).default;
      
      if (activeTab === "status") {
        const statusData = await ApiService.getSimulationStatus(simulationId);
        setStatus(statusData);
      } else if (activeTab === "stats") {
        const statsData = await ApiService.getSimulationStats(simulationId);
        setStats(statsData);
      } else if (activeTab === "events") {
        const eventsData = await ApiService.getSimulationEvents(simulationId);
        setEvents(eventsData.events || []);
      }
    } catch (error) {
      console.error("Error loading data:", error);
    } finally {
      setLoading(false);
    }
  };

  const tabs = [
    { id: "status", label: "État" },
    { id: "stats", label: "Statistiques" },
    { id: "events", label: "Événements" },
  ];

  return (
    <div className="fixed inset-0 bg-black bg-opacity-50 z-50 flex items-center justify-center p-4">
      <div className="absolute inset-0" onClick={onClose}></div>
      <div className="bg-white rounded-2xl shadow-2xl max-w-6xl w-full max-h-[90vh] overflow-hidden flex flex-col relative z-10 animate-fade-in">
        {/* Header */}
        <div className="bg-indigo-600 text-white p-6">
          <div className="flex items-center justify-between">
            <div>
              <h2 className="text-3xl font-bold mb-2">
                Simulation #{simulationId}
              </h2>
              <p className="text-indigo-100">
                {status?.nom || "Chargement..."}
              </p>
            </div>
            <div className="flex items-center gap-4">
              <label className="flex items-center gap-2 bg-white/20 px-4 py-2 rounded-lg cursor-pointer hover:bg-white/30 transition">
                <input
                  type="checkbox"
                  checked={autoRefresh}
                  onChange={(e) => setAutoRefresh(e.target.checked)}
                  className="w-4 h-4"
                />
                <span className="text-sm font-medium">Auto-refresh</span>
              </label>
              <button
                onClick={onClose}
                className="text-white hover:bg-white/20 rounded-full p-2 transition"
              >
                <span className="text-2xl">×</span>
              </button>
            </div>
          </div>
        </div>

        {/* Tabs */}
        <div className="bg-slate-100 border-b border-slate-200">
          <div className="flex gap-1 p-2">
            {tabs.map((tab) => (
              <button
                key={tab.id}
                onClick={() => setActiveTab(tab.id)}
                className={`flex-1 px-6 py-3 rounded-lg font-semibold transition ${
                  activeTab === tab.id
                    ? "bg-white text-indigo-600 shadow-md"
                    : "text-slate-600 hover:bg-slate-200"
                }`}
              >
                {tab.label}
              </button>
            ))}
          </div>
        </div>

        {/* Content */}
        <div className="flex-1 overflow-y-auto p-6">
          {loading && (
            <div className="text-center py-12">
              <div className="inline-block animate-spin rounded-full h-12 w-12 border-4 border-indigo-500 border-t-transparent"></div>
              <p className="text-slate-600 mt-4">Chargement...</p>
            </div>
          )}

          {!loading && activeTab === "status" && status && (
            <StatusTab status={status} />
          )}

          {!loading && activeTab === "stats" && stats && (
            <StatsTab stats={stats} />
          )}

          {!loading && activeTab === "events" && (
            <EventsTab events={events} />
          )}
        </div>

        {/* Footer */}
        <div className="bg-slate-50 border-t border-slate-200 p-4 flex justify-end">
          <Button onClick={onClose} variant="primary" size="lg">
            Fermer
          </Button>
        </div>
      </div>
    </div>
  );
};

const StatusTab = ({ status }) => (
  <div className="space-y-6">
    {/* Progression */}
    <div className="bg-indigo-50 p-6 rounded-xl border-2 border-indigo-200">
      <h3 className="text-xl font-bold text-slate-800 mb-4">Progression</h3>
      <ProgressBar
        value={status.tempsEcouleMinutes || 0}
        max={status.dureeSimulationMinutes || 0}
        label=""
        showPercentage={false}
      />
      <div className="text-center text-2xl font-bold text-indigo-600 mt-3">
        {status.progression?.toFixed(1)}%
      </div>
    </div>

    {/* Patients */}
    <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
      <StatCard
        value={status.nombrePatientsEnAttente || 0}
        label="En Attente"
        color="slate"
      />
      <StatCard
        value={status.nombrePatientsEnOperation || 0}
        label="En Opération"
        color="indigo"
      />
      <StatCard
        value={status.nombrePatientsEnReveil || 0}
        label="En Réveil"
        color="violet"
      />
      <StatCard
        value={status.nombrePatientsTraites || 0}
        label="Traités"
        color="emerald"
      />
    </div>

    {/* Ressources */}
    <div className="bg-slate-50 p-6 rounded-xl border-2 border-slate-200">
      <h3 className="text-xl font-bold text-slate-800 mb-4">Ressources</h3>
      <div className="grid grid-cols-3 gap-4">
        <div className="text-center">
          <div className="text-3xl font-bold text-emerald-600">
            {status.nombreBlocsLibres || 0}
          </div>
          <div className="text-sm text-slate-600 mt-1">Blocs Libres</div>
        </div>
        <div className="text-center">
          <div className="text-3xl font-bold text-red-600">
            {status.nombreBlocsOccupes || 0}
          </div>
          <div className="text-sm text-slate-600 mt-1">Blocs Occupés</div>
        </div>
        <div className="text-center">
          <div className="text-3xl font-bold text-indigo-600">
            {status.nombreEquipesDisponibles || 0}
          </div>
          <div className="text-sm text-slate-600 mt-1">Équipes Dispo</div>
        </div>
      </div>
    </div>

    {/* Infos */}
    <div className="bg-indigo-50 p-4 rounded-xl border-2 border-indigo-200">
      <div className="grid grid-cols-2 gap-4 text-sm">
        <div>
          <span className="text-slate-600">État:</span>
          <span className="ml-2 font-bold text-indigo-600">{status.etat}</span>
        </div>
        <div>
          <span className="text-slate-600">Algorithme:</span>
          <span className="ml-2 font-bold text-indigo-600">{status.algorithme}</span>
        </div>
      </div>
    </div>
  </div>
);

const StatsTab = ({ stats }) => (
  <div className="space-y-6">
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
      {Object.entries(stats).map(([key, value]) => {
        if (key === "simulationId") return null;
        return (
          <div
            key={key}
            className="bg-white p-4 rounded-xl border-2 border-slate-200 hover:border-indigo-300 transition"
          >
            <div className="text-sm text-slate-600 mb-1 font-medium">
              {key.replace(/([A-Z])/g, " $1").trim()}
            </div>
            <div className="text-2xl font-bold text-slate-800">
              {typeof value === "number" ? value.toFixed(2) : value}
            </div>
          </div>
        );
      })}
    </div>

    {Object.keys(stats).length === 1 && (
      <div className="text-center py-12 text-slate-500">
        <p className="text-lg">Aucune statistique disponible pour le moment</p>
      </div>
    )}
  </div>
);

const EventsTab = ({ events }) => (
  <div className="space-y-3">
    {events.length === 0 && (
      <div className="text-center py-12 text-slate-500">
        <p className="text-lg">Aucun événement enregistré</p>
      </div>
    )}

    {events.map((event, index) => (
      <div
        key={index}
        className="bg-white border-l-4 border-indigo-500 p-4 rounded-r-lg shadow-sm hover:shadow-md transition"
      >
        <div className="flex items-start justify-between">
          <div className="flex-1">
            <div className="font-semibold text-slate-800">{event.description}</div>
            <div className="text-sm text-slate-500 mt-1">Type: {event.type}</div>
          </div>
          <div className="text-sm text-slate-600 font-medium">
            {event.timestamp} min
          </div>
        </div>
      </div>
    ))}
  </div>
);

export default SimulationDetails;
