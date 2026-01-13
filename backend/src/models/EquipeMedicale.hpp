#ifndef EQUIPE_MEDICALE_HPP
#define EQUIPE_MEDICALE_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace AutoMed {

/**
 * Représente un membre de l'équipe médicale
 */
struct MembreMedical {
    int id;
    std::string nom;
    std::string prenom;
    std::string specialite;

    MembreMedical(int id, const std::string& nom, const std::string& prenom, const std::string& specialite)
        : id(id), nom(nom), prenom(prenom), specialite(specialite) {}

    nlohmann::json toJson() const {
        return nlohmann::json{
            {"id", id},
            {"nom", nom},
            {"prenom", prenom},
            {"nomComplet", prenom + " " + nom},
            {"specialite", specialite}
        };
    }
};

/**
 * Représente une équipe médicale complète pour une opération
 */
class EquipeMedicale {
private:
    int id;
    std::string nom;
    MembreMedical* chirurgien;
    MembreMedical* anesthesiste;
    std::vector<MembreMedical*> infirmiers;
    bool disponible;

public:
    /**
     * Constructeur
     */
    EquipeMedicale(int id, const std::string& nom)
        : id(id), 
          nom(nom),
          chirurgien(nullptr),
          anesthesiste(nullptr),
          disponible(true) {}

    /**
     * Destructeur - Ne supprime pas les pointeurs (gestion externe)
     */
    ~EquipeMedicale() {
        // Les MembreMedical sont gérés ailleurs
        chirurgien = nullptr;
        anesthesiste = nullptr;
        infirmiers.clear();
    }

    // Getters
    int getId() const { return id; }
    std::string getNom() const { return nom; }
    MembreMedical* getChirurgien() const { return chirurgien; }
    MembreMedical* getAnesthesiste() const { return anesthesiste; }
    const std::vector<MembreMedical*>& getInfirmiers() const { return infirmiers; }
    bool estDisponible() const { return disponible; }

    // Setters
    void setNom(const std::string& newNom) { nom = newNom; }
    void setDisponible(bool dispo) { disponible = dispo; }

    /**
     * Assigne le chirurgien
     */
    void setChirurgien(MembreMedical* chir) {
        chirurgien = chir;
    }

    /**
     * Assigne l'anesthésiste
     */
    void setAnesthesiste(MembreMedical* anesth) {
        anesthesiste = anesth;
    }

    /**
     * Ajoute un infirmier à l'équipe
     */
    void ajouterInfirmier(MembreMedical* infirmier) {
        infirmiers.push_back(infirmier);
    }

    /**
     * Retire un infirmier de l'équipe
     */
    void retirerInfirmier(int infirmierId) {
        infirmiers.erase(
            std::remove_if(infirmiers.begin(), infirmiers.end(),
                [infirmierId](MembreMedical* inf) { return inf->id == infirmierId; }),
            infirmiers.end()
        );
    }

    /**
     * Vérifie si l'équipe est complète (chirurgien + anesthésiste + au moins 1 infirmier)
     */
    bool estComplete() const {
        return chirurgien != nullptr && 
               anesthesiste != nullptr && 
               !infirmiers.empty();
    }

    /**
     * Nombre total de membres
     */
    int getNombreMembres() const {
        int count = 0;
        if (chirurgien) count++;
        if (anesthesiste) count++;
        count += infirmiers.size();
        return count;
    }

    /**
     * Réserve l'équipe pour une opération
     */
    void reserver() {
        disponible = false;
    }

    /**
     * Libère l'équipe après une opération
     */
    void liberer() {
        disponible = true;
    }

    /**
     * Conversion vers JSON pour l'API
     */
    nlohmann::json toJson() const {
        nlohmann::json infirmiersJson = nlohmann::json::array();
        for (const auto& inf : infirmiers) {
            if (inf) {
                infirmiersJson.push_back(inf->toJson());
            }
        }

        return nlohmann::json{
            {"id", id},
            {"nom", nom},
            {"chirurgien", chirurgien ? chirurgien->toJson() : nullptr},
            {"anesthesiste", anesthesiste ? anesthesiste->toJson() : nullptr},
            {"infirmiers", infirmiersJson},
            {"disponible", disponible},
            {"estComplete", estComplete()},
            {"nombreMembres", getNombreMembres()}
        };
    }

    /**
     * Conversion vers string pour debug
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << "Équipe #" << id << ": " << nom
            << " | Membres: " << getNombreMembres()
            << " | Complète: " << (estComplete() ? "Oui" : "Non")
            << " | Disponible: " << (disponible ? "Oui" : "Non");
        return oss.str();
    }
};

} // namespace AutoMed

#endif // EQUIPE_MEDICALE_HPP
