#include<stdlib.h>
#include<string.h>

#include "types.h"

typedef struct ElementListe{
  message *msg;
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
int ajouter_file (File * suite, Element * courant, message *msg);

/* DE_FILER*/
message* retirer_file (File * suite);

/* FirstInFirstOut */
#define file_message(suite) suite->debut->msg
