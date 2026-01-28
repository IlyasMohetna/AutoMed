import SimulationCard from "./SimulationCard";

const SimulationsList = ({
  simulations,
  simulationStatuses,
  onStart,
  onPause,
  onResume,
  onStop,
  onDelete,
  onView
}) => {
  return (
    <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
      {simulations.map((simId) => (
        <SimulationCard
          key={simId}
          simId={simId}
          status={simulationStatuses[simId]}
          onStart={() => onStart(simId)}
          onPause={() => onPause(simId)}
          onResume={() => onResume(simId)}
          onStop={() => onStop(simId)}
          onDelete={() => onDelete(simId)}
          onView={() => onView(simId)}
        />
      ))}
    </div>
  );
};

export default SimulationsList;
