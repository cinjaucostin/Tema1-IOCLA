#include "utils.h"

int main()
{
	Dir *director_parinte = create_new_dir(NULL, "home");
	Dir **director_curent = &director_parinte;
	int k;
	char *delimitators = " \n", *token, *line, **comenzi;
	//Alocam memorie pentru a putea citi cu fgets
	line = calloc(MAX_INPUT_LINE_SIZE, sizeof(char));
	//Matricea de cuvinte in care vom retine separat fiecare cuvant de pe fiecare linie citita
	//Practic comenzi[0] va reprezenta primul cuvant de pe linia citita(comanda in sine:mkdir, touch, etc)
	//comenzi[1] si comenzi[2] pot fi completate sau nu cu informatii utile, in functie de caracterul comenzi[0]
	//daca in comenzi[0] avem mkdir ne vom folosi de comenzi[1] ca fiind numele noului element
	comenzi = calloc(3, sizeof(char *));
	for (int i = 0; i < 3; i++)
		comenzi[i] = calloc(10, sizeof(char));

	do
	{
		fgets(line, 50, stdin); //Citim intreaga linie
		k = 0;					//Ne folosim de strtok pentru a imparti in tokeni linia citita
		token = strtok(line, delimitators);
		while (token != NULL)
		{
			strcpy(comenzi[k++], token); //Adaugam pe linia k din matrice continutul token-ului
			token = strtok(NULL, delimitators);
		}
		if (k == 1) //Inseamna ca avem de efectuat o operatie de tipul ls/stop si stim ca in comenzi[1] si comenzi[2] nu avem copiata nicio informatie
		{
			if (strcmp(comenzi[0], "ls") == 0)
				ls(*director_curent);
			else if (strcmp(comenzi[0], "pwd") == 0)
			{
				char *cale = pwd(*director_curent);
				printf("%s\n", cale);
				free(cale);
			}
			else if (strcmp(comenzi[0], "tree") == 0)
			{
				tree(*director_curent, -1);
			}
			else if (strcmp(comenzi[0], "stop") == 0)
			{
				while(strcmp((*director_curent)->name,"home")!=0)//Fortam intoarcerea in directorul home pentru a putea ulterior apela stop pentru intreaga ierarhie(pornind din home)
					cd(director_curent,"..");
				stop(*director_curent);
				break;
			}
		}
		else if (k == 2) //Inseamna ca avem de efectuat o operatie de tipul touch/mkdir/rm/rmdir/cd deci ne vom folosi de comenzi[1]
		{
			if (strcmp(comenzi[0], "touch") == 0)
				touch(*director_curent, comenzi[1]);
			else if (strcmp(comenzi[0], "mkdir") == 0)
				mkdir(*director_curent, comenzi[1]);
			else if (strcmp(comenzi[0], "rm") == 0)
				rm(*director_curent, comenzi[1]);
			else if (strcmp(comenzi[0], "rmdir") == 0)
				rmdir(*director_curent, comenzi[1]);
			else if (strcmp(comenzi[0], "cd") == 0)
				cd(director_curent, comenzi[1]);
		}
		else if (k == 3) //Inseamna ca avem de efectuat o operatie de tip mv deci ne vom folosi atat de comenzi[1]=oldname cat si de comenzi[2]=newname
		{
			if (strcmp(comenzi[0], "mv") == 0)
				mv(*director_curent, comenzi[1], comenzi[2]);
		}
		free(token);
	} while (1);
	//Eliberam memoria ocupata de matricea de cuvinte
	for (int i = 0; i < 3; i++)
		free(comenzi[i]);
	free(comenzi);
	free(line); //Eliberam memoria ocupata de pointerul de care ne-am folosit la citirea fiecarei linii
	return 0;
}
