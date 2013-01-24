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

int main(int argc, char** argv) {

	int socket_descriptor, longueur; 	// descripteur de socket, longueur du buffer utilisé
	sockaddr_in adresse_locale; 			// adresse de socket local
	hostent *ptr_host; 						// info sur une machine hote
	servent *ptr_service; 					// info  sur service
	char buffer[256];
	char *prog; 								// nom du programme
	char *host;									// nom de la machine distante
	char *mesg;									// message envoyé
	
	if(argc != 3) {
		perror("usage : client <adrese-serveur> <message-a-transmetre>");
		exit(1);
	}
	
	prog = argv[0];
	host = argv[1];
	mesg = argv[2];
	
	printf("nom de l'executable : %s \n", prog);
	printf("adresse du serveur : %s \n",host);
	printf("message envoye : %s \n", mesg);
	
	if ((ptr_host = gethostbyname(host)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son adresse.");
		exit(1);
	}
	
	bcopy((char*)ptr_host->h_addr, (char*) &adresse_locale.sin_addr, ptr_host->h_length);
	adresse_locale.sin_family = AF_INET;
	
	adresse_locale.sin_port = htons(5555);
	
	printf("numero de port pour la connexion au servuer : %d \n", ntohs(adresse_locale.sin_port));
	
	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("erreur : impossible de creer la socket de connexion avec le serveur.");
		exit(1);
	}
	
	if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de se connecter au serveur.");
		exit(1);
	}
	
	printf("connexion etablie avec le serveur. \n");
	printf("envoi d'un message au serveur. \n");
	
	if ((write(socket_descriptor, mesg, strlen(mesg))) < 0) {
		perror("erreur : impossible d'ecrire le message destine au serveur.");
		exit(1);
	}
	
	sleep(3);
	
	printf("message envoye au serveur. \n");
	
	while((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
		printf("reponse du serveur : \n");
		write(1,buffer,longueur);
	}
	
	printf("\nfin de la reception.\n");
	close(socket_descriptor);
	printf("connexion avec le serveur fermee, fin du programme.\n");
	exit(0);
}

	
		
