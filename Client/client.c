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

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

#define TAILLE_MAX 257
#define DELIM_PV ";"

void readFromServ(void* arg) {

	int* tmp = (int*)arg;
	int socket = *tmp;
	
	char buffIn[TAILLE_MAX];
	int longueur;

	while(longueur = read(socket, buffIn, sizeof(buffIn)))
	{
		if(longueur <= 0)
		{
			perror("read");
			exit(1);
		}
		
		
		// Protocole de lecture
		
		if(strncmp(buffIn, "connected:", 10) == 0) {
			// Affichage de la liste des connectés
			printf("Liste des connectés :\n"); 
			char* pseudo_connecte = strtok(buffIn, DELIM_PV);
			while(pseudo_connecte != NULL)
			{
				fprintf(stdout, "| %s |", pseudo_connecte);
				pseudo_connecte = strtok(NULL, DELIM_PV);
			}
			printf("\n");
		}
		
		else if(strncmp(buffIn, "welcome:", 8) == 0) {
			// Affichage du message de bienvenue
		}
		
		// etc
		
		
	}
}

void writeToServ(void* arg) {

	int* tmp = (int*)arg;
	int socket = *tmp;
	
	char buffOut[TAILLE_MAX];
	
	
	memset(buffOut, '\0', TAILLE_MAX);
	printf("Entrer le texte à envoyer : ");
	fgets(buffOut, TAILLE_MAX-1, stdin);
	
	if(strncmp(buffOut, "/help", 5) == 0) {
		// Affichage de l'aide
		
	}
	else {
		write(socket,buffOut,strlen(buffOut));
		if(strncmp(buffOut, "/quit", 4) == 0)
		{
			// TODO quitter
			sleep(1);
		}
	}
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
		perror("usage : client <adresse-serveur> <pseudo> <votre ip>");
		exit(1);
	}
	
	prog = argv[0];
	host = argv[1];
	pseudo = argv[2];
	ip = argv[3];
	
	if (strlen(pseudo) > 30)
	{
		printf("La longueur du pseudo doit être inférieure à 30 caractères.");
	}
	
	int longueur_ip = strlen(ip);
	if (longueur_ip > 15 && longueur_ip < 7)
	{
		printf("L'adresse IP est malformée");
	}
	
	/* DEBUG ONLY :  affichage du message */
	/* TODO : à enlever
	strcat(msg, strcat(pseudo, ";"));
	strcat(msg, strcat(ip, ";"));
	printf("msg : %s\n", msg);
	/* END DEBUG ONLY */
	
	/* Préparation des informations de connexion */
	if ((ptr_host = gethostbyname(host)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son adresse.");
		exit(1);
	}
	
	bcopy((char*)ptr_host->h_addr, (char*) &adresse_locale.sin_addr, ptr_host->h_length);
	adresse_locale.sin_family = AF_INET;
	
	adresse_locale.sin_port = htons(5555);
	
	/* Création du socket */
	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("erreur : impossible de creer la socket de connexion avec le serveur.");
		exit(1);
	}
	
	if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de se connecter au serveur.");
		exit(1);
	}
	
	/* Envoi des informations de connexion au serveur */
	if ((write(socket_descriptor, msg, strlen(msg))) < 0) {
		perror("erreur : impossible d'ecrire le message destine au serveur.");
		exit(1);
	}
	
	printf("\n\n\t****************************************\n\t**************** ChatOn ****************\n\t****************************************\n\n\tPour avoir la liste des commandes disponibles, tapez /help à tout moment\n\n");
	
	
	/* Boucle d'écoute */
	pthread_t readLoop;
	if(pthread_create(&readLoop, NULL, readFromServ, (void*)&socket_descriptor) < 0) {
		perror("Thread problem\n");
		exit(1);
	}
	
	/* Boucle d'écriture *
	pthread_t writeLoop;
	if(pthread_create(&writeLoop, NULL, writeToServ, (void*)&socket_descriptor) < 0) {
		perror("Thread problem\n");
		exit(1);
	}/**/
	
	
	/* Fermeture du socket et extinction du programme */
	close(socket_descriptor);
	return 0;
}
