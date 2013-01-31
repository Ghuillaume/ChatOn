#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>


#define TAILLE_MAX_NOM 256
#define LOCAL_PORT 5555

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;


struct connected_people
{
    char ip[15];
    char pseudo[31];
    // file de messages
    int last_communication;
};


void process_request(int sock)
{
	
}

/*---------------------------------------*/
int renvoi(int sock)
{
	char buffer[256];
	int longueur = read(sock, buffer, sizeof(buffer));
	printf("longueur du texte reçu : %d", longueur);
	
	if(longueur <= 0)
	{
		perror("read");
		return 2;
	}
	
	/* Redécoupage de la chaine à la bonne longueur et affichage du buffer */
	buffer[longueur] = '\0';
	printf("message lu : %s \n", buffer);
	
	
	buffer[0] = 'R';
	buffer[1] = 'E';
	buffer[longueur] = '#';
	buffer[longueur+1] = '\0';
	
	printf("message apres traitement : %s \n", buffer);
	
	printf("renvoi du message traite. \n");
	
	/* mise en attente du programme pour simuler un délai de transmission */
	sleep(3);

	write(sock, buffer, strlen(buffer) + 1);
	printf("message envoye.\n");
	return 0;
}

/*---------------------------------------*/

/*---------------------------------------*/
main(int argc, char** argv)
{
	int socket_descriptor, 			/* descripteur de socket */
	    nouv_socket_descriptor, 		/* [nouveau] descripteur de socket */
	    longueur_adresse_courante; 		/* longueur d'adresse courante du client */
	
	sockaddr_in adresse_locale, 		/* structure d'adresse locale */
	            adresse_client_courant;	/* adresse client courant */
	
	hostent* ptr_hote;			/* les infos récupérées sur la machine hôte */
	servent* ptr_service;			/* les infos récupérées sur le service de la machine */
	char machine[TAILLE_MAX_NOM + 1]; 	/* nom de la machine locale */
	
	 gethostname(machine, TAILLE_MAX_NOM);	/* récupération du nom de la machine */

	/* récupération de la structure d'adresse en utilisant le nom */
	if((ptr_hote = gethostbyname(machine)) == NULL)
	{
		perror("erreur : impossible de trouver le serveur a partir de son nom");
		exit(1);
	}
	
	/* initialisation de la structure adresse_locale avec le infos récupérées */
	
	/* copie de ptr_hote vers adresse_locale */
	bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
	adresse_locale.sin_family = ptr_hote->h_addrtype;	/* ou AF_INET */
	adresse_locale.sin_addr.s_addr = INADDR_ANY;		/* ou AF_INET */
	
	/* SOLUTION 2 */
	adresse_locale.sin_port = htons(LOCAL_PORT);
	
	printf("numero de port pour la connexion au serveur : %d \n", ntohs(adresse_locale.sin_port));
	
	/* création du socket */
	if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0 )) < 0)
	{
		perror("erreur : impossible de créer la socket de connexion avec le client.");
		exit(1);
	}

	/* association du socket socket_descriptor à la structure d'adresse adresse_locale */
	if((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0)
	{
		perror("erreur : impossible de lier le socket à l'adresse de connexion.");
		exit(1);
	}

	/* initialisation de la file d'écoute */
	listen(socket_descriptor, 5);

	/* attente des connexions et traitement des données reçues */
	while(1)
	{
		longueur_adresse_courante = sizeof(adresse_client_courant);
		/* adresse_client_courant sera renseignée par accept via les infos du connect */
		if((nouv_socket_descriptor = accept(socket_descriptor, (sockaddr*)(&adresse_client_courant), &longueur_adresse_courante)) < 0)
		{
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
		
		// Prise en charge de multiples connexions avec un fork
		switch(fork())
		{
			case 0:
				printf("Je suis le fils\n");
				close(socket_descriptor);
				/* traitement du message */
				printf("réception d'un message");
				process_request(nouv_socket_descriptor);
				renvoi(nouv_socket_descriptor);
				close(nouv_socket_descriptor);
				exit(1);
				break;
				
			case -1:
				printf("Erreur cosmique\n");
				exit(1);
				break;
				
			default:
				printf("Je suis ton père\n");
				close(nouv_socket_descriptor);
				break;
		}
	}
	close(socket_descriptor);
}

