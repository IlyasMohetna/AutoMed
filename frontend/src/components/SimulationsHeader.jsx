import Button from "./ui/Button";

const SimulationsHeader = ({ simulationsCount, onCreateClick }) => {
  return (
    <div className="flex items-center justify-between mb-8">
      <div>
        <h2 className="text-2xl font-semibold text-slate-900 mb-1">
          Mes Simulations
        </h2>
        <p className="text-slate-600 text-sm">
          {simulationsCount} simulation{simulationsCount !== 1 ? "s" : ""} active
          {simulationsCount !== 1 ? "s" : ""}
        </p>
      </div>
      <Button
        onClick={onCreateClick}
        variant="primary"
        size="lg"
      >
        Nouvelle Simulation
      </Button>
    </div>
  );
};

export default SimulationsHeader;
