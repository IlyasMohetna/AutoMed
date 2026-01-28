import Button from "./ui/Button";

const EmptyState = ({ onCreateClick }) => {
  return (
    <div className="bg-white rounded-2xl shadow-sm p-12 text-center border border-slate-200">
      <h3 className="text-2xl font-semibold text-slate-900 mb-3">
        Aucune Simulation
      </h3>
      <p className="text-slate-600 mb-8">
        Commencez par créer votre première simulation
      </p>
      <Button
        onClick={onCreateClick}
        variant="primary"
        size="lg"
      >
        Créer une Simulation
      </Button>
    </div>
  );
};

export default EmptyState;
