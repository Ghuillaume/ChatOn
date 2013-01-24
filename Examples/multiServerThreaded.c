#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#define TAILLE_MAX_NOM 256


typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;


/*---------------------------------------*/
void renvoi(void *arg) {
	int* tmp = (int*)arg;
	int nsd = *tmp;
	
	char buffer[256];
	int longueur;
	if((longueur = read(nsd, buffer, sizeof(buffer))) <= 0) {
		return;
	}
	
	printf("message lu : %s \n", buffer);
	buffer[0] = 'R';
	buffer[1] = 'E';
	buffer[longueur] = '#';
	buffer[longueur+1] = '\0';
	
	printf("message apres traitement : %s \n", buffer);
	
	printf("renvoi du message traite. \n");
	
	/* mise en attente du programme pour simuler un délai de transmission */
	sleep(3);

	write(nsd, buffer, strlen(buffer) + 1);
	printf("message envoye.\n");
	
	close(nsd);
	
	return;
}

/*---------------------------------------*/

/*---------------------------------------*/
main(int argc, char** argv) {
	int socket_descriptor, 			/* descripteur de socket */
	    nouv_socket_descriptor, 		/* [nouveau] descripteur de socket */
	    longueur_adresse_courante; 		/* longueur d'adresse courante du client */
	
	sockaddr_in adresse_locale, 		/* structure d'adresse locale */
	            adresse_client_courant;	/* adresse client courant */
	            
	pthread_t clientHandler;
	
	hostent* ptr_hote;			/* les infos récupérées sur la machine hôte */
	servent* ptr_service;			/* les infos récupérées sur le service de la machine */
	char machine[TAILLE_MAX_NOM + 1]; 	/* nom de la machine locale */
	
	gethostname(machine, TAILLE_MAX_NOM);	/* récupération du nom de la machine */

	/* récupération de la structure d'adresse en utilisant le nom */
	if((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son nom");
		exit(1);
	}
	
	/* initialisation de la structure adresse_locale avec le infos récupérées */
	
	/* copie de ptr_hote vers adresse_locale */
	bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
	adresse_locale.sin_family = ptr_hote->h_addrtype;	/* ou AF_INET */
	adresse_locale.sin_addr.s_addr = INADDR_ANY;		/* ou AF_INET */
	
	/* SOLUTION 2 */
	adresse_locale.sin_port = htons(5555);
	
	printf("numero de port pour la connexio au serveur : %d \n", ntohs(adresse_locale.sin_port));
	
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
		
		if(pthread_create(&clientHandler, NULL, renvoi, (void*)&nouv_socket_descriptor) < 0) {
			perror("Thread problem\n");
			exit(1);
		}
		
	}
	close(socket_descriptor);
}

