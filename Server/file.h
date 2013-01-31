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
int enfiler (File * suite, Element * courant, char *donnee);

/* DE_FILER*/
char* de_filer (File * suite);

/* FirstInFirstOut */
#define file_donnee(suite) suite->debut->donnee
