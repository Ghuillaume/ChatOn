#include "file.h"

void initFile(File *file)
{
	debug = fopen("file_debug.txt", "w");
	fprintf(debug, "Initializing queue\n");
	fclose(debug);
	
	file->debut = NULL;
	file->fin = NULL;
	file->taille = 0;
}

// ajouter un élément dans la file 
int push(File* file, message *msg)
{
	debug = fopen("file_debug.txt", "a+");
	
	Node *toPush;
	if ((toPush = (Node *) malloc (sizeof (Node))) == NULL) {
		fprintf(debug, "Malloc error !\n");
		fclose(debug);
		return -1;
	}
		
	toPush->msg = msg;
	toPush->suivant = NULL;
		
	// Si pas encore d'élément ajouté, init du début
	if(file->taille == 0) {
		file->debut = toPush;
	}
	else {
		file->fin->suivant = toPush;		
	}
	
	file->fin = toPush;
	file->taille++;
	
	fprintf(debug, "Pushed in queue : %s\n", toPush->msg->message);
	fclose(debug);
	
	fileDebug(file);
		
	return 0;
}

Node* getFirstMessage(File* file, char* dest) {

	debug = fopen("file_debug.txt", "a+");

	// On parcourt la file pour chercher le premier message qu'il faudra envoyer à dest
	Node* current = file->debut;
	Node* previous = NULL;
	
	//fprintf(debug, "Looking for messages for %s (len: %d) :\n", dest, strlen(dest));
	
	while(current != NULL) {
	
		if( strncmp(current->msg->dest, dest, strlen(dest)) == 0) {
			fprintf(debug, "begin pop\n");
			
			// Si on enlève le premier élément, on change le début de file
			if(previous == NULL) {
				file->debut = current->suivant;			
			}
			// Sinon, on raccorde l'élément précédent à l'élément suivant
			else {
				previous->suivant = current->suivant;
			}
			
			file->taille--;
			
			fprintf(debug, "Found a message for %s, return %s.\n", dest, current->msg->message);
			fprintf(debug, "end pop\n");
			fclose(debug);
			
			fileDebug(file);
			
			return current;
		}
		
		previous = current;
		current = current->suivant;
	}

	fclose(debug);
	return NULL;
}

void fileDebug(File* file) {
	debug = fopen("file_debug.txt", "a+");

	if(file->taille < 0) {
		fprintf(debug, "\t\tFile de taile négative\n");
	}
	else if(file->taille >= 0) {
		fprintf(debug, "\t\tFile (%d) :\n", file->taille);
		
		Node* current = file->debut;
		
		int i = 1;
		while(current != NULL) {
			fprintf(debug, "\t\t\t%d. %s(from %s to %s)\n", i, current->msg->message, current->msg->source, current->msg->dest);
			current = current->suivant;
			i++;
		}
	}
	
	fclose(debug);
}
