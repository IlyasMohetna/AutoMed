import { useState } from "react";
import { Save, X } from "lucide-react";
import { DialogHeader, DialogTitle, DialogDescription, DialogFooter } from "@/components/ui/dialog";
import { Button } from "@/components/ui/button";
import { Label } from "@/components/ui/label";
import FormInput from "./form/FormInput";
import FormSelect from "./form/FormSelect";
import SpeedSelector from "./form/SpeedSelector";
import ResourcesSection from "./form/ResourcesSection";
import PatientsSection from "./form/PatientsSection";

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
    <>
      <DialogHeader>
        <DialogTitle>Nouvelle Simulation</DialogTitle>
        <DialogDescription>
          Configurez les paramètres de votre simulation de bloc opératoire
        </DialogDescription>
      </DialogHeader>

      <form onSubmit={handleSubmit} className="space-y-6 py-4">
        <FormInput
          label="Nom de la Simulation"
          name="nom"
          value={formData.nom}
          onChange={handleChange}
          placeholder="Ex: Simulation Test 1"
          required
        />

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

        <SpeedSelector
          value={formData.facteurVitesse}
          onChange={handleSpeedChange}
        />

        <ResourcesSection formData={formData} onChange={handleChange} />

        <PatientsSection formData={formData} onChange={handleChange} />

        <DialogFooter className="gap-2">
          <Button
            type="button"
            onClick={onCancel}
            variant="outline"
          >
            Annuler
          </Button>
          <Button
            type="submit"
            disabled={isSubmitting}
          >
            <Save className="h-4 w-4 mr-2" />
            Créer la Simulation
          </Button>
        </DialogFooter>
      </form>
    </>
  );
};

export default SimulationForm;
