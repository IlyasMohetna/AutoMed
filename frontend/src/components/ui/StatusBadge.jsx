const StatusBadge = ({ status }) => {
  const getStateColor = (etat) => {
    const colors = {
      CREATED: "bg-slate-100 text-slate-700 border-slate-300",
      RUNNING: "bg-emerald-100 text-emerald-700 border-emerald-300 animate-pulse",
      PAUSED: "bg-amber-100 text-amber-700 border-amber-300",
      STOPPED: "bg-red-100 text-red-700 border-red-300",
      FINISHED: "bg-indigo-100 text-indigo-700 border-indigo-300",
    };
    return colors[etat] || "bg-slate-100 text-slate-700";
  };

  const getStateLabel = (etat) => {
    const labels = {
      CREATED: "Créée",
      RUNNING: "En cours",
      PAUSED: "En pause",
      STOPPED: "Arrêtée",
      FINISHED: "Terminée",
    };
    return labels[etat] || etat;
  };

  return (
    <div
      className={`px-3 py-1 rounded-full border-2 text-sm font-bold inline-flex items-center ${getStateColor(
        status
      )}`}
    >
      <span>{getStateLabel(status)}</span>
    </div>
  );
};

export default StatusBadge;
