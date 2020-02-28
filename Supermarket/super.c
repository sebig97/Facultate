#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Produs_supermarket{
	char nume[30];
	char categorie[30];
	int pret;
	int cantitate;
	int stoc;
	struct Produs_supermarket *next;
};
typedef struct Produs_supermarket PS;

struct Produs{
     char* nume;
     char* categorie;
     int cantitate;
     struct Produs *next;
};
typedef struct Produs P;

void introducere1(PS *varf_lista, PS *final_lista, PS *produs)
{
	if(varf_lista==NULL)
    {
        varf_lista = produs;
        final_lista = varf_lista;
        varf_lista->next=NULL;
    }
    else
    {
        final_lista->next = produs;
        final_lista=produs;
        final_lista->next=NULL;
    }
}

void introducere2(P *varf_lista, P *final_lista, P *produs)
{
	if(varf_lista==NULL)
    {
        varf_lista = produs;
        final_lista = varf_lista;
        varf_lista->next=NULL;
    }
    else
    {
        final_lista->next = produs;
        final_lista=produs;
        final_lista->next=NULL;
    }
}

void citire_lista1(int *nr, PS *varf_lista,PS *final_lista, char *date1)
{
    PS *produs = (PS *)malloc(sizeof(PS));

    FILE *f;
    f = fopen("date1.txt","r");
    char sir[100];

    fscanf(f,"%d",nr);
    fgets(sir, 100, f);

    while(fgets(sir, 100, f))
    {
         char *p = strtok(sir," ");
         strcpy(produs->nume,p);
         p=strtok(NULL," ");
         strcpy(produs->categorie,p);
         p=strtok(NULL," ");
         produs->pret = atoi(p);
         p=strtok(NULL," ");
         produs->cantitate = atoi(p);
         p=strtok(NULL," ");
         produs->stoc = atoi(p);

         introducere1(varf_lista,final_lista,produs);
    }
}

void citire_lista2(int *nr, P *varf_lista, P *final_lista, char *date2)
{
    char sir[100];
    P *produs = (P *)malloc(sizeof(P));
    FILE *f;
    f=fopen("date2.txt","r");

    fscanf(f,"%d",nr);
    fgets(sir, 100, f);

    while(fgets(sir, 100, f))
    {
         char *p = strtok(sir," ");
         strcpy(produs->nume,p);
         p=strtok(NULL," ");
         strcpy(produs->categorie,p);
         p=strtok(NULL," ");
         produs->cantitate = atoi(p);

         introducere2(varf_lista,final_lista,produs);
    }
}


void eliminare(P *varf_lista, char nume_cautat[20])
{
    int i;
    if(varf_lista==NULL)
        printf("EROARE");

        P *t=varf_lista;
        P *q;
        int gasit=0;
        while(t!=NULL)
        {
            if(strcmp(t->nume, nume_cautat)==0) {gasit=1;break;}
            if(t->next != NULL)
            {q=t;
            t=t->next;
            }

        }

        if(gasit)
          {q=t->next;
           free(t);}
}

int main()
{
    int nr1,nr2;
    PS *varf_lista1, *final_lista1;
    P *varf_lista2, *final_lista2;

    char a[10]="date1.txt";
    char b[20]="date2.txt";

    citire_lista1(&nr1,varf_lista1,final_lista1,a);
    citire_lista2(&nr2,varf_lista2,final_lista2,b);

    int k=0;
    char nume[30];
    PS *z = varf_lista1;
    while(z!=NULL)
    {
        k++;
        if(z->stoc == 0)
            eliminare(varf_lista2,z->nume);

        z=z->next;
    }




}
