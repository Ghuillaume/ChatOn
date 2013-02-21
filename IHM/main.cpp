#include <QtGui/QApplication>
#include "Window.hpp"
#include "ConnexionDialog.hpp"

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

char** split(char* chaine,const char* delim,int vide);


int main(int argc, char *argv[])
{

    int socket_descriptor, longueur; 	// descripteur de socket, longueur du buffer utilisé
    sockaddr_in adresse_locale; 			// adresse de socket local
    hostent *ptr_host; 						// info sur une machine hote
    servent *ptr_service; 					// info  sur service
    char buffer[TAILLE_MAX]; 								// nom du programme
    char host[TAILLE_MAX] = "";									// nom de la machine distante
    char pseudo[TAILLE_MAX] = "";
    char ip[TAILLE_MAX] = "";
    int port;


    QApplication a(argc, argv);
	ConnexionDialog dialog(0);
	dialog.exec();
	
	if(dialog.result() != QDialog::Accepted) {
		return 0;
    }

    QByteArray temp = dialog.serverEdit->text().toLocal8Bit();
    strcpy(host, temp.data());
    temp = dialog.pseudoEdit->text().toLocal8Bit();
    strcpy(pseudo, temp.data());
    strcpy(ip, "localhost");
    port = dialog.portEdit->text().toInt();

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
    cout << "Ready to read" << endl;
    int sortie = 0;
    args* tmp = (args*)arg;
    int socket = tmp->socket;
    Window* w = tmp->window;

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
        cout << "Receive from server : " << buffIn << endl;
        if(strncmp(buffIn, "close:", 6) == 0)
        {
            printf("Fin de connexion par le serveur, extinction.\n");
            return;
        }
        else if(strncmp(buffIn, "connected:", 10) == 0)
        {
            char** splittedBuffer = split(buffIn, ":", 0);
            w->addConnected(splittedBuffer[1]);
        }
        else if(strncmp(buffIn, "disconnected:", 13) == 0)
        {
            char pseudo[TAILLE_MAX];
            copier_chaine(pseudo, buffIn, 13, strlen(buffIn));
            w->removeConnected(pseudo);
        }
        else if(strncmp(buffIn, "welcome:", 8) == 0)
        {
            // Affichage du message de bienvenue
        }
        else if(strncmp(buffIn, "all;", 4) == 0) {

            char** splittedBuffer = split(buffIn, ";", 0);
            char* pseudo = splittedBuffer[1];
            char* msg = splittedBuffer[2];

            w->receiveText(msg, pseudo, false);
        }
        else if(strncmp(buffIn, "pv;", 3) == 0) {

            char** splittedBuffer = split(buffIn, ";", 0);
            char* pseudo = splittedBuffer[1];
            char* msg = splittedBuffer[2];

            w->receiveText(msg, pseudo, true);
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


/********************************/
/**** FROM CODESOURCE.COM *******/
/********************************/

// Retour tableau des chaines recupérer. Terminé par NULL.
// chaine : chaine à splitter
// delim : delimiteur qui sert à la decoupe
// vide : 0 : on n'accepte pas les chaines vides
// 1 : on accepte les chaines vides
char** split(char* chaine,const char* delim,int vide){
char** tab=NULL; //tableau de chaine, tableau resultat
char *ptr; //pointeur sur une partie de
int sizeStr; //taille de la chaine à recupérer
int sizeTab=0; //taille du tableau de chaine
char* largestring; //chaine à traiter
int sizeDelim=strlen(delim); //taille du delimiteur
largestring = chaine; //comme ca on ne modifie pas le pointeur d'origine
//(faut ke je verifie si c bien nécessaire)
while( (ptr=strstr(largestring, delim))!=NULL ){
sizeStr=ptr-largestring;
//si la chaine trouvé n'est pas vide ou si on accepte les chaine vide
if(vide==1 || sizeStr!=0){
//on alloue une case en plus au tableau de chaines
sizeTab++;
tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
//on alloue la chaine du tableau
tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
strncpy(tab[sizeTab-1],largestring,sizeStr);
tab[sizeTab-1][sizeStr]='\0';
}
//on decale le pointeur largestring pour continuer la boucle apres le premier elément traiter
ptr=ptr+sizeDelim;
largestring=ptr;
}
//si la chaine n'est pas vide, on recupere le dernier "morceau"
if(strlen(largestring)!=0){
sizeStr=strlen(largestring);
sizeTab++;
tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
tab[sizeTab-1]=(char*) malloc( sizeof(char)*(sizeStr+1) );
strncpy(tab[sizeTab-1],largestring,sizeStr);
tab[sizeTab-1][sizeStr]='\0';
}
else if(vide==1){ //si on fini sur un delimiteur et si on accepte les mots vides,on ajoute un mot vide
sizeTab++;
tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
tab[sizeTab-1]=(char*) malloc( sizeof(char)*1 );
tab[sizeTab-1][0]='\0';
}
//on ajoute une case à null pour finir le tableau
sizeTab++;
tab= (char**) realloc(tab,sizeof(char*)*sizeTab);
tab[sizeTab-1]=NULL;
return tab;
}
