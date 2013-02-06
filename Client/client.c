/* -----------------------------------------------------------
Client a lancer apres le serveur avec la commande : 
client <adresse-serveur> <message-a-transmettre>
------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "../tools/common.h"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

#define TAILLE_MAX 257
#define DELIM_PV ";"

void readFromServ(void* arg) {
	int sortie = 0;
	int* tmp = (int*)arg;
	int socket = *tmp;
	
	char buffIn[TAILLE_MAX];
	
	memset(buffIn, '\0', TAILLE_MAX);
	int longueur = read(socket, buffIn, sizeof(buffIn));
	
	while(!sortie)
	{
		if(longueur <= 0)
		{
			perror("read");
			exit(1);
		}
		
		// Protocole de lecture
		printf("message:%s\n", buffIn);
		if(strncmp(buffIn, "close:", 6) == 0)
		{
			printf("Fin de connexion par le serveur, extinction.\n");
			return;
		}
		else if(strncmp(buffIn, "connected:", 10) == 0)
		{
			char liste_connectes[TAILLE_MAX];
			copier_chaine(liste_connectes, buffIn, 10, strlen(buffIn)-10);
			// Affichage de la liste des connectés
			printf("Liste des connectés :\n"); 
			char* pseudo_connecte = strtok(liste_connectes, DELIM_PV);
			while(pseudo_connecte != NULL)
			{
				fprintf(stdout, "| %s |", pseudo_connecte);
				pseudo_connecte = strtok(NULL, DELIM_PV);
			}
			printf("\n");
		}
		else if(strncmp(buffIn, "welcome:", 8) == 0)
		{
			// Affichage du message de bienvenue
		}
		else
		{
			// TODO
		}
		
		memset(buffIn, '\0', TAILLE_MAX);
		longueur = read(socket, buffIn, sizeof(buffIn));
	}
	
	return;
}

void writeToServ(void* arg) {
	int sortie = 0;
	int* tmp = (int*)arg;
	int socket = *tmp;
	
	char buffOut[TAILLE_MAX];
	
	printf("Entrer le texte à envoyer : ");
	while (!sortie)
	{
		// On flush le buffer et on récupère les entrées clavier de l'utilisateur
		memset(buffOut, '\0', TAILLE_MAX);
		fgets(buffOut, TAILLE_MAX-1, stdin);
	
		if(strncmp(buffOut, "/help", 5) == 0) {
			// TODO : Affichage de l'aide
		printf("\t/help :\tAffichage de l'aide\n\t/quit :\tQuitter le programme\n");
		}
		else {
			write(socket,buffOut,strlen(buffOut));
			if(strncmp(buffOut, "/quit", 4) == 0)
			{
				sortie = 1;
				// A la place de null, possibilité de renvoyer un void*
				//pthread_exit(NULL);
			}
		}
	}
	return;
}

int main(int argc, char** argv) {

	int socket_descriptor, longueur; 	// descripteur de socket, longueur du buffer utilisé
	sockaddr_in adresse_locale; 			// adresse de socket local
	hostent *ptr_host; 						// info sur une machine hote
	servent *ptr_service; 					// info  sur service
	char buffer[TAILLE_MAX];
	char* prog; 								// nom du programme
	char* host;									// nom de la machine distante
	char* pseudo;									
	char* ip;										
	char msg[TAILLE_MAX] = "";									
	
	/* Vérification des arguments */
	if(argc != 4) {
		printf("usage : client <adresse-serveur> <pseudo> <votre ip>\n");
		exit(1);
	}
	
	prog = argv[0];
	host = argv[1];
	pseudo = argv[2];
	ip = argv[3];
	
	if (strlen(pseudo) > 30)
	{
		printf("La longueur du pseudo doit être inférieure à 30 caractères.\n");
	}
	
	int longueur_ip = strlen(ip);
	if (longueur_ip > 15 && longueur_ip < 7)
	{
		printf("L'adresse IP est malformée");
	}
	
	/* DEBUG ONLY :  affichage du message */
	/* TODO : à enlever */
	memset(msg, '\0', TAILLE_MAX);
	strcat(msg, strcat(pseudo, ";"));
	strcat(msg, strcat(ip, ";"));
	printf("msg : %s\n", msg);
	/* END DEBUG ONLY */
	
	/* Préparation des informations de connexion */
	if ((ptr_host = gethostbyname(host)) == NULL) {
		printf("erreur : impossible de trouver le serveur a partir de son adresse.\n");
		exit(1);
	}
	
	bcopy((char*)ptr_host->h_addr, (char*) &adresse_locale.sin_addr, ptr_host->h_length);
	adresse_locale.sin_family = AF_INET;
	
	adresse_locale.sin_port = htons(5555);
	
	/* Création du socket */
	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("erreur : impossible de creer la socket de connexion avec le serveur.\n");
		exit(1);
	}
	
	if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		printf("erreur : impossible de se connecter au serveur.\n");
		exit(1);
	}
	
	/* Envoi des informations de connexion au serveur */
	if ((write(socket_descriptor, msg, strlen(msg))) < 0) {
		printf("erreur : impossible d'ecrire le message destine au serveur.\n");
		exit(1);
	}
	
	//printf("\n\n\t****************************************\n\t**************** ChatOn ****************\n\t****************************************\n\n\tPour avoir la liste des commandes disponibles, tapez /help à tout moment\n\n");
	
	
	/* Boucle d'écoute */
	pthread_t readLoop;
	if(pthread_create(&readLoop, NULL, readFromServ, (void*)&socket_descriptor) < 0)
	{
		perror("pthread_create");
		exit(1);
	}/**/
	
	/* Boucle d'écriture */
	pthread_t writeLoop;
	if(pthread_create(&writeLoop, NULL, writeToServ, (void*)&socket_descriptor) < 0)
	{
		perror("pthread_create");
		exit(1);
	}/**/
	
	// On attends que le processus d'écriture demande la fermeture avant de quitter
	int *res, *res2  = NULL;
	pthread_join(writeLoop, (void *) &res);
	pthread_join(readLoop, (void *) &res2);
	
	// Fermeture du socket et extinction du programme
	close(socket_descriptor);
	return 0;
}
