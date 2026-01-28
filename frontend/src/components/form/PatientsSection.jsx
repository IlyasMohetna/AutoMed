import FormInput from "./FormInput";

const PatientsSection = ({ formData, onChange }) => {
  return (
    <div className="bg-slate-50 p-5 rounded-xl border border-slate-200">
      <h3 className="font-semibold text-slate-900 mb-4 text-sm uppercase tracking-wide">
        Configuration des Patients
      </h3>
      <div className="grid grid-cols-2 gap-4">
        <FormInput
          label="Urgences par Heure"
          name="tauxArriveeHoraireUrgences"
          type="number"
          value={formData.tauxArriveeHoraireUrgences}
          onChange={onChange}
          min="0"
          max="20"
          step="0.5"
        />
        <FormInput
          label="Patients Électifs"
          name="nombrePatientsElectifs"
          type="number"
          value={formData.nombrePatientsElectifs}
          onChange={onChange}
          min="0"
          max="100"
        />
      </div>
    </div>
  );
};

export default PatientsSection;
