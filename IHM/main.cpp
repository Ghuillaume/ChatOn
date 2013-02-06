#include <QtGui/QApplication>
#include "Window.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>

#include "../tools/common.h"

#define TAILLE_MAX 257
#define DELIM_PV ";"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

struct args {
    int socket;
    Window* window;
};
typedef struct args args;

// Reader thread code
void readFromServ(void* arg);

int copier_chaine(char *dest, const char *src, int debut, int longueur);


int main(int argc, char *argv[])
{

    int socket_descriptor, longueur; 	// descripteur de socket, longueur du buffer utilisé
    sockaddr_in adresse_locale; 			// adresse de socket local
    hostent *ptr_host; 						// info sur une machine hote
    servent *ptr_service; 					// info  sur service
    char buffer[TAILLE_MAX];
    char* prog; 								// nom du programme
    char* host;									// nom de la machine distante
    char* pseudo;
    char* ip;



    // Vérification des arguments
    /*if(argc != 4) {
        printf("usage : client <adresse-serveur> <pseudo> <votre ip>\n");
        exit(1);
    }

    prog = argv[0];
    host = argv[1];
    pseudo = argv[2];
    ip = argv[3];*/

    host = "localhost";
    pseudo = "TempLogin";
    ip = "localhost";

    if (strlen(pseudo) > 30)
    {
        printf("La longueur du pseudo doit être inférieure à 30 caractères.\n");
    }

    int longueur_ip = strlen(ip);
    if (longueur_ip > 15 && longueur_ip < 7)
    {
        printf("L'adresse IP est malformée");
    }


    /* Préparation des informations de connexion */
    if ((ptr_host = gethostbyname(host)) == NULL) {
        printf("erreur : impossible de trouver le serveur a partir de son adresse.\n");
        exit(1);
    }

    bcopy((char*)ptr_host->h_addr, (char*) &adresse_locale.sin_addr, ptr_host->h_length);
    adresse_locale.sin_family = AF_INET;

    adresse_locale.sin_port = htons(5555);

    // Création du socket
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("erreur : impossible de creer la socket de connexion avec le serveur.\n");
        exit(1);
    }

    if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
        printf("erreur : impossible de se connecter au serveur.\n");
        exit(1);
    }

    // Envoi des informations de connexion au serveur
    char msg[TAILLE_MAX] = "";
    strcat(msg, pseudo);
    strcat(msg, ";");
    strcat(msg, ip);
    std::cout << "Send to server : " << msg << endl;
    if ((write(socket_descriptor, msg, strlen(msg))) < 0) {
        printf("erreur : impossible d'ecrire le message destine au serveur.\n");
        exit(1);
    }

    QApplication a(argc, argv);

    Window w(0, socket_descriptor, pseudo);

    // Thread d'écoute du serveur
    pthread_t readLoop;
    args args;
    args.socket = socket_descriptor;
    args.window = &w;
    if(pthread_create(&readLoop, NULL, readFromServ, (void *)(&args)) < 0)
    {
        perror("pthread_create");
        exit(1);
    }


    w.show();

    return a.exec();
}


void readFromServ(void* arg) {
    printf("Ready to read\n");
    int sortie = 0;
    args* tmp = (args*)arg;
    int socket = tmp->socket;
    Window* w = tmp->window;

    w->addConnected("machin");
    w->addConnected("truc");
    w->addConnected("bidule");
    w->addConnected("chouette");
    w->removeConnected("truc");

    char buffIn[TAILLE_MAX];

    int longueur;

    while(longueur = read(socket, buffIn, sizeof(buffIn)) && !sortie)
    {
        if(longueur <= 0)
        {
            perror("read error");
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
            char pseudo[TAILLE_MAX];
            copier_chaine(pseudo, buffIn, 10, strlen(buffIn));
            w->addConnected(pseudo);
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
    }

    return;

}

int copier_chaine(char *dest, const char *src, int debut, int longueur)
{
    int i = debut;
    int j = 0;
    while ((src[i] != '\0') && (j < longueur) )
    {
        dest[j] = src[i];
        i++;
        j++;
    }

    // On retourne l'indice courant de la chaine afin de pouvoir la retraiter sans repartir du début.
    return i;
}
