#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"

FILE* debug;

typedef struct ListNode{
  message *msg;
  struct ListNode *suivant;
} Node;

typedef struct FilePtr{
  Node *debut;
  Node *fin;
  int taille;
} File;

/* initialisation */
void initFile(File* file);

/* ENFILER*/
int push(File* file, message *msg);

/* DE_FILER*/
Node* getFirstMessage(File* file, char* dest);

void fileDebug(File* f);

/* FirstInFirstOut */
#define file_message(suite) suite->debut->msg
