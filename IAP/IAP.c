#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// Messages emis par les instructions -----------------------------------------
#define MSG_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CLIENT "le client %s a commande : "
#define MSG_CHARGE "charge de travail pour %s : "
#define MSG_SUPERVISION "etat des taches pour %s : "
#define MSG_INTERRUPTION "## fin de programme\n" //message renvoyé lorsque l'utilisateur intterompt

// Lexemes --------------------------------------------------------------------
#define LGMOT 35
#define NBCHIFFREMAX 5
#define MAX_SPECIALITES 10
#define MAX_TRAVAILLEURS 50
#define MAX_CLIENTS 100
typedef char Mot[LGMOT + 1];

void get_id(Mot id) {
	scanf("%s", id);
	if (EchoActif) printf(">>echo %s\n", id);
}
int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) printf(">>echo %s\n", buffer);
	return atoi(buffer);
}

// Donnees --------------------------------------------------------------------

//Specialites ----------------------------------------------------------

typedef struct {
	Mot nom;
	int cout_horaire;
}Specialite;

typedef struct {
	Specialite specialites[MAX_SPECIALITES];
	int nb_specialites;
}Specialites; //stocker ls valeurs des différentes spécialités entrées par l'utilisateur

// Travailleurs --------------------------------------------------
typedef struct {
	Mot nom;
	unsigned int heures_travailleurs;
	Booleen tags_competences[MAX_SPECIALITES];
} Travailleur;

typedef struct {
	Travailleur travailleurs[MAX_TRAVAILLEURS];
	int nb_travailleurs;
} Travailleurs; //stocker les différents noms des travailleurs entrés par l'utilisateur

// Client ------------------------------------------------------
typedef struct {
	Mot tab_clients[MAX_CLIENTS];
	int nb_clients;
}Clients;

// Commandes --------------------------------------------------

#define MAX_COMMANDES 500
typedef struct {
	Mot nom;
	unsigned int idx_travailleur;
	unsigned int nb_heures_requises;
	unsigned int nb_heures_effectuees;
} Tache;

typedef struct {
	Mot nom;// la commande a un nom
	unsigned int idx_client;//indice auquel se trouve le client dans le tableau tab_clients de cette commande
	Tache taches_par_specialite[MAX_SPECIALITES]; //  nb_heures_requise s==0 <=> pas de tache pour cette specialite
	int nb_taches;
} Commande;

typedef struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned int nb_commandes;
	unsigned int nb_commandes_effectuees;
} Commandes;

void traite_facturation(Commande* comm, Specialites* spe);
void traite_facturation_globale(Commandes* comms, Specialites* spe, Clients* cli);


// Instructions ---------------------------------------------------------------
// developpe ---------------------------
void traite_developpe(Specialites* spe) {
	Mot nom_specialite;
	get_id(nom_specialite);
	strcpy(spe->specialites[spe->nb_specialites].nom, nom_specialite);
	int cout_horaire = get_int();
	spe->specialites[spe->nb_specialites].cout_horaire = cout_horaire;
	spe->nb_specialites++;
}

void afficher_specialites(Specialites* spe) {
	int i;
	printf("specialites traitees :");
	for (i = 0; i < spe->nb_specialites; i++) {
		printf(" %s/%d", spe->specialites[i].nom, spe->specialites[i].cout_horaire);
		if (i < spe->nb_specialites - 1)
			printf(",");
	}
	printf("\n");
}

void traite_embauche(Specialites* spec, Travailleurs* trav) {
	Mot nom_travailleur;
	Mot nom_specialite;
	// obtient le nom du travailleur
	get_id(nom_travailleur);
	//obtient la spécialité du travailleur
	get_id(nom_specialite);
	int position_travailleurs = 0;
	while (position_travailleurs < trav->nb_travailleurs && strcmp(nom_travailleur, trav->travailleurs[position_travailleurs].nom) != 0) {
		position_travailleurs++; //sur ce while, on récupère la position du travailleur ayant le nom : nom_travailleur
	}
	int indice = 0;
	while (indice < spec->nb_specialites && strcmp(nom_specialite, spec->specialites[indice].nom) != 0) {
		indice++;//sur ce while, on récupère la position de la spécialité ayant le nom : nom_specialite
	}
	trav->travailleurs[position_travailleurs].tags_competences[indice] = VRAI;
	if (position_travailleurs == trav->nb_travailleurs)
	{
		strcpy(trav->travailleurs[trav->nb_travailleurs].nom, nom_travailleur);
		trav->travailleurs[position_travailleurs].heures_travailleurs = 0;
		trav->nb_travailleurs++;
	}
}

void afficher_travailleurs(Travailleurs* trav, Specialites* spe) {
	Mot specialites;
	get_id(specialites);
	int i;
	Booleen condition_virgule = FAUX;

	for (i = 0; i < spe->nb_specialites; ++i) {
		int k;
		if (strcmp(specialites, "tous") == 0) {
			printf(MSG_TRAVAILLEURS, spe->specialites[i].nom);

			for (k = 0; k < trav->nb_travailleurs; ++k) {

				if (trav->travailleurs[k].tags_competences[i] == VRAI) {

					if (condition_virgule == VRAI && trav->nb_travailleurs > 0) {
						printf(", ");
					}
					printf("%s", trav->travailleurs[k].nom);
					condition_virgule = VRAI;
				}

			}
			printf("\n");
			condition_virgule = FAUX;
		}
		else if (strcmp(spe->specialites[i].nom, specialites) == 0) {
			printf(MSG_TRAVAILLEURS, spe->specialites[i].nom);
			int j;
			for (j = 0; j < trav->nb_travailleurs; j++) {
				if (trav->travailleurs[j].tags_competences[i] == VRAI) {
					if (condition_virgule == VRAI && trav->nb_travailleurs > 0) {
						printf(", ");
					}
					printf("%s", trav->travailleurs[j].nom);
					condition_virgule = VRAI;
				}

			}
			printf("\n");
		}


	}
	condition_virgule = FAUX;

}


void traite_demarche(Clients* cli) {
	Mot nom_client;
	get_id(nom_client);
	strcpy(cli->tab_clients[cli->nb_clients], nom_client);
	cli->nb_clients++;

}
void afficher_client(Commandes* comm, Clients* cli) {
	Mot nom_client;
	get_id(nom_client);
	unsigned int nb_commandes = 0;
	if (strcmp(nom_client, "tous") == 0) {
		int i = 0;
		for (i = 0; i < cli->nb_clients; i++) {
			printf(MSG_CLIENT, cli->tab_clients[i]);
			unsigned int compt;
			for (compt = 0; compt < comm->nb_commandes; ++compt) {
				if (i == comm->tab_commandes[compt].idx_client) {
					if (nb_commandes > 0) {
						printf(", ");
					}
					printf("%s", comm->tab_commandes[compt].nom);
					nb_commandes++;
				}

			}
			nb_commandes = 0;
			printf("\n");
		}
	}

	else {
		unsigned int compt;
		unsigned int indice_client = 0, nb_commandes = 0;
		while (indice_client <= cli->nb_clients && strcmp(nom_client, cli->tab_clients[indice_client]) != 0) {
			indice_client++; //sur ce while, on récupère la position du travailleur ayant le nom : nom_client
		}
		if (indice_client < cli->nb_clients) {
			printf(MSG_CLIENT, nom_client);
			for (compt = 0; compt < comm->nb_commandes; ++compt) {
				if (indice_client == comm->tab_commandes[compt].idx_client) {
					if (nb_commandes > 0) {
						printf(", ");
					}
					printf("%s", comm->tab_commandes[compt].nom);
					nb_commandes++;
				}
			}
			printf("\n");
		}
	}
}


// commande--------------------
void traite_commande(Commandes* comm, Clients* cli) {
	Mot nom_commande;
	Mot nom_client;
	// obtient le nom de la commande
	get_id(nom_commande);
	//obtient la spécialité du client
	get_id(nom_client);
	strcpy(comm->tab_commandes[comm->nb_commandes].nom, nom_commande);//récupérer le nom de la commande
	comm->tab_commandes[comm->nb_commandes].nb_taches = 0;
	int indice;
	for (indice = 0; indice < MAX_SPECIALITES; indice++) {
		comm->tab_commandes[comm->nb_commandes].taches_par_specialite[indice].nb_heures_effectuees = 0;//initialisation à 0 de toutes les valeurs(nb_heures_effectuees) présentes dans le tableau de commandes
		comm->tab_commandes[comm->nb_commandes].taches_par_specialite[indice].nb_heures_requises = 0;//initialisation à 0 de toutes les valeurs(nb_heures_requises) présentes dans le tableau de commandes
	}
	indice = 0;
	while (indice < cli->nb_clients) {
		if (strcmp(cli->tab_clients[indice], nom_client) == 0) {

			comm->tab_commandes[comm->nb_commandes].idx_client = indice;
			break;
		}
		indice++;
	}

	comm->nb_commandes++;
}

// supervision------------------------
void traite_supervision(Specialites* specialites, Commandes* comm) {

	int i;
	int nb_taches = 0;
	for (i = 0; i < comm->nb_commandes; ++i) {
		printf(MSG_SUPERVISION, comm->tab_commandes[i].nom);
		int compt;
		for (compt = 0; compt < specialites->nb_specialites; compt++) {
			if (comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_effectuees == 0
				&& comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_requises != 0) {
				if (nb_taches > 0) {
					printf(", ");
				}
				printf("%s:0/%d", specialites->specialites[compt].nom,
					comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_requises);
				nb_taches++;
			}
			else if (comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_effectuees != 0
				&& comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_requises != 0) {
				if (nb_taches > 0) {
					printf(", ");
				}
				printf("%s:%d/%d", specialites->specialites[compt].nom,
					comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_effectuees,
					comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_requises);
				nb_taches++;
			}
		}
		nb_taches = 0;
		printf("\n");
	}
}



// tache--------------------
//void nombre_de_taches(Commandes* comm, Specialites* spe, Travailleurs* trav){ //ici on initialise une fonction qui permet de compter le nombre de taches afin de pouvoir ensuite assimilier une nouvelle tache au client. 
//	unsigned int compt, nombre_de_taches = 0, nombre_d_heures;
//	for (compt = 0; compt < comm->nb_commandes; compt++) {
//
//}

void traite_tache(Commandes* comm, Specialites* spe, Travailleurs* trav) {
	Mot nom_commande;
	get_id(nom_commande);
	Mot nom_specialite;
	get_id(nom_specialite);
	unsigned int nb_heures_requises = get_int();
	int i;
	for (i = 0; i < comm->nb_commandes; ++i) {
		if (strcmp(nom_commande, comm->tab_commandes[i].nom) == 0) {//indice de la commande
			break;
		}
	}
	int indice = 0;
	while (indice < MAX_SPECIALITES && strcmp(spe->specialites[indice].nom, nom_specialite) != 0) {
		indice++;
	}
	unsigned int minimum_travail = 0;
	int compteur;
	for (compteur = 0; compteur < trav->nb_travailleurs; compteur++) {
		if (trav->travailleurs[compteur].tags_competences[indice] == VRAI) {
			if (compteur == 0) {
				minimum_travail = trav->travailleurs[compteur].heures_travailleurs;
			}
			else if (trav->travailleurs[compteur].heures_travailleurs < minimum_travail) {
				minimum_travail = trav->travailleurs[compteur].heures_travailleurs;
			}
		}
	}
	int j = 0;
	while (trav->travailleurs[j].heures_travailleurs != minimum_travail) {
		j++;
	}
	strcpy(comm->tab_commandes[i].taches_par_specialite[indice].nom, nom_specialite);
	comm->tab_commandes[i].taches_par_specialite[indice].nb_heures_effectuees = 0;
	comm->tab_commandes[i].taches_par_specialite[indice].nb_heures_requises = nb_heures_requises;
	comm->tab_commandes[i].taches_par_specialite[indice].idx_travailleur = j;
	comm->tab_commandes[i].nb_taches++;
	trav->travailleurs[j].heures_travailleurs += nb_heures_requises;

}

void traite_progression(Commandes* comm, Specialites* spe, Clients* cli) {
	Mot nom_commande;
	get_id(nom_commande);
	Mot nom_specialite;
	get_id(nom_specialite);
	Booleen travail_fini = FAUX;
	unsigned int nb_heures_effectuees = get_int();
	int compt, i = 0;
	for (compt = 0; compt < comm->nb_commandes; compt++) {
		if (strcmp(nom_commande, comm->tab_commandes[compt].nom) == 0) {//indice de la commande
			break;
		}
	}
	int indice = 0;
	while (indice < MAX_SPECIALITES && strcmp(spe->specialites[indice].nom, nom_specialite) != 0) {
		indice++;
	}
	if (comm->tab_commandes[compt].taches_par_specialite[indice].nb_heures_effectuees != 0) {
		comm->tab_commandes[compt].taches_par_specialite[indice].nb_heures_effectuees += nb_heures_effectuees;
	}
	else {
		comm->tab_commandes[compt].taches_par_specialite[indice].nb_heures_effectuees = nb_heures_effectuees;
	}

	int toute_taches_finies = 1, taches_finies;
	for (taches_finies = 0; taches_finies < comm->tab_commandes[compt].nb_taches; taches_finies++) {
		if (comm->tab_commandes[compt].taches_par_specialite[taches_finies].nb_heures_effectuees <
			comm->tab_commandes[compt].taches_par_specialite[taches_finies].nb_heures_requises) {
			toute_taches_finies = 0;
		}
	}
	if (toute_taches_finies == 1) {
		travail_fini = VRAI;

	}
	int b = 0;
	for (int spec = 0; spec < spe->nb_specialites; spec++) {
		if (comm->tab_commandes[compt].taches_par_specialite[spec].nb_heures_effectuees -
			comm->tab_commandes[compt].taches_par_specialite[spec].nb_heures_requises == 0 &&
			comm->tab_commandes[compt].taches_par_specialite[spec].nb_heures_requises != 0) {
			b++;

		}
	}
	if (travail_fini == VRAI && b == comm->tab_commandes[compt].nb_taches) {
		traite_facturation(&comm->tab_commandes[compt], spe, cli);
		comm->nb_commandes_effectuees++;
	}
	if (comm->nb_commandes_effectuees == comm->nb_commandes) {
		traite_facturation_globale(comm, spe, cli);
	}

}

//charge--------------------
void traite_charge(Travailleurs* trav, Specialites* spe, Commandes* comm) {
	Mot prenom_travailleur;
	get_id(prenom_travailleur);
	unsigned int compteur, nombre_heures_restantes = 0;//besoin d'un premier compteur pour notre premier for qui va parcourir le nombre de travailleurs pour ainsi récupérer le nom. Ensuite on initialisé un nombre d'heures qui permet d'afficher le nombre d'heures restantes pour compléter la tache dans charge. Enfin un compteur pour les tâches qui permet de les compter.
	unsigned int cout_produit = 0;
	for (compteur = 0; compteur < trav->nb_travailleurs; compteur++) {
		if (strcmp(prenom_travailleur, trav->travailleurs[compteur].nom) == 0) {
			printf(MSG_CHARGE, trav->travailleurs[compteur].nom);
			break;
		}
	}
	unsigned int i, compt, nb_tache = 0;
	for (i = 0; i < comm->nb_commandes; i++) {
		for (compt = 0; compt < spe->nb_specialites; compt++) {
			nombre_heures_restantes = comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_requises
				- comm->tab_commandes[i].taches_par_specialite[compt].nb_heures_effectuees;
			if (comm->tab_commandes[i].taches_par_specialite[compt].idx_travailleur == compteur && nombre_heures_restantes != 0) {

				if (nb_tache > 0) {
					printf(", ");
				}
				printf("%s/%s/%dheure(s)", comm->tab_commandes[i].nom, spe->specialites[compt].nom,
					nombre_heures_restantes);
				nb_tache++;
			}
		}

	}
	printf("\n");
}

//fonction qui renvoit 1 si la commande est terminée,0 sinon
int commande_completee(Commande* comm) {
	int i = 0;
	for (i = 0; i < MAX_SPECIALITES; ++i) {
		if (comm->taches_par_specialite[i].nb_heures_effectuees < comm->taches_par_specialite[i].nb_heures_requises) {
			return -1;
		}
	}
	return 1;
}


void traite_facturation(Commande* comm, Specialites* spe) {
	int cout_produit, chaque_commande, cpt_specialite;
	Booleen tache_indice;
	tache_indice = FAUX;
	if (commande_completee(comm) == 1) {
		printf("facturation %s : ", comm->nom);
		for (cpt_specialite = 0; cpt_specialite < MAX_SPECIALITES; cpt_specialite++) {
			if (comm->taches_par_specialite[cpt_specialite].nb_heures_effectuees != 0) {
				cout_produit = spe->specialites[cpt_specialite].cout_horaire *
					comm->taches_par_specialite[cpt_specialite].nb_heures_requises;
				if (tache_indice == VRAI) {
					printf(", ");
				}
				printf("%s:%d", spe->specialites[cpt_specialite].nom, cout_produit);
				tache_indice = VRAI;
			}
		}

		printf("\n");
	}
}

// Facturation globale client -----------------------

void traite_facturation_globale(Commandes* comms, Specialites* spe, Clients* cli) {


	printf("facturations : ");
	int cpt_clients, facturation_par_personne = 0, compteur, j;

	for (cpt_clients = 0; cpt_clients < cli->nb_clients; cpt_clients++) {

		for (compteur = 0; compteur < comms->nb_commandes; compteur++) {

			if (cpt_clients == comms->tab_commandes[compteur].idx_client) {
				for (j = 0; j < spe->nb_specialites; j++)

					facturation_par_personne += (comms->tab_commandes[compteur].taches_par_specialite[j].nb_heures_requises * spe->specialites[j].cout_horaire);

			}

		}

		if (cpt_clients > 0) {
			printf(", ");


		}
		printf("%s:%d", cli->tab_clients[cpt_clients], facturation_par_personne);
		facturation_par_personne = 0;
	}
	printf("\n");
	exit(0);
}


// interruption ------------------------
void traite_interruption() {
	//printf(MSG_INTERRUPTION);
}

//Boucle principale ---------------------------------------------------------- 
int main(int argc, char* argv[]) {
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	Specialites specialites;
	specialites.nb_specialites = 0;
	Travailleurs travailleurs;
	travailleurs.nb_travailleurs = 0;
	Clients clients;
	clients.nb_clients = 0;
	Commandes commandes;
	commandes.nb_commandes = 0;
	commandes.nb_commandes_effectuees = 0;
	while (VRAI) {
		get_id(buffer);
		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe(&specialites);
			continue;
		}
		if (strcmp(buffer, "specialites") == 0) {
			afficher_specialites(&specialites);
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche(&specialites, &travailleurs);
			continue;
		}
		if (strcmp(buffer, "travailleurs") == 0) {
			afficher_travailleurs(&travailleurs, &specialites);
			continue;
		}
		if (strcmp(buffer, "demarche") == 0) {
			traite_demarche(&clients);
			continue;
		}
		if (strcmp(buffer, "client") == 0) {
			afficher_client(&commandes, &clients);
			continue;
		}

		if (strcmp(buffer, "commande") == 0) {
			traite_commande(&commandes, &clients);
			continue;
		}
		if (strcmp(buffer, "supervision") == 0) {
			traite_supervision(&specialites, &commandes);
			continue;
		}
		if (strcmp(buffer, "tache") == 0) {
			traite_tache(&commandes, &specialites, &travailleurs);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traite_progression(&commandes, &specialites, &clients);
			continue;
		}
		if (strcmp(buffer, "passe") == 0) {
			continue;
		}
		if (strcmp(buffer, "charge") == 0) {
			traite_charge(&travailleurs, &specialites, &commandes);
			continue;
		}
		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}
		printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}