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
	toPush->suivant = NULL;
		
	// Si pas encore d'élément ajouté, init de la fin
	if(file->taille == 0) {
		file->debut = toPush;
	}
	else {
		file->fin->suivant = toPush;		
	}
	
	file->fin = toPush;
	file->taille++;
	
	
	printf("Pushed : %s\n", toPush->msg->message);
	return 0;
}

/* retirer_file (supprimer) un élément de la file 
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
}*/

Node* getFirstMessage(File* file, char* dest) {

	// On parcourt la file pour chercher le premier message qu'il faudra envoyer à dest
	Node* current = file->debut;
	Node* previous = NULL;
	
	printf("Looking for messages for %s\n", dest);
	
	while(current != NULL) {
	
		
		printf("%s (%d) == %s (%d) ?? ", current->msg->dest, strlen(current->msg->dest), dest, strlen(dest));
	
		if( strcmp(current->msg->dest, "all") == 0 || strcmp(current->msg->dest, dest) == 0) {
			printf("OK\n");
			// Si on enlève le premier élément, on change le début de file
			if(previous == NULL) {
				file->debut = current->suivant;			
			}
			// Sinon, on raccorde l'élément précédent à l'élément suivant
			else {
				previous->suivant = current->suivant;
			}
			
			return current;
		}
		else {
			printf("No\n");
		}
		
		current = current->suivant;
	}

	return NULL;
}

void fileDebug(File* file) {

	if(file->taille < 0) {
		printf("WTF ? Taille négative\n");
	}
	else if(file->taille >= 0) {
		printf("File :\n");
		
		Node* current = file->debut;
		
		int i = 1;
		while(current != NULL) {
			printf("%d. %s(from %s to %s)\n", i, current->msg->message, current->msg->source, current->msg->dest);
			current = current->suivant;
			i++;
		}
	}
}
