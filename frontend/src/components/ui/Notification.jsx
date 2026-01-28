const Notification = ({ message, type = "success", onClose }) => {
  if (!message) return null;

  const typeClasses = {
    success: "bg-green-50 border-green-300 text-green-800",
    error: "bg-red-50 border-red-300 text-red-800",
    warning: "bg-yellow-50 border-yellow-300 text-yellow-800",
    info: "bg-blue-50 border-blue-300 text-blue-800",
  };

  const icons = {
    success: "✅",
    error: "❌",
    warning: "⚠️",
    info: "ℹ️",
  };

  return (
    <div className="fixed top-20 right-4 z-50 animate-slide-in-right">
      <div
        className={`px-6 py-4 rounded-lg shadow-lg border-2 flex items-center gap-3 min-w-[300px] ${typeClasses[type]}`}
      >
        <span className="text-2xl">{icons[type]}</span>
        <p className="font-semibold flex-1">{message}</p>
        {onClose && (
          <button
            onClick={onClose}
            className="text-gray-600 hover:text-gray-800 font-bold text-xl"
          >
            ✕
          </button>
        )}
      </div>
    </div>
  );
};

export default Notification;
