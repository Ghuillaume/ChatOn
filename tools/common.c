#include "common.h"

int copier_chaine(char *dest, const char *src, int debut, int longueur)
{	
	int i = debut;
	int j = 0;
	while ((src[i] != '\0') && (j < longueur) )
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	
	// On retourne l'indice courant de la chaine afin de pouvoir la retraiter sans repartir du début.
	return i;
}

void separer_phrase(char *partie1, char *partie2, char *src, int occurence)
{
	// Ré-initialisation des chaines avec le symbole nul
	memset(partie1, '\0', sizeof(partie1));
	memset(partie2, '\0', sizeof(partie2));
	
	// Découpage de la chaine
	char* mot = strtok(src, DELIM_ESPACE);
	int occurence_espace = 0;
	while((mot != NULL))
	{
		if(occurence_espace < occurence)
		{
			strcat(partie1, mot);
			strcat(partie1, DELIM_ESPACE);
		}
		else
		{
			strcat(partie2, mot);
			strcat(partie2, DELIM_ESPACE);
		}
		mot = strtok(NULL, DELIM_ESPACE);
		occurence_espace++;
	}
	
	return;
}
