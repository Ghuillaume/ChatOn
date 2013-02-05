#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#include "file.h"
#include "types.h"
#include "../tools/common.h"

#define	h_addr h_addr_list[0]
#define TAILLE_MAX 256
#define NB_SLOTS_SERVEUR 5

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

/* Variables globales */
int slots_serveurs_restants = NB_SLOTS_SERVEUR;
utilisateur** liste_connectes;


/***********************************************************/
/* Code du thread correspondant au traitement d'un client  */
/***********************************************************/
void traiter_requete(void *arg) {
	int* tmp = (int*)arg;
	int sock = *tmp;

	// Réception de la requête
	char buffer[TAILLE_MAX];
	int longueur = read(sock, buffer, sizeof(buffer));
	printf("longueur du texte reçu : %d\n", longueur);
	
	if(longueur < 0)
	{
		perror("read");
		return;
	}
	
	buffer[longueur] = '\0';
	
	// Décrémente le nombre de slots disponibles sur le serveur
	if (slots_serveurs_restants == 0)
	{
		printf("Pas d'espace disponible pour les nouveaux utilisateurs.\n");
		return;
	}
	else
	{
		slots_serveurs_restants--;
	}
	
	// Contruit dynamiquement une structure de données utilisateur 
	utilisateur *nouvel_utilisateur = malloc(sizeof(utilisateur));
	if(nouvel_utilisateur == NULL)
	{
		perror("malloc");
		return;
	}
	nouvel_utilisateur->pseudo[LONGUEUR_MAX_PSEUDO] = '\0';
	nouvel_utilisateur->ip[LONGUEUR_MAX_IP] = '\0';

	// On remplit les différents champs
	int indice = copier_donnees(nouvel_utilisateur->pseudo, buffer, 0, LONGUEUR_MAX_PSEUDO);
	copier_donnees(nouvel_utilisateur->ip, buffer, indice + 1, LONGUEUR_MAX_IP);
	struct timeval tv;
	gettimeofday(&tv,NULL);
	nouvel_utilisateur->dernier_contact = tv.tv_sec;
	
	// Alimente le tableau de gens connectés
	int ajoute = 0;
	int i = 0;
	while (!ajoute && i < NB_SLOTS_SERVEUR)
	{
		if(liste_connectes[i] == NULL)
		{
			liste_connectes[i] = nouvel_utilisateur;
			ajoute = 1;
		}
		else
		{
			i++;
		}
	}
	
	// Envoie la liste des pseudos des gens connectés
	char pseudos_connectes[156] = "connected:";
	for(i = 0; i < NB_SLOTS_SERVEUR; i++)
	{
		if (liste_connectes[i] != NULL)
		{
			char pseudo_courant[LONGUEUR_MAX_PSEUDO+1] = "";
			strcpy(pseudo_courant, liste_connectes[i]->pseudo);
			strcat(pseudos_connectes, strcat(pseudo_courant, ";"));
		}
	}
	
	printf("pseudos connectés : %s\n", pseudos_connectes);
	memset(buffer, '\0', TAILLE_MAX);
	strcpy(buffer, pseudos_connectes);
	write(sock, buffer, strlen(buffer));
	
	memset(buffer, '\0', TAILLE_MAX);
	longueur = read(sock, buffer, sizeof(buffer));
	
	// TODO : Ecouter en boucle les requêtes du client et les traiter
	int sortie = 0; 
	while (!sortie)
	{
		if(longueur <= 0)
		{
			perror("read");
			return;
		}

		// On remplace le dernier caractère \n par \0 pour simplifier l'affichage
		if(strlen(buffer) > 1)
		{
			buffer[strlen(buffer)-1] = '\0';		
		}
		
		printf("%s : \"%s\"\n", nouvel_utilisateur->pseudo, buffer);
		
		if (strncmp(buffer, "/quit", 4) == 0)
		{
			sortie = 1;
		}
		else if (strncmp(buffer, "/msg", 4) == 0)
		{
			// TODO
			printf("msg\n");
		}
		else if (strncmp(buffer, "/all", 4) == 0)
		{
			// TODO
			printf("all\n");
		}
		else
		{
			printf("Commande non reconnue.\n");
		}
		
		memset(buffer, '\0', TAILLE_MAX);
		if(!sortie)
		{
			longueur = read(sock, buffer, sizeof(buffer));
		}
	}
	
	// Ferme le socket, libère le slot et termine le thread
	printf("fin connexion\n");
	free(nouvel_utilisateur);
	close(sock);
	slots_serveurs_restants++;
	return;
}

/*---------------------------------------*/

/*---------------------------------------*/
int main(int argc, char** argv) {
	
	File *suite;
	char *nom;
	if ((suite = (File *) malloc (sizeof (File))) == NULL)
		return -1;
	if ((nom = (char *) malloc (50 * sizeof (char))) == NULL)
		return -1;
	initialisation (suite);
	
	liste_connectes = malloc(sizeof(utilisateur*) * NB_SLOTS_SERVEUR);
	if(liste_connectes == NULL)
	{
		perror("malloc");
		exit(1);
	}
	
	int socket_descriptor, 			/* descripteur de socket */
	    nouv_socket_descriptor, 		/* [nouveau] descripteur de socket */
	    longueur_adresse_courante; 		/* longueur d'adresse courante du client */
	
	sockaddr_in adresse_locale, 		/* structure d'adresse locale */
	            adresse_client_courant;	/* adresse client courant */
	            
	pthread_t clientHandler;
	
	hostent* ptr_hote;			/* les infos récupérées sur la machine hôte */
	servent* ptr_service;			/* les infos récupérées sur le service de la machine */
	char machine[TAILLE_MAX + 1]; 	/* nom de la machine locale */
	
	gethostname(machine, TAILLE_MAX);	/* récupération du nom de la machine */

	/* récupération de la structure d'adresse en utilisant le nom */
	if((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur à partir de son nom");
		exit(1);
	}
	
	/* initialisation de la structure adresse_locale avec le infos récupérées */
	
	/* copie de ptr_hote vers adresse_locale */
	bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
	adresse_locale.sin_family = ptr_hote->h_addrtype;	/* ou AF_INET */
	adresse_locale.sin_addr.s_addr = INADDR_ANY;		/* ou AF_INET */
	
	/* SOLUTION 2 */
	adresse_locale.sin_port = htons(5555);
	
	printf("Écoute sur le port %d \n", ntohs(adresse_locale.sin_port));
	
	/* création du socket */
	if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0 )) < 0) {
		perror("erreur : impossible de creer la socket de connexion avec le client.");
		exit(1);
	}

	/* association du socket socket_descriptor à la structure d'adresse adresse_locale */
	if((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de lier le socket a l'adresse de connexion.");
		exit(1);
	}

	/* initialisation de la file d'écoute */
	listen(socket_descriptor, 5);

	/* attente des connexions et traitement des données reçues */
	while(1) {
		longueur_adresse_courante = sizeof(adresse_client_courant);
		/* adresse_client_courant sera renseignée par accept via les infos du connect */
		if((nouv_socket_descriptor = accept(socket_descriptor, (sockaddr*)(&adresse_client_courant), &longueur_adresse_courante)) < 0) {
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
		
		if(pthread_create(&clientHandler, NULL, traiter_requete, (void*)&nouv_socket_descriptor) < 0) {
			perror("Thread problem\n");
			exit(1);
		}
	}
	close(socket_descriptor);
	free(liste_connectes);
	
	return EXIT_SUCCESS;
}
