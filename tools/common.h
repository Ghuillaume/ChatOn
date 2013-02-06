/* Fonctions communément appelées par les clients / serveurs */
#define DELIM_ESPACE " "

// Copier une sous-chaine de caractères
int copier_chaine(char *dest, const char *src, int debut, int longueur);

// Séparer une phrase en 2 parties au Nième symbole espace
void separer_phrase(char *partie1, char *partie2, char *src, int occurence);
