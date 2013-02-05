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
	/* TODO : à enlever*/
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
	
	/* Récupération de la liste des personnes connectées */	
	int fin_connexion = 0;
	longueur = read(socket_descriptor, buffer, sizeof(buffer));
	if(longueur <= 0)
	{
		perror("read");
		exit(1);
	}

	/* Affichage de cette liste */
	printf("Liste des connectés :\n"); 
	char* pseudo_connecte = strtok(buffer, DELIM_PV);
	while(pseudo_connecte != NULL)
	{
		fprintf(stdout, "| %s |", pseudo_connecte);
		pseudo_connecte = strtok(NULL, DELIM_PV);
	}
	printf("\n");
	
	/* TODO : Créer deux threads : un d'envoi, un de réception */
	while(!fin_connexion) {
		/* Nettoyage du buffer et remplissage du buffer avec les entrées-clavier */
		memset(buffer, '\0', TAILLE_MAX);
		printf("Entrer le texte à envoyer : ");
		fgets(buffer, TAILLE_MAX-1, stdin);
		write(1,buffer,strlen(buffer));
		if(strncmp(buffer, "/quit", 4) == 0)
		{
			fin_connexion = 1;
			sleep(1);
		}
	}
	
	/* Fermeture du socket et extinction du programme */
	close(socket_descriptor);
	return 0;
}
