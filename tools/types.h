#define LONGUEUR_MAX_PSEUDO 30
#define LONGUEUR_MAX_IP 15
#define LONGUEUR_MAX_MSG 500
#define DELIM_ESPACE " "

typedef struct utilisateur {
    char pseudo[LONGUEUR_MAX_PSEUDO + 1];
    char ip[LONGUEUR_MAX_IP + 1];
    // minute du dernier échange avec le serveur
    int dernier_contact;
} utilisateur;

typedef struct message {
	char source[LONGUEUR_MAX_PSEUDO + 1];
	char dest[LONGUEUR_MAX_PSEUDO + 1];
	char message[LONGUEUR_MAX_MSG + 1];
} message;

typedef struct clientThreadArgs {
    int socket;
    utilisateur* user;
} clientThreadArgs;
