#include "utils.h"

//Functie auxiliara pentru a crea un nou fisier
File *create_new_file(Dir *parent, char *name)
{
    File *new_file = calloc(1, sizeof(File)); //Alocam memorie pentru noua structura de tip fisier
    new_file->name = strdup(name);            //alocam memorie pentru numele noului fisier si copiem string-ul dat ca parametru in acest camp
    new_file->parent = parent;                //facem legatura cu directorul parinte al fisierului
    new_file->next = NULL;                    //legatura next este initializata cu NULL
    return new_file;
}

void touch(Dir *parent, char *name)
{
    if (parent->head_children_files == NULL)
    {
        File *new_file = create_new_file(parent, name);
        parent->head_children_files = new_file; //Adaugam noul element in lista de fisiere
    }
    else //Inseamna ca in lista de fisiere existau deja alte fisiere
    {
        File *parcurgere_lista_fisiere = parent->head_children_files; //Pentru a putea parcurge lista de fisiere a directorului curent
        int flag = 0;
        while (parcurgere_lista_fisiere) //Parcurgere pentru a verifica daca fisierul exista deja in lista de fisiere
        {
            if (strcmp(parcurgere_lista_fisiere->name, name) == 0) //Inseamna ca fisierul exista deja in lista de fisiere
            {
                flag = 1;
                break;
            }
            parcurgere_lista_fisiere = parcurgere_lista_fisiere->next;
        }
        if (flag == 1) //Cazul in care fisierul deja exista in lista de fisiere a directorului curent
        {
            printf("File already exists\n");
        }
        else
        {
            File *new_file = create_new_file(parent, name);
            parcurgere_lista_fisiere = parent->head_children_files;
            while (parcurgere_lista_fisiere->next != NULL) //Pentru a afla pointerul la ultimul element din lista de fisiere
                parcurgere_lista_fisiere = parcurgere_lista_fisiere->next;
            parcurgere_lista_fisiere->next = new_file; //Adaugam noul fisier in lista de fisiere(realizam legatura next a ultimului element cu noul fisier)
        }
    }
}

//Functie auxiliara pentru a crea un noi director
Dir *create_new_dir(Dir *parent, char *name)
{
    Dir *new_dir = calloc(1, sizeof(Dir)); //Alocam memorie pentru noua celula de tip Dir
    new_dir->name = strdup(name);          //alocam memorie numelui directorului si copiem string-ul dat ca argument in acest camp
    new_dir->parent = parent;              //legatura cu directorul parinte al directorului
    new_dir->next = NULL;                  //legatura next a directorului o initializam cu NULL
    new_dir->head_children_dirs = NULL;
    new_dir->head_children_files = NULL;
    return new_dir;
}

void mkdir(Dir *parent, char *name)
{
    if (parent->head_children_dirs == NULL) //Cazul in care lista head_children_dirs este nula
    {
        Dir *new_dir = create_new_dir(parent, name);
        parent->head_children_dirs = new_dir; //adaugam noul element in lista
    }
    else //Inseamna ca lista de directoare nu este goala si vom insera la sfarsitul acesteia noul director
    {
        Dir *parcurgere_lista_directoare = parent->head_children_dirs;
        int flag = 0;
        while (parcurgere_lista_directoare) //Parcurgere pentru a vedea daca directorul se afla deja in lista
        {
            if (strcmp(parcurgere_lista_directoare->name, name) == 0) //Inseamna ca directorul deja exista in lista de directoare
            {
                flag = 1;
                break;
            }
            parcurgere_lista_directoare = parcurgere_lista_directoare->next;
        }
        if (flag == 1) //Cazul in care directorul deja exista in lista de directoare
        {
            printf("Directory already exists\n");
        }
        else
        {
            Dir *new_dir = create_new_dir(parent, name);
            parcurgere_lista_directoare = parent->head_children_dirs; //pentru a putea parcurge din nou lista de directoare
            while (parcurgere_lista_directoare->next != NULL)         //pentru a afla pointerul la ultimul element din lista
                parcurgere_lista_directoare = parcurgere_lista_directoare->next;
            parcurgere_lista_directoare->next = new_dir; //adaugam noul director la sfarsitul listei de directoare
        }
    }
}

void ls(Dir *parent)
{
    File *parcurgere_fisiere = parent->head_children_files;
    Dir *parcurgere_directoare = parent->head_children_dirs;
    while (parcurgere_directoare)
    {
        printf("%s\n", parcurgere_directoare->name);
        parcurgere_directoare = parcurgere_directoare->next;
    }
    while (parcurgere_fisiere)
    {
        printf("%s\n", parcurgere_fisiere->name);
        parcurgere_fisiere = parcurgere_fisiere->next;
    }
}

void rm(Dir *parent, char *name)
{
    if (parent->head_children_files == NULL) //In caz ca lista de fisiere a directorului parinte este nula este clar ca nu avem ce fisier sa stergem si afisam mesajul cerut
    {
        printf("Could not find the file\n");
        return;
    }
    else
    {
        File *parcurgere_lista_fisiere = parent->head_children_files, *eliminat = parent->head_children_files;
        if (strcmp(parcurgere_lista_fisiere->name, name) == 0) //Inseamna ca trebuie eliminat primul element
        {
            parent->head_children_files = parcurgere_lista_fisiere->next; //Practic primul element va deveni al doilea din lista anterioara
            free(eliminat->name);
            free(eliminat); //Eliberam memoria celui eliminat
            return;         //Ne oprim pentru ca am eliminat fisierul din lista
        }
        else //Inseamna ca nu primul element este cel care trebuie eliminat
        {
            while (parcurgere_lista_fisiere->next) //parcurgem lista de fisiere a directorului curent
            {
                if (strcmp(parcurgere_lista_fisiere->next->name, name) == 0) //inseamna ca fisierul urmator trebuie eliminat
                {
                    eliminat = parcurgere_lista_fisiere->next;       //ii salvam adresa intr-un pointer auxiliar pentru a putea elibera memoria ocupata de acesta
                    parcurgere_lista_fisiere->next = eliminat->next; //facem legatura astfel incat sa il stergem din lista
                    free(eliminat->name);                            //Eliberam memoria ocupata de numele fisierului eliminat
                    free(eliminat);                                  //Eliberam memoria ocupata de fisierul eliminat
                    return;                                          //Ne oprim pentru ca am eliminat deja fisierul
                }
                parcurgere_lista_fisiere = parcurgere_lista_fisiere->next; //Iteram catre urmatorul element
            }
        }
        //Inseamna ca fisierul nu a fost gasit
        printf("Could not find the file\n");
    }
}

void elibereaza_fisiere(Dir *target)
{
    File *parcurgere_file = target->head_children_files, *elimina_fisier;
    while (parcurgere_file)
    {
        elimina_fisier = parcurgere_file;        //Pastram pointerul la elementul curent intr-un pointer auxiliar(pentru a putea da free ulterior)
        parcurgere_file = parcurgere_file->next; //Iteram catre urmatorul element
        free(elimina_fisier->name);              //Eliberam memoria ocupata de numele fisierului sters
        free(elimina_fisier);                    //Eliberam memoria ocupata de fisierul sters
    }
}

void stop(Dir *target) //Definim functia recursiv pentru a trece prin fiecare director in parte, sa ii eliberam lista de fisiere(daca este cazul) si pe el in sine
{
    if (target == NULL)
        return;
    else
    {
        Dir *dirs = target->head_children_dirs;
        while (dirs)//Pentru a parcurge intreaga lista de directoare a directorului curent
        {
            Dir *aux = dirs->next; //Pastram pointer la dirs->next pentru a nu avea probleme in urma recurentei
            stop(dirs);
            dirs = aux;
        }
        elibereaza_fisiere(target); // Eliberam lista de fisiere
        free(target->name);         //Eliberam memoria ocupata de numele directorului curent
        free(target);               //Eliberam memoria ocupata de directorul curent/target(cel caruia i-am eliberat mai devreme listele de fisiere si subdirectoare)
    }
}

void rmdir(Dir *parent, char *name)
{
    if (parent->head_children_dirs == NULL) //Daca lista de directoare a directorului parinte este nula inseamna ca nu avem ce director sa stergem si afisam mesajul aferent
    {
        printf("Could not find the dir\n");
        return;
    }
    else //Daca are ceva in lista de directoare
    {
        Dir *parcurgere_lista_directoare = parent->head_children_dirs, *eliminat = parent->head_children_dirs;
        if (strcmp(parcurgere_lista_directoare->name, name) == 0) //Inseamna ca trebuie eliminat primul director din lista
        {
            parent->head_children_dirs = parcurgere_lista_directoare->next; //Primul element din lista va fi acum al doilea element din lista de dinainte
            stop(eliminat);                                                 //Eliberam memoria ocupata de directorul eliminat
            //Ne folosim de functia stop pentru ca trebuie sa eliminam tot ce se afla "sub" directorul nostru
            return;
        }
        else //Trebuie sa cautam alt element in lista pentru a fi eliminat
        {
            while (parcurgere_lista_directoare->next)
            {
                if (strcmp(parcurgere_lista_directoare->next->name, name) == 0) //inseamna ca urmatorul director din lista trebuie eliminat
                {
                    eliminat = parcurgere_lista_directoare->next;       //Salvam pointerul la elementul pe care trebuie sa il eliminam
                    parcurgere_lista_directoare->next = eliminat->next; //facem legatura astfel incat sa il stergem din lista
                    stop(eliminat);
                    return;
                }
                parcurgere_lista_directoare = parcurgere_lista_directoare->next;
            }
        }
        //Inseamna ca directorul nu a fost gasit in lista si nu am avut ce elimina
        printf("Could not find the dir\n");
    }
}

void cd(Dir **target, char *name)
{
    if (strcmp(name, "..") == 0) //Cazul in care trebuie sa schimbam directorul curent cu parintele sau
    {
        if ((*target)->parent == NULL) //Daca directorul curent nu are parinte inseamna ca nu trebuie sa facem nicio modificare(suntem in directorul home)
        {
            return; //Ne oprim deoarece nu trebuie facuta nicio modificare
        }
        else
        {
            *target = (*target)->parent; //Daca nu este actualizam directorul curent nou ca fiind directorul parinte al directorului vechi(Ne deplasam in sus in ierarhia de directoare)
            return;                      //Ne oprim deoarece am facut deja modificarea necesara
        }
    }
    else //Inseamna ca trebuie sa schimbam directorul curent cu unul din lista sa de directoare
    {
        Dir *parcurgere_lista_directoare = (*target)->head_children_dirs; //Pointer auxiliar pentru a putea parcurge lista de directoare
        while (parcurgere_lista_directoare != NULL)
        {
            if (strcmp(parcurgere_lista_directoare->name, name) == 0) //Inseamna ca am gasit directorul pe care trebuie sa il facem director curent
            {
                *target = parcurgere_lista_directoare; //Schimbam directorul curent
                return;                                //Ne oprim deoarece am facut modificarea necesara
            }
            parcurgere_lista_directoare = parcurgere_lista_directoare->next; //Avansam in lista de directoare
        }
        printf("No directories found!\n"); //Daca am ajuns aici inseamna ca directorul pe care dorim sa il facem director curent nu exista in lista de directoare
    }
}

char *pwd(Dir *target)
{
    Dir *parcurgere_directoare = target;
    char **cale = calloc(10, sizeof(char *));         //Matrice de cuvinte in care memoram directoarele prin care trecem
    char *cale_directoare = calloc(30, sizeof(char)); //Aici vom retine de fapt calea
    int k = 0;                                        //Plecam cu index-ul k de la 0
    for (int i = 0; i < 10; i++)                      //Alocare memorie pentru fiecare cuvant in parte
        cale[i] = calloc(10, sizeof(char));
    while (parcurgere_directoare) //Cat timp nu ajungem la NULL
    {
        strcpy(cale[k++], parcurgere_directoare->name);        //Copiem numele directorului in care ne aflam in cuvantul de pe pozitia k si dupa incrementam k
        parcurgere_directoare = parcurgere_directoare->parent; //Avansam in sus (mergem in parintele directorului)
    }
    for (int i = k; i >= 0; i--) //Parcurgem in ordine inversa matricea de cuvinte in care sunt stocate numele directoarelor prin care am trecut pana a ajunge la home
    {
        //Ne folosim de strcat pentru a concatena directoarele ce reprezinta calea, inainte de fiecare director adaugam un /
        if (i == k)
            strcat(cale_directoare, cale[i]);
        else
        {
            strcat(cale_directoare, "/");
            strcat(cale_directoare, cale[i]);
        }
    }
    //Eliberam memoria ocupata de matricea de cuvinte
    for (int i = 0; i < 10; i++)
        free(cale[i]);
    free(cale);

    return cale_directoare;
}

//Functie auxiliara de afisare a 4 spatii * level + nume_fisier/nume_director
void print_n_spaces(int level, char *str)
{
    if (level == 0)
        printf("%s\n", str);
    else
    {
        for (int i = 0; i < level; i++)
            printf("    ");
        printf("%s\n", str);
    }
}

void tree(Dir *target, int level)
{
    if (target == NULL) //Cazul de baza al recurentei
        return;
    if (level >= 0) //Verificam daca level>=0 deoarece nu vrem sa facem afisarea si pentru directorul home
        print_n_spaces(level, target->name);
    Dir *dirs = target->head_children_dirs; //Pentru a putea parcurge lista de directoare
    while (dirs != NULL)
    {
        tree(dirs, level + 1); //Pentru fiecare director in parte din lista apelam recursiv functia tree
        dirs = dirs->next;
    }
    //Pentru a afisa numele fisierelor(practic ultimul pas din recurenta)
    File *file = target->head_children_files;
    while (file != NULL)
    {
        print_n_spaces(level + 1, file->name);
        file = file->next;
    }
}

int cauta_director_sau_fisier(Dir *parent, char *name)
{
    File *parcurgere_lista_fisiere = parent->head_children_files;
    Dir *parcurgere_lista_directoare = parent->head_children_dirs;
    while (parcurgere_lista_fisiere)
    {
        if (strcmp(parcurgere_lista_fisiere->name, name) == 0)
            return 1; //Inseamna ca am gasit in lista de fisiere
        parcurgere_lista_fisiere = parcurgere_lista_fisiere->next;
    }
    while (parcurgere_lista_directoare)
    {
        if (strcmp(parcurgere_lista_directoare->name, name) == 0)
            return 2; //Inseamna ca am gasit in lista de directoare
        parcurgere_lista_directoare = parcurgere_lista_directoare->next;
    }
    return 0; //Inseamna ca nu am gasit elementul cu numele dat in niciuna dintre liste
}

void mv(Dir *parent, char *oldname, char *newname)
{
    if (cauta_director_sau_fisier(parent, oldname) == 0) //Inseamna ca fisierul/directorul nu exista, deci nu avem cui sa schimbam numele
    {
        printf("File/Director not found\n"); //Afisam ca nu am gasit niciun fisier sau director caruia sa ii schimbam numele
        return;                              //Oprim functia
    }
    if (cauta_director_sau_fisier(parent, newname) == 1 || cauta_director_sau_fisier(parent, newname) == 2) //Inseamna ca exista un alt fisier/director cu numele newname, deci nu mai putem crea inca unul
    {
        printf("File/Director already exists\n"); //Afisam mesajul prin care specificam ca fisierul/directorul exista deja si nu mai putem crea inca unul
        return;                                   //Oprim functia
    }

    if (cauta_director_sau_fisier(parent, oldname) == 1) //Inseamna ca trebuie sa schimbam numele unui fisier
    {
        //Folosim operatiile specifice pentru fisiere
        rm(parent, oldname);
        touch(parent, newname);
        return;
    }
    else if (cauta_director_sau_fisier(parent, oldname) == 2) //Inseamna ca trebuie sa schimbam numele unui director
    {
        //Folosim operatiile specifice pentru pentru directoare
        rmdir(parent, oldname);
        mkdir(parent, newname);
        return;
    }
}
