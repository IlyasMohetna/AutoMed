const ProgressBar = ({ value, max, label, showPercentage = true }) => {
  const percentage = max > 0 ? (value / max) * 100 : 0;

  return (
    <div className="w-full">
      {label && (
        <div className="flex justify-between text-sm mb-1">
          <span className="text-slate-600 font-medium">{label}</span>
          {showPercentage && (
            <span className="text-indigo-600 font-bold">
              {percentage.toFixed(1)}%
            </span>
          )}
        </div>
      )}
      <div className="w-full bg-slate-200 rounded-full h-3 overflow-hidden">
        <div
          className="bg-indigo-600 h-3 rounded-full transition-all duration-500"
          style={{ width: `${Math.min(percentage, 100)}%` }}
        ></div>
      </div>
      {!showPercentage && (
        <div className="text-xs text-slate-500 mt-1 text-center">
          {value} / {max}
        </div>
      )}
    </div>
  );
};

export default ProgressBar;
