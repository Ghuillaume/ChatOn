#include "file.h"

void initialisation (File *file_msg)
{
	file_msg->debut = NULL;
	file_msg->fin = NULL;
	file_msg->taille = 0;
}

/* ajouter_file (ajouter) un élément dans la file */
int ajouter_file (File * file_message, Element * courant, message *msg)
{
	Element *nouveau_element;
	if ((nouveau_element = (Element *) malloc (sizeof (Element))) == NULL)
		return -1;

	if(courant == NULL)
	{
		if(file_message->taille == 0)
			file_message->fin = nouveau_element;
		nouveau_element->suivant = file_message->debut;
		file_message->debut = nouveau_element;
	}
	else
	{
		if(courant->suivant == NULL)
			file_message->fin = nouveau_element;
		nouveau_element->suivant = courant->suivant;
		courant->suivant = nouveau_element;
	}
	
	file_message->taille++;
	return 0;
}

/* retirer_file (supprimer) un élément de la file */
message* retirer_file (File * file_message)
{
	Element *supp_element;
	if (file_message->taille == 0)
		return NULL;
	supp_element = file_message->debut;
	file_message->debut = file_message->debut->suivant;
	
	return supp_element;
}
