#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <strings.h>

#include "file.h"
#include "types.h"
#include "common.h"

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
utilisateur* utilisateur_par_defaut;
File *file_message;
pthread_mutex_t mutex_file = PTHREAD_MUTEX_INITIALIZER;
//TODO : mutex pour la liste des utilisateurs et le nombre de slots restants sur le serveur

// Threads de communication avec le client
void protocoleReception(void* arg);
void protocoleEnvoi(void* arg);


utilisateur* initConnection(int socket);


int main(int argc, char** argv)
{
	if ((file_message = (File *) malloc (sizeof (File))) == NULL)
		return -1;
	
	// On initialise l'utilisateur destinataire qui servira pour le "/all"
	if ((utilisateur_par_defaut = (utilisateur *) malloc (sizeof (utilisateur))) == NULL)
		return -1;	
	
	strcpy(utilisateur_par_defaut->pseudo, "all");
		
	
	// Appel à la fonction d'initialisation du serveur	
	initFile(file_message);
	
	// Création de la liste des connectés sur le serveur et initialisation à NULL
	liste_connectes = malloc(sizeof(utilisateur*) * NB_SLOTS_SERVEUR);
	if(liste_connectes == NULL)
	{
		perror("malloc");
		exit(1);
	}
	for (int i = 0; i < NB_SLOTS_SERVEUR; i++)
	{
		liste_connectes[i] = NULL;
	}
	
	int socket_descriptor, 			/* descripteur de socket */
	    nouv_socket_descriptor, 		/* [nouveau] descripteur de socket */
	    longueur_adresse_courante; 		/* longueur d'adresse courante du client */
	
	sockaddr_in adresse_locale, 		/* structure d'adresse locale */
	            adresse_client_courant;	/* adresse client courant */
	            
	pthread_t clientHandler;
	pthread_t clientHandler2;
	
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
		if((nouv_socket_descriptor = accept(socket_descriptor, (sockaddr_in*)(&adresse_client_courant), &longueur_adresse_courante)) < 0) {
			perror("erreur : impossible d'accepter la connexion avec le client.");
			exit(1);
		}
				
		clientThreadArgs args;
		args.socket = nouv_socket_descriptor;
		args.user = initConnection(nouv_socket_descriptor);
		args.linked_thread = clientHandler2;
		if(args.user == NULL) {
			exit(1);
		}
		
		if (pthread_create(&clientHandler, NULL, protocoleReception, (void*)&args) < 0)
		{
			perror("Receiving thread problem\n");
			exit(1);
		}
		
		args.linked_thread = clientHandler;
		if (pthread_create(&clientHandler2, NULL, protocoleEnvoi, (void*)&args) < 0)
		{
			perror("Sending thread problem\n");
			exit(1);
		} 

	}
	close(socket_descriptor);
	free(liste_connectes);
	
	return EXIT_SUCCESS;
}


utilisateur* initConnection(int socket)
{

	char buffer[TAILLE_MAX];
	
	// Attente des informations de connexion du client (pseudo, ip)
	int longueur;
	longueur = read(socket, buffer, sizeof(buffer));
	
	printf("\t<<<<< Receiving connection information from client : '%s'\n", buffer);
	
	// Décrémente le nombre de slots disponibles sur le serveur
	if (slots_serveurs_restants == 0)
	{
		printf("Pas d'espace disponible pour les nouveaux utilisateurs.\n");
		return NULL;
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
		return NULL;
	}
	//nouvel_utilisateur->pseudo[strlen(nouvel_utilisateur->pseudo)] = '\0';
	//nouvel_utilisateur->ip[strlen(nouvel_utilisateur->ip)] = '\0';
	memset(nouvel_utilisateur->pseudo, '\0', sizeof(nouvel_utilisateur->pseudo));
	memset(nouvel_utilisateur->ip, '\0', sizeof(nouvel_utilisateur->ip));

	// On remplit les différents champs
	int indice = copier_chaine(nouvel_utilisateur->pseudo, buffer, 0, LONGUEUR_MAX_PSEUDO);
	copier_chaine(nouvel_utilisateur->ip, buffer, indice + 1, LONGUEUR_MAX_IP);
	struct timeval tv;
	gettimeofday(&tv,NULL);
	nouvel_utilisateur->dernier_contact = tv.tv_sec;
	
	// Alimente le tableau de gens connectés
	int ajoute = 0;
	int indice_tab_connectes = 0;
	while (!ajoute && indice_tab_connectes < NB_SLOTS_SERVEUR)
	{
		if(liste_connectes[indice_tab_connectes] == NULL)
		{
			liste_connectes[indice_tab_connectes] = nouvel_utilisateur;
			ajoute = 1;
		}
		else
		{
			indice_tab_connectes++;
		}
	}
	
	
	// Envoie la liste des pseudos des gens connectés au nouvel utilisateur
	memset(buffer, '\0', TAILLE_MAX);
	int i;
	strcpy(buffer,"here:");
	for(i = 0; i < NB_SLOTS_SERVEUR; i++)
	{
		if (liste_connectes[i] != NULL && liste_connectes[i]->pseudo != nouvel_utilisateur->pseudo)
		{
			strcat(buffer, liste_connectes[i]->pseudo);
			strcat(buffer, ";");
		}
	}
	buffer[strlen(buffer)] = '\0';
	if(strlen(buffer) > 6) {
		printf("\t>>>>> Sending to %s : '%s'\n", nouvel_utilisateur->pseudo, buffer);
		write(socket, buffer, strlen(buffer));
	}
	memset(buffer, '\0', TAILLE_MAX);
	
	
	// Notification de connexion aux utilisateurs
	// Pour chaque utilisateur connecté, on ajoute un message à la file
	message** msg = malloc(sizeof(message*));
	i = 0;
	while (i < NB_SLOTS_SERVEUR)
	{
		if(liste_connectes[i] != NULL) {
			
			// On ajoute pour tout le monde sauf celui qui a envoyé le message
			if(strcmp(nouvel_utilisateur->pseudo, liste_connectes[i]->pseudo) != 0) {
				msg[i] = malloc(sizeof(message));
				if (msg[i] == NULL)
				{
					perror("malloc error");
					return; // ignorer le message
				}
	
				// Découpage de la phrase et assemblage du message
				strcpy(msg[i]->source, nouvel_utilisateur->pseudo);
				strcpy(msg[i]->dest, liste_connectes[i]->pseudo);
				strcpy(msg[i]->message, "/notifyConnection");
				msg[i]->forAll = 1;
	
				// On verrouille la file de messsage globale, on ajoute le message et on déverrouille
	   			pthread_mutex_lock(&mutex_file);
				push(file_message, msg[i]);
	   			pthread_mutex_unlock(&mutex_file);
	   		}
   		}
   		i++;
	}
	
	printf("Client initialized !!\n");
	
	return nouvel_utilisateur;
	
}


void protocoleReception(void* arg)
{

    clientThreadArgs* tmp = (clientThreadArgs*)arg;
    int socket = tmp->socket;
    utilisateur* currentUser = tmp->user;
	
	char buffer[TAILLE_MAX];
	int longueur;
	
	
	printf("Reading thread %s initialized\n", currentUser);
	
	int clientConnected = 1;
	while(longueur = read(socket, buffer, sizeof(buffer)) && clientConnected)
	{

		if(longueur <= 0)
		{
			perror("read");
			return;
		}
	
		printf("\t<<<<< Received from %s : %s\n", currentUser->pseudo, buffer);
	
		// On traite les messages en fonction des mots-clés (/quit, /msg, ...)
		if (strncmp(buffer, "/quit", 4) == 0)
		{
			printf("%s disconnected\n", currentUser->pseudo);
			clientConnected = 0;
			
			// Notification de déconnexion aux utilisateurs
			// Pour chaque utilisateur connecté, on ajoute un message à la file
			message** msg = malloc(sizeof(message*));
			int i = 0;
			while (i < NB_SLOTS_SERVEUR)
			{
				if(liste_connectes[i] != NULL) {
			
					// On ajoute pour tout le monde sauf celui qui a envoyé le message
					if(strcmp(currentUser->pseudo, liste_connectes[i]->pseudo) != 0)
					{
						printf("%s supprimé !\n", currentUser->pseudo);
						msg[i] = malloc(sizeof(message));
						if (msg[i] == NULL)
						{
							perror("malloc error");
							return; // ignorer le message
						}
	
						// Découpage de la phrase et assemblage du message
						strcpy(msg[i]->source, currentUser->pseudo);
						strcpy(msg[i]->dest, liste_connectes[i]->pseudo);
						strcpy(msg[i]->message, "/notifyDisconnection");
						msg[i]->forAll = 1;
	
						printf("test\n");
						
						// On verrouille la file de messsage globale, on ajoute le message et on déverrouille
			   			pthread_mutex_lock(&mutex_file);
						push(file_message, msg[i]);
			   			pthread_mutex_unlock(&mutex_file);
			   			
						printf("test2\n");
						// On supprime l'utilisateur pour libérer la place
			   			liste_connectes[i] = NULL;
						currentUser = NULL;
						clientConnected = 0;
						pthread_cancel(tmp->linked_thread);
						
						printf("test3\n");
			   		}
		   		}
		   		i++;
			}
		}
		else if (strncmp(buffer, "/msg", 4) == 0)
		{
			// On découpe les différentes parties de la chaine (commande, destinataire, message)
			char commande[11];
			char reste_commande[TAILLE_MAX];
			char chaine_message[TAILLE_MAX];
			separer_phrase(commande, reste_commande, buffer, 1);
			char destinataire[LONGUEUR_MAX_PSEUDO];
			separer_phrase(destinataire, chaine_message, reste_commande, 1);
		
			// On crée la structure message qui contient toutes les informations
			message *msg = malloc(sizeof(message));
			if (msg == NULL)
			{
				perror("malloc error");
				return; // ignorer le message
			}
		
			// On crée le message et on le remplit
			strcpy(msg->source, currentUser->pseudo);
			strcpy(msg->message, chaine_message);
			strcpy(msg->dest, destinataire);
			msg->forAll = 0;
			
			
			// On verrouille la file de messsage gloable, on ajoute un message et on déverrouille
   			pthread_mutex_lock(&mutex_file);
			push(file_message, msg);
   			pthread_mutex_unlock(&mutex_file);
	   		
		}
		else if (strncmp(buffer, "/all", 4) == 0)
		{
			char commande[TAILLE_MAX];
			char chaine_message[TAILLE_MAX];
			
			separer_phrase(commande, chaine_message, buffer, 1);
			
			// Pour chaque utilisateur connecté, on ajoute un message à la file
			message** msg = malloc(sizeof(message*));
			int i = 0;
			while (i < NB_SLOTS_SERVEUR)
			{
				if(liste_connectes[i] != NULL) {
					
					// On ajoute pour tout le monde sauf celui qui a envoyé le message
					if(strcmp(currentUser->pseudo, liste_connectes[i]->pseudo) != 0) {
						msg[i] = malloc(sizeof(message));
						if (msg[i] == NULL)
						{
							perror("malloc error");
							return; // ignorer le message
						}
			
						// Découpage de la phrase et assemblage du message
						strcpy(msg[i]->source, currentUser->pseudo);
						strcpy(msg[i]->dest, liste_connectes[i]->pseudo);
						strcpy(msg[i]->message, chaine_message);
						msg[i]->forAll = 1;
			
						// On verrouille la file de messsage gloable, on ajoute le message et on déverrouille
			   			pthread_mutex_lock(&mutex_file);
						push(file_message, msg[i]);
			   			pthread_mutex_unlock(&mutex_file);
			   		}
		   		}
		   		i++;
	   		}
		}
		else
		{
			printf("Commande non reconnue.\n");
		}
	
	   		
	   	//fileDebug(file_message);
	   	
		memset(buffer, '\0', TAILLE_MAX);
	}
	
	// Ferme le socket, libère le slot et termine le thread
	/*
	int i;
	for(i = 0; i < NB_SLOTS_SERVEUR; i++)
	{
		if (liste_connectes[i]->pseudo == currentUser->pseudo)
		{
			liste_connectes[i] = NULL;
		}
	}
	free(currentUser);
	*/
	close(socket);
	slots_serveurs_restants++;
	return;
}

// Thread d'envoi des messages de la file
void protocoleEnvoi(void* arg)
{
    clientThreadArgs* tmp = (clientThreadArgs*)arg;
    int socket = tmp->socket;
    utilisateur* utilisateur_courant = tmp->user;
    
	printf("Sending thread %s initialized\n", utilisateur_courant->pseudo);
	
	// Tant que l'utilisateur est connecté, on consulte la file de message
	
	while (utilisateur_courant != NULL)
	{
	
		pthread_mutex_lock(&mutex_file);
		Node* toSend = getFirstMessage(file_message, utilisateur_courant->pseudo);
		pthread_mutex_unlock(&mutex_file);
		
		if(toSend != NULL) {
			
   			char message_complet[600] = "";
			memset(message_complet, '\0', sizeof(message_complet));
			
			if(strcmp(toSend->msg->message, "/notifyConnection") == 0) {
				strcpy(message_complet, "connected:");
				strcat(message_complet, toSend->msg->source);
				strcat(message_complet, ";");
			}
			
			else if(strcmp(toSend->msg->message, "/notifyDisconnection") == 0) {
				strcpy(message_complet, "disconnected:");
				strcat(message_complet, toSend->msg->source);
				strcat(message_complet, ";");
			}
   			
			// Private message "pv;pseudo_source;message"
			else if (toSend->msg->forAll == 0)
   			{
   				strcpy(message_complet, "pv;");
   				strcat(message_complet, toSend->msg->source);
   				strcat(message_complet, ";");
   				strcat(message_complet, toSend->msg->message);
   			}
   			
   			// Public message "all;pseudo_source;message"
   			else if (toSend->msg->forAll == 1)
   			{
   				strcpy(message_complet, "all;");
   				strcat(message_complet, toSend->msg->source);
   				strcat(message_complet, ";");
   				strcat(message_complet, toSend->msg->message);
   			}
			
   			printf("\t>>>>> Sending to %s : '%s'\n", utilisateur_courant->pseudo, message_complet);
			write(socket, message_complet, strlen(message_complet));
			
			// Libération mémoire
			free(toSend);
			
		}
	
		sleep(1);
	}
	printf("FIN DE LA CONNEXION\n");
}

int pseudo_already_used(char pseudo_temp[LONGUEUR_MAX_PSEUDO+1])
{
	int exists = 0;
	// Pour tous les gens connectés, le pseudo a-t-il déjà été utilisé ?
	for (int i = 0; i < NB_SLOTS_SERVEUR; i++)
	{
		if (liste_connectes[i] != NULL)
		{
			exists |= (!strcmp(liste_connectes[i]->pseudo, pseudo_temp));
		}
	}
	return exists;
}
