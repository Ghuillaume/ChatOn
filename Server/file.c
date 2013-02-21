#include "file.h"

void initFile(File *file)
{
	file->debut = NULL;
	file->fin = NULL;
	file->taille = 0;
}

// ajouter un élément dans la file 
int push(File* file, message *msg)
{
	Node *toPush;
	if ((toPush = (Node *) malloc (sizeof (Node))) == NULL)
		return -1;
		
	toPush->msg = msg;
		
	// Si pas encore d'élément ajouté, init de la fin
	if(file->taille == 0) {
		file->fin = toPush;
		file->debut = toPush;
		toPush->suivant = NULL;
	}
	else {
		toPush->suivant = file->debut;
		file->debut = toPush;
	}
	
	file->taille++;
	
	
	printf("Pushed : %s\n", toPush->msg->message);
	return 0;
}

/* retirer_file (supprimer) un élément de la file */
message* pop(File * file)
{
	printf("Pophip\n");
	// toPop vaut null si la file est vide
	message* toPop = file->fin;
	
	// Si la file contient un seul élément, il suffit de la vider
	if(file->taille == 1) {
		file->debut = NULL;
		file->fin = NULL;
	}
	
	// Recherche du noeud précédant le dernier noeud (celui à enlever) afin de le mettre en fin de file
	Node* tmp = file->debut;
	while(tmp->suivant->suivant != NULL)  {
		tmp = tmp->suivant;
	}
	file->fin = tmp;
	
	file->taille--;
	
	return toPop;
}

void fileDebug(File* file) {

	if(file->taille < 0) {
		printf("WTF ? Taille négative\n");
	}
	else if(file->taille >= 0) {
		printf("File :\n");
		
		Node* current = file->debut;
		printf("1. %s\n", current->msg->message);
		
		int i = 2;
		while(current->suivant != NULL) {
			current = current->suivant;
			printf("%d. %s\n", i, current->msg->message);
			i++;
		}
	}
}
