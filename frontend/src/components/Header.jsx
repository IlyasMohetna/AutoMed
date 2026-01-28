const Header = ({ connected, serverInfo }) => {
  return (
    <header className="bg-white shadow-sm border-b border-slate-200">
      <div className="max-w-7xl mx-auto px-4 py-6">
        <div className="flex items-center justify-between">
          <div>
            <h1 className="text-3xl font-bold text-slate-900">
              AutoMed
            </h1>
            <p className="text-slate-600 mt-1 text-sm">
              Simulateur de Blocs Opératoires
            </p>
          </div>
          <div className="flex items-center gap-4">
            <div className="flex items-center gap-2 bg-slate-50 px-3 py-2 rounded-lg border border-slate-200">
              <div
                className={`h-2 w-2 rounded-full ${
                  connected ? "bg-emerald-500 animate-pulse" : "bg-red-500"
                }`}
              ></div>
              <span
                className={`font-medium text-sm ${
                  connected ? "text-emerald-600" : "text-red-600"
                }`}
              >
                {connected ? "Connected" : "Disconnected"}
              </span>
            </div>
            {serverInfo && (
              <div className="hidden md:block text-sm text-slate-600 bg-slate-50 px-3 py-2 rounded-lg border border-slate-200">
                <span className="font-medium">v{serverInfo.version}</span>
              </div>
            )}
          </div>
        </div>
      </div>
    </header>
  );
};

export default Header;
