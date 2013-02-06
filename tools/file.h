#include<stdlib.h>
#include<string.h>

typedef struct ElementListe{
  char *donnee;
  struct ElementListe *suivant;
} Element;

typedef struct ListeRepere{
  Element *debut;
  Element *fin;
  int taille;
} File;

/* initialisation */
void initialisation (File * suite);



/* ENFILER*/
int ajouter_file (File * suite, Element * courant, char *donnee);

/* DE_FILER*/
char* retirer_file (File * suite);

/* FirstInFirstOut */
#define file_donnee(suite) suite->debut->donnee
