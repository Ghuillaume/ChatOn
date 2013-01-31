#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

char* remove_newline(char *s)
{
    int len = strlen(s);

    if (len > 0 && s[len-1] == '\n')  // if there's a newline
        s[len-1] = '\0';          // truncate the string

    return s;
}

int main()
{	
    char* chaine = (char*)malloc(sizeof(char)*100);
    
	fgets(chaine, sizeof(chaine), stdin);
	//chaine[sizeof(chaine)-1] = '\0';
	chaine = remove_newline(chaine);
	initscr();
	
	while (strcmp(chaine,"/quit") != 0) {
	    printw(chaine);
	    refresh();
	    fgets(chaine, sizeof(chaine), stdin);
	    chaine[sizeof(chaine)-1] = '\0';
	}
	
    //getch();			/* Wait for user input */
    printw(chaine);
    refresh();
    free(chaine);
    endwin();
	
	return 0;
}
