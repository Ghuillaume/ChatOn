#include<stdlib.h>
#include<string.h>

#include "types.h"

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
//message* pop(File* file);
Node* getFirstMessage(File* file, char* dest);

void fileDebug(File* f);

/* FirstInFirstOut */
#define file_message(suite) suite->debut->msg
