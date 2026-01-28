const ConnectionAlert = () => {
  return (
    <div className="bg-amber-50 border border-amber-200 rounded-xl p-8 mb-6 text-center">
      <h2 className="text-2xl font-semibold text-amber-900 mb-2">
        Serveur Non Connecté
      </h2>
      <p className="text-amber-700">
        En attente de connexion au backend AutoMed...
      </p>
    </div>
  );
};

export default ConnectionAlert;
