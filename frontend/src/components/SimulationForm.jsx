import { useState } from "react";
import FormInput from "./form/FormInput";
import FormSelect from "./form/FormSelect";
import SpeedSelector from "./form/SpeedSelector";
import ResourcesSection from "./form/ResourcesSection";
import PatientsSection from "./form/PatientsSection";
import Button from "./ui/Button";

const SimulationForm = ({ onSimulationCreated, onCancel }) => {
  const [formData, setFormData] = useState({
    nom: "",
    dureeSimulationMinutes: 480,
    algorithme: "FCFS",
    nombreBlocs: 3,
    nombreEquipes: 3,
    capaciteSalleAttente: 50,
    capaciteSalleReveil: 20,
    tauxArriveeHoraireUrgences: 2.0,
    nombrePatientsElectifs: 10,
    facteurVitesse: 0.0,
  });

  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleChange = (e) => {
    const { name, value, type } = e.target;
    const numericValue = type === "number" ? parseFloat(value) : value;
    setFormData((prev) => ({
      ...prev,
      [name]: numericValue,
    }));
  };

  const handleSpeedChange = (speed) => {
    setFormData((prev) => ({
      ...prev,
      facteurVitesse: speed,
    }));
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setIsSubmitting(true);
    try {
      await onSimulationCreated(formData);
    } finally {
      setIsSubmitting(false);
    }
  };

  const algorithmeOptions = [
    { value: "FCFS", label: "FCFS - First Come First Served" },
    { value: "SJF", label: "SJF - Shortest Job First" },
    { value: "PRIORITY", label: "PRIORITY - Par Priorité" },
    { value: "RR", label: "RR - Round Robin" },
  ];

  return (
    <div className="bg-white rounded-2xl shadow-xl border border-slate-200">
      <div className="flex items-center justify-between p-6 border-b border-slate-200">
        <h2 className="text-2xl font-semibold text-slate-900">
          Nouvelle Simulation
        </h2>
        {onCancel && (
          <button
            onClick={onCancel}
            className="text-slate-400 hover:text-slate-600 hover:bg-slate-100 rounded-lg w-8 h-8 flex items-center justify-center transition-colors duration-200"
          >
            <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M6 18L18 6M6 6l12 12" />
            </svg>
          </button>
        )}
      </div>

      <form onSubmit={handleSubmit} className="p-6 space-y-6">
        {/* Nom de la simulation */}
        <FormInput
          label="Nom de la Simulation"
          name="nom"
          value={formData.nom}
          onChange={handleChange}
          placeholder="Ex: Simulation Test 1"
          required
        />

        {/* Configuration Temps et Algorithme */}
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          <FormInput
            label="Durée (minutes)"
            name="dureeSimulationMinutes"
            type="number"
            value={formData.dureeSimulationMinutes}
            onChange={handleChange}
            min="60"
            max="1440"
          />
          <FormSelect
            label="Algorithme"
            name="algorithme"
            value={formData.algorithme}
            onChange={handleChange}
            options={algorithmeOptions}
          />
        </div>

        {/* Vitesse de simulation */}
        <SpeedSelector
          value={formData.facteurVitesse}
          onChange={handleSpeedChange}
        />

        {/* Ressources Hospitalières */}
        <ResourcesSection formData={formData} onChange={handleChange} />

        {/* Patients */}
        <PatientsSection formData={formData} onChange={handleChange} />

        {/* Boutons d'action */}
        <div className="flex gap-3 pt-4 border-t border-slate-200">
          {onCancel && (
            <Button
              type="button"
              onClick={onCancel}
              variant="secondary"
              size="lg"
              className="flex-1"
            >
              Annuler
            </Button>
          )}
          <Button
            type="submit"
            variant="primary"
            size="lg"
            loading={isSubmitting}
            className="flex-1"
          >
            Créer la Simulation
          </Button>
        </div>
      </form>
    </div>
  );
};

export default SimulationForm;
