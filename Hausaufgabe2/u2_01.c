#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct stud_type_ {                  /* Struktur des Datensatzes:    */
    int matnum;                              /* Matrikelnummer, Vor-         */
    char vorname[20];                        /* und Nachname sind Eintraege. */
    char nachname[20];                       /* Die Datenbank ist eine       */
    struct stud_type_ *next_student;         /* einfach verkettete Liste     */
} stud_type;

typedef struct sort_type_ {
	stud_type *content;
	struct sort_type_ *next_sort;
} sort_type;

/* Ist die Datenbank leer?      */
int is_empty(stud_type** liste) {
    return *liste == NULL;
}


/* Einfuegen eines Elementes    
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt MatNr, Vorname und Nachname des Studenten der EingefŸgt werden soll
 *
 */
void enqueue(stud_type** studenten_liste, int matnum, char vorname[20], char nachname[20])
{
    /* Deklariere benötigte Variablen */
    stud_type *curr, *prev;

    /* Hol Speicher auf dem Heap an für den neuen Listen Eintrag */
	stud_type *zeiger = (stud_type*) malloc(sizeof(stud_type));
	
	if(zeiger == NULL)
    {
    	printf("%s\n", strerror(errno));
    	return;
    }     
    
    if(zeiger == NULL)
    {
    	printf("%s\n", strerror(errno));
    	return;
    }
    
    /* Befüll den Speicher */
     	//Anlegen des neuen Eintrags
	zeiger->matnum = matnum;
	strcpy(zeiger->vorname, vorname);
	strcpy(zeiger->nachname, nachname);	
	zeiger->next_student = NULL;

    /* Füg den neuen Eintrag in die Liste ein */
    /* Ist die Liste leer ? */
    /* Sortier den Studenten aufsteigend nach Matrikelnummer ein */
    curr = *studenten_liste;
    prev = NULL;

	while((curr != NULL) && (curr->matnum < matnum))
	{
		prev = curr;
		curr = curr->next_student;
	}

	if(curr == NULL)
	{
		if(prev == NULL)
		{
			*studenten_liste = zeiger;
		}
		else
		{
			prev->next_student = zeiger;	
		}
	}
	else
	{
		if(prev == NULL)
		{
			zeiger->next_student = curr;
			*studenten_liste = zeiger;
		}
		else
		{
			zeiger->next_student = curr;
			prev->next_student = zeiger;
		}
	}
}

/* Lšschen eines Elementes. 
 * 
 * Bekommt einen Zeiger auf einen Zeiger der auf das 1. Element der Liste zeigt
 * Bekommt die MatNr des Studenten der zu lšschen ist
 *
 * Gibt 0 fŸr einen Fehler zurŸck
 * Gibt 1 fŸr erfolg zurŸck
 */
int dequeue(stud_type** studenten_liste, int matnum)
{
    /* Deklariere benštigte Variablen */
    stud_type *curr, *prev;
	curr = *studenten_liste;

    /* PrŸfe Randbedingungen */
    
    /* Finde den Studenten */
    	while((curr != NULL) && (curr->matnum != matnum))
	{
		prev = curr;
		curr = curr->next_student;
	}

    /* Lšsche den Studenten und gibt den Speicher frei */
    /* Was muss passieren wenn das 1. Element gelšscht wird? */
    /* Was ist wenn es nicht in der Liste ist? */
    /* ... */
	if(curr == NULL)
	{
		return 0;
	}
	else
	{
		if(prev == NULL)
		{
			*studenten_liste = curr->next_student;
		}
		else
		{
			prev->next_student = curr->next_student;
		}
		free(curr);
	}

	return 1;
    
}

/* Auslesen eines Elementes 
 *
 * Bekommt zeiger auf den Listenstart
 * Bekommt matnr des Studenten der ausgelesen werden soll
 *
 * Schreibt Vorname und Nachname in vorname und nachname
 */
int get_student(stud_type* studenten_liste, int matnum, char vorname[20], char nachname[20])
{
    stud_type *curr;

    /* Durchmustern der DB */
    curr = studenten_liste;                  
    while ((curr != NULL) && (curr->matnum < matnum))
        curr = curr->next_student;
    if ((curr == NULL) || (curr->matnum != matnum))
        /* RŸckgabewert: Fehler */
        return 0;                             
   else {
       strcpy(vorname, curr->vorname);
       strcpy(nachname, curr->nachname);
        /* RŸckgabewert: OK */
       return 1;                             
   }
}

/**
Sortiert
*/
sort_type** sortiere_liste(stud_type** studenten_liste, int (*compare)(stud_type*, stud_type*))
{
	stud_type *curr;
	sort_type **sorted_list;
    
    sorted_list = NULL;
	
	if(studenten_liste == NULL)
	{
		return NULL;
	}
    
	curr = *studenten_liste;
	while(curr != NULL)
	{
		if(sorted_list == NULL)
		{
			//Erstelle erstes Listenelement
			sort_type *first_element = (sort_type*) malloc(sizeof(sort_type));
			
			if(first_element == NULL)
    		{
    			printf("%s\n", strerror(errno));
    			return NULL;
    		}
			
			sorted_list = &first_element;
			first_element->content = curr;
			first_element->next_sort = NULL;
		}
		else
		{
			sort_type *zeiger = (sort_type*) malloc(sizeof(sort_type));
			
			if(zeiger == NULL)
    		{
    			printf("%s\n", strerror(errno));
    			return NULL;
    		}
			
			zeiger->content = curr;
			zeiger->next_sort = NULL;
			
			sort_type *tmp = *sorted_list;
			sort_type *prev;
			prev = NULL;
			
			while(tmp != NULL)
			{
				if((*compare)(tmp->content, zeiger->content) > 0)
				{
					if(prev == NULL)
					{
						zeiger->next_sort = tmp;
						*sorted_list = zeiger;
					}
					else
					{
						prev->next_sort = zeiger;
						zeiger->next_sort = tmp;
					}
					break;
				}
				else if((*compare)(tmp->content, zeiger->content) < 0)
				{
					if(tmp->next_sort == NULL)
					{
						tmp->next_sort = zeiger;
						break;
					}
				}
				else
				{
					if(prev != NULL)
					{
						prev->next_sort = zeiger;
						zeiger->next_sort = tmp;
					}
					else
					{
						zeiger->next_sort = tmp;
						*sorted_list = zeiger;
					}
					break;
				}

				prev = tmp;
				tmp = tmp->next_sort;
			}
		}
		curr = curr->next_student;
	}
    
	return sorted_list;
}

void print_sorted_list(sort_type** liste)
{
	sort_type *tmp;
    tmp = *liste;
	while(tmp != NULL)
	{
		printf("Matnum: %d, Vorname: %s, Nachname %s\n", tmp->content->matnum,
               tmp->content->vorname, tmp->content->nachname);
		tmp = tmp->next_sort;
	}
}

int sortiere_vorname(stud_type* a, stud_type* b)
{
	char vorname_a[20];
	char vorname_b[20];
	int i;
	
	strcpy(vorname_a, a->vorname);
	strcpy(vorname_b, b->vorname);
	
	

	for(i = 0; i < 20; i++)
	{
		if(vorname_a[i] == '\0' && vorname_b[i] != '\0')
		{
			return 1;
		}

		if(vorname_b[i] == '\0' && vorname_a[i] != '\0')
		{
			return -1;
		}
		
		if(vorname_a[i] > vorname_b[i])
		{
			return 1;
		}
		else if(vorname_a[i] < vorname_b[i])
		{
			return -1;
		}
	}

	return 0; 
}

int sortiere_nachname(stud_type* a, stud_type* b)
{
	char nachname_a[20];
	char nachname_b[20];
	int i;
	
	strcpy(nachname_a, a->nachname);
	strcpy(nachname_b, b->nachname);
	
	

	for(i = 0; i < 20; i++)
	{
		if(nachname_a[i] == '\0' && nachname_b[i] != '\0')
		{
			return 1;
		}

		if(nachname_b[i] == '\0' && nachname_a[i] != '\0')
		{
			return -1;
		}
		
		if(nachname_a[i] > nachname_b[i])
		{
			return 1;
		}
		else if(nachname_a[i] < nachname_b[i])
		{
			return -1;
		}
	}

	return 0; 
}

void save(stud_type** studenten_liste)
{
	int fh;
	
	fh = open("studenten_liste.dat", O_RDWR | O_TRUNC | O_CREAT, S_IRWXU);
	if(fh != -1)
	{
		stud_type *tmp;
		tmp = *studenten_liste;
		
		while(tmp != NULL)
		{	
			if(write(fh, tmp, sizeof(*tmp)) == -1)
			{
				printf("Achtung! Matrikelnummer: %d konnte nicht gespeichert werden.\n", tmp->matnum);
				printf("%s\n", strerror(errno));
			}
			
			tmp = tmp->next_student;
		}
	}
	else
	{
		printf("%s\n", strerror(errno));
	}
	
	if(close(fh) == -1)
	{
		printf("%s\n", strerror(errno));
	}
}

void load(stud_type** studenten_liste)
{
	int fh, matnum;
	char vorname[20], nachname[20];
	stud_type puffer;
	
	fh = open("studenten_liste.dat", O_RDONLY);
	
	if(fh != -1)
	{
		while(read(fh, &puffer, sizeof(puffer)))
		{
			matnum = puffer.matnum;
			strcpy(vorname, puffer.vorname);
			strcpy(nachname, puffer.nachname);
			enqueue(studenten_liste, matnum, vorname, nachname);
		}
	}
	else
	{
		printf("%s\n", strerror(errno));
	}
	
	if(close(fh) == -1)
	{
		printf("%s\n", strerror(errno));
	}
}
/* Test der Listenfunktionen  */
int main()                                     
{
    /* Initialisierung der Datenbank */
    stud_type *studenten_liste = NULL;
    /* platz fŸr vorname */
    char vorname[20];
    /* platz fŸr nachname */
    char nachname[20];
    /* zeiger fŸr iteration */
    stud_type *curr;
    int fh;
	fh = open("studenten_liste.dat", O_RDONLY);

    if(fh == -1)
    {
    	printf(">>> Fuege neuen Studenten in die Liste ein: Bill Clinton [6666] ...\n");
		enqueue(&studenten_liste, 6666, "Bill", "Clinton");
		printf(">>> Fuege neuen Studenten in die Liste ein: Hillary Clinton [4711] ...\n");
		enqueue(&studenten_liste, 4711, "Hillary", "Clinton");
		printf(">>> Fuege neuen Studenten in die Liste ein: Newt Gingrich [9999] ...\n");
		enqueue(&studenten_liste, 9999, "Newt", "Gingrich");
		printf(">>> Test, ob die Matrikelnummer 0815 bereits erfasst wurde ...\n");

		if (get_student(studenten_liste, 815, vorname, nachname))
		    printf("    Matrikelnummer %4i: %s %s\n", 815, vorname, nachname);
		else
		    printf("    Matrikelnummer %4i ist unbekannt\n", 815);

		printf(">>> Fuege neuen Studenten in die Liste ein: Monica Lewinsky [0815] ...\n");
		enqueue(&studenten_liste, 815, "Monica", "Lewinsky");
		printf(">>> Loesche die Matrikelnummer 4711 ...\n");

		if (dequeue(&studenten_liste, 4711))
		    printf("    Matrikelnummer %4i geloescht\n", 4711);
		else
		    printf("    Matrikelnummer %4i war nicht erfasst\n", 4711);

		printf(">>> Test ob die Studentenliste leer ist ...\n");

		if (is_empty(&studenten_liste))
		    printf("    Die Studentenliste ist leer \n");
		else
		    printf("    Die Studentenliste ist nicht leer \n");

		printf(">>> Test, ob die Matrikelnummer 6666 bereits erfasst wurde ...\n");

		if (get_student(studenten_liste, 6666, vorname, nachname))
		    printf("    Matrikelnummer %4i: %s %s\n", 6666, vorname, nachname);
		else
		    printf("    Matrikelnummer %4i ist unbekannt\n", 6666);
	 
		printf(">>> Loesche die Matrikelnummer 9998 ...\n");
	 
		if (dequeue(&studenten_liste, 9998))
		    printf("    Matrikelnummer %4i geloescht\n", 9998);
		else
		    printf("    Matrikelnummer %4i war nicht erfasst\n", 9998);

		printf(">>> Loesche die Matrikelnummer 9999 ...\n");

		if (dequeue(&studenten_liste, 9999))
		    printf("    Matrikelnummer %4i geloescht\n", 9999);
		else
		    printf("    Matrikelnummer %4i war nicht erfasst\n", 9999);

		printf(">>> Gebe alle erfassten Studenten aus ...\n");
		curr = studenten_liste;

		while (curr != NULL) {
		    printf("    Matrikelnummer %4i: %s %s\n", 
				   curr->matnum, curr->vorname, curr->nachname);
		    curr = curr->next_student;
		}
		
		printf(">>> Fuege neuen Studenten in die Liste ein: Sven Festag [330723] ...\n");
		enqueue(&studenten_liste, 330723, "Sven", "Festag");
		printf(">>> Fuege neuen Studenten in die Liste ein: Simon Glimm [335266] ...\n");
		enqueue(&studenten_liste, 335266, "Simon", "Glimm");
		printf(">>> Fuege neuen Studenten in die Liste ein: Nicola Gatto [334836] ...\n");
		enqueue(&studenten_liste, 334836, "Nicola", "Gatto");
		
		sort_type** my_sort;
		my_sort = sortiere_liste(&studenten_liste, sortiere_vorname);
		
		if(my_sort != NULL)
		{
			print_sorted_list(my_sort);
		}
	
		printf(">>> Sortiere nach Nachnamen und gebe aus...\n");
		my_sort = sortiere_liste(&studenten_liste, sortiere_nachname);
		
		if(my_sort != NULL)
		{
			print_sorted_list(my_sort);
		}
		
		printf(">>> Elemente werden in eine Datei gespeichert...\n");
		save(&studenten_liste);
		printf("...Abgeschlossen\n");
	}
	else
	{
		close(fh);
		printf("Loading File gefunden. Lese ein...\n");
		load(&studenten_liste);
		
		printf(">>> Gebe alle erfassten Studenten aus ...\n");
		curr = studenten_liste;
		while (curr != NULL) {
		    printf("    Matrikelnummer %4i: %s %s\n", 
				   curr->matnum, curr->vorname, curr->nachname);
		    curr = curr->next_student;
		}
	}
	

    return 0;
}
