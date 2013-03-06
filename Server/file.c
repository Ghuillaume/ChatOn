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
	
	return 0;
}

Node* getFirstMessage(File* file, char* dest) {

	// On parcourt la file pour chercher le premier message qu'il faudra envoyer à dest
	Node* current = file->debut;
	Node* previous = NULL;
	
	//printf("Looking for messages for %s\n", dest);
	
	while(current != NULL) {
	
		if( strncmp(current->msg->dest, dest, strlen(dest)) == 0) {
			
			// Si on enlève le premier élément, on change le début de file
			if(previous == NULL) {
				file->debut = current->suivant;			
			}
			// Sinon, on raccorde l'élément précédent à l'élément suivant
			else {
				previous->suivant = current->suivant;
			}
			
			file->taille--;
			
			return current;
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
