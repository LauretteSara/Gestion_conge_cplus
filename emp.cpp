// on importe les biblothèques qui vont etre utilisé pour notre application
#include <iostream> // pour les entrées/sorties
#include <vector> //pour les tableaux, les matrices, ...
#include <string> // pour les chaines de caracteres
#include <ctime> // pour avoir l'année actuelle 
#include <algorithm>  // Inclusion de l'en-tête algorithm pour utiliser la fonction remove_it 
using namespace std;


// on commence par creer une classe employes qui va gerer les employes 
class Employes
{
    private:
        string Nom; 
        string Prenom;
        string date_naissance; // il doit etre sur le format suivant : jj/mm/aaaa
        int age; // pour verifier si nos calculs d'age sont corrects
        int degre_invalidite;
        int vacances_prises; 

        // fonction qui permet de calculer l'age d'un employe à patir du 01/01 de l'annee en cours
        void calculAge() {
            // Obtenir l'année actuelle
            time_t t = time(0);
            tm* now = localtime(&t);
            int anneeActuelle = now->tm_year + 1900;

            // Extraire l'année de naissance à partir de la date de naissance
            size_t pos = date_naissance.find_last_of('/');
            if (pos != string::npos && pos < date_naissance.length() - 1) {
                int anneeNaissance = stoi(date_naissance.substr(pos + 1));
                
                // Calculer et retourner l'âge
                age = anneeActuelle - anneeNaissance;
            } 
            else {
                // Gérer le cas où le format de date est incorrect
                cerr << "Erreur : Format de date incorrect." << endl;
                age = -1; // Valeur spéciale pour indiquer une erreur
            }
        }

    

    public:
        // on definit le constructeur de la classe 
        Employes () {
            //  Demander à l'utilisateur d'entrer les informations 
            cout << "Entrer le nom : "; 
            cin >> Nom; 

            cout << "Entrer le prenom : "; 
            cin >> Prenom;

            cout << "Entrer la date de naissance suivant ce format '(jj/mm/aaaa)' : ";
            cin >> date_naissance;

            cout << "Entrer le degre d'invalidité : ";
            cin >> degre_invalidite;

            calculAge();
            // initialiser les vacances_prises en fontion des conditions spécifiées 
            if (age >= 50){
                vacances_prises = 32; // ajouter 2 jours pour les emp de plus de 50 ans
            }
            else {
                vacances_prises = 30; // par defaut, 30 jours pour les autres employés 
            }
            if (degre_invalidite >= 50){
                vacances_prises += 5; // ajouter 5 jours aux degre d'invalidite d'au moins 50%
            }
        }

        // methode pour afficher les details d'un employé 
        void afficherDetails() const {
            cout << "   Noms : " << Nom  << ", \nPrenoms : " << Prenom
                    << ", \nDate de naissance : " << date_naissance
                    << ", \nAge: " << age <<" ans"
                    << ", \nDegre d'invalidité : " << degre_invalidite << " %"
                    << ", \nJours de vacances restants : " << vacances_prises << endl;
        }

        // methode pour calculer les jours de vacances restant pour un employé
        void calcul_vacances_restants (int days) {
            if (days <= vacances_prises) {
                vacances_prises -= days;
                cout << days << "jours de vacances pris. Jours de vacances restants : " << vacances_prises << endl;
            }
            else {
                cout <<"Pas assez de jours de vacances disponibles." <<endl;
            }
        }

        // Accesseur pour obtenir le nom de l'employé
        string getNom() const {
            return Nom;
        }

        string getPrenom() const {
            return Prenom;
        }

        // Ajoutez une méthode pour obtenir l'âge si vous en avez besoin à l'extérieur de la classe
        int getAge() const {
            return age;
        }

        int getVacances_prises() const {
            return vacances_prises;
        }

        int getdegre_invalidite() const {
            return degre_invalidite;
        }

};

// ici nous creons une classe vacances qui va se charger des gerer les congés en fonction des informations de tous les employes enregistrés dans le système
class GestionVacances {
    private:
        vector <Employes> employes;
        static const int maxEmployes = 500; // pour definir la taille maximale du vecteur d'employé 

    public: 
        // methode pour ajouter un nouvel employé 
        void addEmploye () {
            // on verifie si on peut encore ajouter un employé dans notre vecteur 
            if (employes.size() < maxEmployes) {
                employes.push_back(Employes()); // si c'est le cas on ajoute le nouvel employé à la fin
                cout << "Employé ajouté avec succes." << endl;
            }
            else {
                // sinon on renvoit un message  
                cout << "Limite d'employé atteinte. On ne peut plus en rajouter." <<endl;
            }
        }

        // Méthode pour supprimer un employé par son nom
        void deleteEmploye(const string & nom) {
            auto it = remove_if(employes.begin(), employes.end(), [&](const Employes & employe) {
                return employe.getNom() == nom;
            });

            employes.erase(it, employes.end());

            cout << "Employé avec le nom : " << nom << " supprimé avec succès." << endl;
        }

        // methode pour afficher tous les employés présent dans le systeme 
        void displayAllEmploye () const {
            for (const auto & employe: employes) {
                employe.afficherDetails();
            }
        }

        // methode pour rechercher un employe par son nom et afficher ses informations 
        void findEmploye (const string & Nom) const {
            for (const auto & employe : employes) {
                if (employe.getNom() == Nom) {
                    employe.afficherDetails();
                }
            }
            cout << "Aucun employé trouvé avec le Nom :" << Nom << endl;
        }

        // Méthode pour saisir les jours de vacances pris par un employé
        void prendre_vacances(const string& nomEmploye, int days) {
            // Chercher l'employé par son nom
            auto it = find_if(employes.begin(), employes.end(),
                            [nomEmploye](const Employes& employe) {
                                return employe.getNom() == nomEmploye;
                            });

            if (it != employes.end()) {
                // Si l'employé est trouvé, appeler la méthode prendre_vacances de la classe Employes
                it->calcul_vacances_restants(days);
            } else {
                cout << "Aucun employé trouvé avec le nom : " << nomEmploye << endl;
            }
        }
};

/*
int main() {
    GestionVacances gestionVacances;

    gestionVacances.addEmploye(Employes("Doe", "John", "08/06/1970", 25));
    gestionVacances.addEmploye(Employes("Smith", "Jane", "05/12/1995", 60));

    cout << "Liste de tous les employés : " << endl;
    gestionVacances.displayAllEmploye();

    cout << "\nRecherche d'un employé par nom : " << endl;
    gestionVacances.findEmploye("Doe");

    cout << "\nSuppression d'un employé par nom : " << endl;
    gestionVacances.deleteEmploye("Doe");

    cout << "\nListe mise à jour de tous les employés : " << endl;
    gestionVacances.displayAllEmploye();

    return 0;
}
*/

int main() {
    GestionVacances gestionVacances;

    int choix;

    do {
        // Afficher le menu
        cout << "\n                 ****** Menu ******:             \n" << endl;
        cout << "       1. Ajouter un employé \n" << endl;
        cout << "       2. Afficher tous les employés \n" << endl;
        cout << "       3. Rechercher un employé par nom \n" << endl;
        cout << "       4. Supprimer un employé par nom \n" << endl;
        cout << "       0. Quitter" << endl;

        // Demander le choix de l'utilisateur
        cout << "Entrez votre choix (0-4) : ";
        cin >> choix;

        // Traiter le choix de l'utilisateur
        switch (choix) {
            case 1:
                // Ajouter un employé
                do {
                    gestionVacances.addEmploye();
                    char reponse;
                    // entrer o/O pour oui et ajouter un autre employé et n pour non et retourner au menu
                    cout << "Voulez-vous ajouter un autre employé ? (o/n) : ";
                    cin >> reponse;
                    if (reponse != 'o' && reponse != 'O') {
                        break;
                    }
                } while (true);
                break;

            case 2:
                // Afficher tous les employés
                cout << "Liste de tous les employés : " << endl;
                gestionVacances.displayAllEmploye();
                break;

            case 3:
                // Rechercher un employé par nom
                do {
                    string nomRecherche;
                    cout << "Entrez le nom de l'employé à rechercher : ";
                    cin >> nomRecherche;
                    gestionVacances.findEmploye(nomRecherche);

                    // gerer la prise de congé pour l'employé recherhe à l'instant
                    char res; // pour stocke la repons de l'utilisateur pour la prise de congé 
                    cout << "Voulez-vous prendre des jours de congés pour cet employé ? (o/n) :" ;
                    cin >> res;
                    
                    if (res == 'o' || res == 'O') {
                        int jours;
                        cout << "Combien de jours de congé souhaitez-vous prendre ? ";
                        cin >> jours;
                        gestionVacances.prendre_vacances(nomRecherche, jours);
                    }

                    char reponse;
                    // entrer o/O pour oui et rechercher un autre employé et n pour non et retourner au menu
                    cout << "Voulez-vous rechercher un autre employé ? (o/n) : ";
                    cin >> reponse;
                    if (reponse != 'o' && reponse != 'O') {
                        break;
                    }
                } while (true);
                break;
            
            case 4:
                // Supprimer un employé par nom
                do {
                    string nomRecherche;
                    cout << "Entrez le nom de l'employé à supprimer : ";
                    cin >> nomRecherche;
                    gestionVacances.deleteEmploye(nomRecherche);

                    char reponse;
                    // entrer o/O pour oui et supprimer un autre employé et n pour non et retourner au menu
                    cout << "Voulez-vous supprimer un autre employé ? (o/n) : ";
                    cin >> reponse;
                    if (reponse != 'o' && reponse != 'O') {
                        break;
                    }
                } while (true);
                break;

            case 0:
                // Quitter le programme
                cout << "Au revoir !" << endl;
                break;

            default:
                cout << "Choix invalide. Veuillez entrer un nombre entre 0 et 3." << endl;
                break;
        }
    } while (choix != 0);

    return 0;
}



