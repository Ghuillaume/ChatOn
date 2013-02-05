int copier_donnees(char *dest, const char *src, int debut, int longueur)
{	
	int i = debut;
	int j = 0;
	while ( (src[i] != ';') && (src[i] != '\0') && (j < longueur) )
	{
		dest[j] = src[i];
		i++;
		j++;
	}
	
	/* On s'arrête au prochain symbole ";" dans la chaine source */
	for(; src[i] != ';'; i++);
	dest[j] = '\0';
	
	/*
	On retourne l'indice courant de la chaine afin de pouvoir la retraiter
	sans repartir du début.
	*/
	return i;
}
