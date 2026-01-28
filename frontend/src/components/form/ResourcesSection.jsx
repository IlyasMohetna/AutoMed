import FormInput from "./FormInput";

const ResourcesSection = ({ formData, onChange }) => {
  return (
    <div className="bg-slate-50 p-5 rounded-xl border border-slate-200">
      <h3 className="font-semibold text-slate-900 mb-4 text-sm uppercase tracking-wide">
        Ressources Hospitalières
      </h3>
      <div className="grid grid-cols-2 gap-4">
        <FormInput
          label="Blocs Opératoires"
          name="nombreBlocs"
          type="number"
          value={formData.nombreBlocs}
          onChange={onChange}
          min="1"
          max="20"
        />
        <FormInput
          label="Équipes Médicales"
          name="nombreEquipes"
          type="number"
          value={formData.nombreEquipes}
          onChange={onChange}
          min="1"
          max="20"
        />
        <FormInput
          label="Salle d'Attente"
          name="capaciteSalleAttente"
          type="number"
          value={formData.capaciteSalleAttente}
          onChange={onChange}
          min="10"
          max="200"
        />
        <FormInput
          label="Salle de Réveil"
          name="capaciteSalleReveil"
          type="number"
          value={formData.capaciteSalleReveil}
          onChange={onChange}
          min="5"
          max="100"
        />
      </div>
    </div>
  );
};

export default ResourcesSection;
