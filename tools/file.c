#include "file.h"

void initialisation (File * suite){
  suite->debut = NULL;
  suite->fin = NULL;
  suite->taille = 0;
}

/* ajouter_file (ajouter) un élément dans la file */
int ajouter_file (File * suite, Element * courant, char *donnee){
  Element *nouveau_element;
  if ((nouveau_element = (Element *) malloc (sizeof (Element))) == NULL)
    return -1;
  if ((nouveau_element->donnee = (char *) malloc (50 * sizeof (char)))
      == NULL)
    return -1;
  strcpy (nouveau_element->donnee, donnee);

  if(courant == NULL){
    if(suite->taille == 0)
      suite->fin = nouveau_element;
    nouveau_element->suivant = suite->debut;
    suite->debut = nouveau_element;
  }else {
    if(courant->suivant == NULL)
      suite->fin = nouveau_element;
    nouveau_element->suivant = courant->suivant;
    courant->suivant = nouveau_element;
  }
  suite->taille++;
  return 0;
}

/* retirer_file (supprimer) un élément de la file */
char* retirer_file (File * suite){
	Element *supp_element;
	if (suite->taille == 0)
	return "\0\0";
	supp_element = suite->debut;
	suite->debut = suite->debut->suivant;
	return supp_element->donnee;
	/*free (supp_element);
	suite->taille--;
	return 0;
*/}
