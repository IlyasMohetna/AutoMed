const StatCard = ({ value, label, color = "slate" }) => {
  const colorClasses = {
    slate: "bg-slate-50 border-slate-200 text-slate-600 text-slate-900",
    indigo: "bg-indigo-50 border-indigo-200 text-indigo-600 text-indigo-900",
    violet: "bg-violet-50 border-violet-200 text-violet-600 text-violet-900",
    emerald: "bg-emerald-50 border-emerald-200 text-emerald-600 text-emerald-900",
    red: "bg-red-50 border-red-200 text-red-600 text-red-900",
    amber: "bg-amber-50 border-amber-200 text-amber-600 text-amber-900",
  };

  const [bgClass, borderClass, labelClass, valueClass] = colorClasses[color]?.split(" ") || colorClasses.slate.split(" ");

  return (
    <div className={`${bgClass} p-3 rounded-lg border ${borderClass}`}>
      <div className={`text-xs ${labelClass} font-semibold mb-1`}>
        {label}
      </div>
      <div className={`text-2xl font-bold ${valueClass}`}>
        {value !== undefined ? value : "—"}
      </div>
    </div>
  );
};

export default StatCard;
