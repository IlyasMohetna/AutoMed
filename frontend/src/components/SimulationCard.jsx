import { useState, useEffect } from "react";
import StatusBadge from "./ui/StatusBadge";
import ProgressBar from "./ui/ProgressBar";
import StatCard from "./ui/StatCard";
import Button from "./ui/Button";

const SimulationCard = ({
  simulation,
  status: initialStatus,
  onStart,
  onPause,
  onResume,
  onStop,
  onDelete,
  onViewDetails,
}) => {
  const [status, setStatus] = useState(initialStatus);
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    setStatus(initialStatus);
  }, [initialStatus]);

  const handleAction = async (action) => {
    setLoading(true);
    try {
      await action();
    } finally {
      setLoading(false);
    }
  };

  return (
    <div className="bg-white rounded-xl shadow-sm hover:shadow-md transition-all duration-200 border border-slate-200 overflow-hidden">
      <div className="bg-indigo-600 p-4 text-white">
        <div className="flex items-start justify-between mb-2">
          <div className="flex-1">
            <h3 className="text-xl font-bold">
              {status?.nom || `Simulation #${simulation}`}
            </h3>
            <p className="text-indigo-100 text-sm mt-1">
              ID: {simulation} • {status?.algorithme || "N/A"}
            </p>
          </div>
          {status && <StatusBadge status={status.etat} />}
        </div>
      </div>

      {/* Corps de la carte */}
      <div className="p-5 space-y-4">
        {status && (
          <>
            {/* Barre de progression */}
            {status.progression !== undefined && (
              <ProgressBar
                value={status.tempsEcouleMinutes || 0}
                max={status.dureeSimulationMinutes || 0}
                label="Progression"
                showPercentage={true}
              />
            )}

            {/* Statistiques en grille */}
            <div className="grid grid-cols-2 gap-3">
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
            <div className="bg-slate-50 p-3 rounded-lg border border-slate-200">
              <div className="grid grid-cols-3 gap-2 text-center text-sm">
                <div>
                  <div className="text-slate-500 mb-1">Libres</div>
                  <div className="text-lg font-bold text-emerald-600">
                    {status.nombreBlocsLibres || 0}
                  </div>
                </div>
                <div>
                  <div className="text-slate-500 mb-1">Occupés</div>
                  <div className="text-lg font-bold text-red-600">
                    {status.nombreBlocsOccupes || 0}
                  </div>
                </div>
                <div>
                  <div className="text-slate-500 mb-1">Équipes</div>
                  <div className="text-lg font-bold text-indigo-600">
                    {status.nombreEquipesDisponibles || 0}
                  </div>
                </div>
              </div>
            </div>
          </>
        )}

        {/* Boutons d'action */}
        <div className="flex flex-wrap gap-2">
          {status?.etat === "CREATED" && (
            <Button
              onClick={() => handleAction(() => onStart(simulation))}
              variant="success"
              size="sm"
              loading={loading}
              className="flex-1"
            >
              Démarrer
            </Button>
          )}

          {status?.etat === "RUNNING" && (
            <Button
              onClick={() => handleAction(() => onPause(simulation))}
              variant="warning"
              size="sm"
              loading={loading}
              className="flex-1"
            >
              Pause
            </Button>
          )}

          {status?.etat === "PAUSED" && (
            <Button
              onClick={() => handleAction(() => onResume(simulation))}
              variant="info"
              size="sm"
              loading={loading}
              className="flex-1"
            >
              Reprendre
            </Button>
          )}

          {(status?.etat === "RUNNING" || status?.etat === "PAUSED") && (
            <Button
              onClick={() => handleAction(() => onStop(simulation))}
              variant="danger"
              size="sm"
              loading={loading}
              className="flex-1"
            >
              Arrêter
            </Button>
          )}

          <Button
            onClick={() => onViewDetails(simulation, status)}
            variant="primary"
            size="sm"
            className="flex-1"
          >
            Détails
          </Button>

          {(status?.etat === "STOPPED" ||
            status?.etat === "FINISHED" ||
            status?.etat === "CREATED") && (
            <Button
              onClick={() => handleAction(() => onDelete(simulation))}
              variant="secondary"
              size="sm"
              loading={loading}
            >
              Supprimer
            </Button>
          )}
        </div>
      </div>
    </div>
  );
};

export default SimulationCard;
