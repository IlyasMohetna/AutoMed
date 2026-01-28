const SpeedSelector = ({ value, onChange }) => {
  const speedModes = [
    { value: 0.0, label: "Instantané (quelques secondes)", color: "text-violet-600" },
    { value: 60.0, label: "Rapide (1 min = 1 sec)", color: "text-blue-600" },
    { value: 96.0, label: "Observable (480 min ≈ 5 min)", color: "text-emerald-600" },
    { value: 600.0, label: "Très Rapide (480 min ≈ 48 sec)", color: "text-amber-600" },
    { value: 1.0, label: "Temps Réel (480 min = 8h)", color: "text-rose-600" },
  ];

  return (
    <div className="bg-slate-50 p-5 rounded-xl border border-slate-200">
      <label className="block text-sm font-semibold text-slate-900 mb-4 uppercase tracking-wide">
        Vitesse de Simulation
      </label>
      <div className="space-y-2">
        {speedModes.map((mode) => (
          <label
            key={mode.value}
            className={`flex items-center gap-3 p-3 rounded-lg cursor-pointer transition-all duration-200 ${
              value === mode.value
                ? "bg-white shadow-sm border border-indigo-300 ring-2 ring-indigo-100"
                : "hover:bg-white border border-transparent"
            }`}
          >
            <input
              type="radio"
              name="facteurVitesse"
              checked={value === mode.value}
              onChange={() => onChange(mode.value)}
              className="w-4 h-4 text-indigo-600 cursor-pointer focus:ring-2 focus:ring-indigo-500"
            />
            <span className={`font-medium text-sm ${mode.color}`}>
              {mode.label}
            </span>
          </label>
        ))}
      </div>
    </div>
  );
};

export default SpeedSelector;
