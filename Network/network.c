#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

struct PC
{
    char * nume_pc;
    char * IP_pc;
    int stare;
};
typedef struct PC PC;

struct Router
{
    char * nume_router;
    char * IP_router;
    int nr_pc_conectate;

    PC * vector_pc;
};
typedef struct Router Router;

void citire_date(char *nume_fisier, int *nr_routere, int *nr_muchii, int *nr_operatii, Router ** vector_router, int ***M, char ***operatii, char ***ipuri)
{

    FILE *f;
    f = fopen(nume_fisier,"rt");
    char sir[100];
    int n=0,i,j,k,l;

    fscanf(f,"%d",nr_routere);
    *ipuri = (char **)malloc(*nr_routere * sizeof(char **));
    *vector_router = (Router *)malloc(*nr_routere * sizeof(Router));
    fgets(sir, 100, f);

    for(j=0; j<*nr_routere; j++)
    {
        fgets(sir, 100, f);

        (*vector_router)[n].nume_router = (char *)malloc(strlen(sir)* sizeof(char));
        strcpy((*vector_router)[n].nume_router,sir);
        (*vector_router)[n].nume_router[strlen(sir)-1] = 0;

        fgets(sir, 100, f);

        (*vector_router)[n].IP_router = (char *)malloc(strlen(sir));
        (*ipuri)[n] = (char *)malloc(strlen(sir));
        strcpy((*vector_router)[n].IP_router,sir);
        strcpy((*ipuri)[n], sir);
        (*vector_router)[n].IP_router[strlen(sir)-1] = 0;
        (*ipuri)[n][strlen(sir)-1] = 0;

        fgets(sir, 100, f);
        (*vector_router)[n].nr_pc_conectate=atoi(sir);
        (*vector_router)[n].vector_pc = (PC *)malloc((*vector_router)[n].nr_pc_conectate * sizeof(PC));
        for(i=0; i<(*vector_router)[n].nr_pc_conectate; i++)
        {

            fgets(sir, 100, f);
            (*vector_router)[n].vector_pc[i].nume_pc =(char *)malloc(strlen(sir)+1);
            strcpy((*vector_router)[n].vector_pc[i].nume_pc, sir);
            (*vector_router)[n].vector_pc[i].nume_pc[strlen(sir)-1] = 0;
            fgets(sir, 100, f);
            (*vector_router)[n].vector_pc[i].IP_pc =(char *)malloc(strlen(sir)+1);
            strcpy((*vector_router)[n].vector_pc[i].IP_pc, sir);
            (*vector_router)[n].vector_pc[i].IP_pc[strlen(sir)-1] = 0;
            fgets(sir, 100, f);
            (*vector_router)[n].vector_pc[i].stare = atoi(sir);
        }
        n++;
    }

    fscanf(f,"%d",nr_muchii);
    fgets(sir, 100, f);

    *M = (int **)calloc(*nr_routere, sizeof(int **));
    for(i=0; i<*nr_routere; i++)
        (*M)[i] = (int *)calloc(*nr_routere, sizeof(int *));


    for(l=0; l<*nr_muchii; l++)
    {
        fgets(sir, 100, f);
        char *p = strtok(sir," ");
        i=atoi(p+6);
        p = strtok(NULL," ");
        j=atoi(p+6);
        p = strtok(NULL," ");
        k=atoi(p);
        (*M)[i-1][j-1] = (*M)[j-1][i-1] = k;
    }

    fscanf(f,"%d",nr_operatii);
    fgets(sir, 100, f);

    *operatii = (char **)malloc(*nr_operatii * sizeof(char **));
    for(i=0; i<*nr_operatii; i++)
    {

        fgets(sir, 100, f);
        (*operatii)[i] = (char *)malloc(strlen(sir));
        strcpy((*operatii)[i],sir);
        (*operatii)[i][strlen(sir)-1] = 0;
    }

}

unsigned index_valoare_minima(int nr_routere, int *distanta, int *vizitat)
{
    int i, index = 0, min=INT_MAX;
    for(i=0; i<nr_routere; i++)
    {
        if(distanta[i] < min && vizitat[i] == 0)
        {
            min = distanta[i];
            index = i;
        }
    }


    return index;
}

int legatura(int a, int b, int **M, int nr_routere)
{
    if(M[a][b] != 0)
        return 1;

    int z=nr_routere, *timp, *vizitat, t = 0,i,j;

    timp =(int *)malloc(nr_routere * sizeof(int));
    vizitat =(int *)malloc(nr_routere * sizeof(int));

    for(i=0; i<nr_routere; i++)
    {
        vizitat[i] = 0;
        timp[i]= INT_MAX;
    }
    timp[a] = t++;

    while(z != 0)
    {
        unsigned nod = index_valoare_minima(nr_routere,timp,vizitat);

        vizitat[nod] = 1;

        for(j=0; j<nr_routere; j++)
        {
            if(j == b)
                return 1;
            if(M[nod][j] != 0)
            {
                timp[j] = t++;
            }
        }
        z--;
    }
    return 0;
}

void ping(FILE **f, int nr_routere, char *a, char *b, int **M, Router * vector_router)
{
    int i,j,x=-1,y=-1;
    char *m, *n;
    m = (char *)malloc(strlen(a)+1);
    strcpy(m,a);
    n = (char *)malloc(strlen(b)+1);
    strcpy(n,b);

    char *e = strtok(a,".");
    e = strtok(NULL,".");
    e = strtok(NULL,".");
    x=atoi(e)-1;

    char *t = strtok(b,".");
    t = strtok(NULL,".");
    t = strtok(NULL,".");
    y=atoi(t)-1;

    for(i=0; i<nr_routere; i++)
        for(j=0; j<vector_router[i].nr_pc_conectate; j++)
        {
            if(!strcmp(m,vector_router[i].vector_pc[j].IP_pc) && vector_router[i].vector_pc[j].stare == 0)
            {

                fprintf(*f,"%s \n","NO");
                return;
            }
            if(!strcmp(n,vector_router[i].vector_pc[j].IP_pc) && vector_router[i].vector_pc[j].stare == 0)
            {

                fprintf(*f,"%s \n","NO");
                return;
            }
        }


    if(legatura(x, y, M, nr_routere))
    {
        fprintf(*f,"%s \n","OK");
        return;
    }

    fprintf(*f,"%s \n","NO");
}

void sipn(FILE **f, int nr_routere, char *a, int **M, Router * vector_router)
{
    int i,j;
    for(i=0; i<nr_routere; i++)
        if(!strcmp(a,vector_router[i].IP_router))
            for(j=0; j<nr_routere; j++)
                if(M[i][j] !=0 )
                    fprintf(*f,"%s ", vector_router[j].IP_router);

    fprintf(*f,"\n");
}

void si(FILE **f, int nr_routere, char *a, int **M, Router * vector_router)
{
    int i,j;
    for(i=0; i<nr_routere; i++)
        if(!strcmp(a,vector_router[i].IP_router))
            for(j=0; j<vector_router[i].nr_pc_conectate; j++)
                if(vector_router[i].vector_pc[j].stare == 1)
                    fprintf(*f, "%s ", vector_router[i].vector_pc[j].IP_pc);

    fprintf(*f,"\n");
}

void up(int nr_routere, char *a, int **M, Router ** vector_router)
{
    int i,j;
    for(i=0; i<nr_routere; i++)
        for(j=0; j<(*vector_router)[i].nr_pc_conectate; j++)
            if(!strcmp(a,(*vector_router)[i].vector_pc[j].IP_pc) && (*vector_router)[i].vector_pc[j].stare == 0)
                (*vector_router)[i].vector_pc[j].stare = 1;
}

void broke(int nr_routere, char *a, int **M, Router ** vector_router)
{
    int i,j;
    for(i=0; i<nr_routere; i++)
        for(j=0; j<(*vector_router)[i].nr_pc_conectate; j++)
            if(!strcmp(a,(*vector_router)[i].vector_pc[j].IP_pc) && (*vector_router)[i].vector_pc[j].stare == 1)
                (*vector_router)[i].vector_pc[j].stare = 0;
}

void lc(int nr_routere, char *a, char *b, int **M, Router * vector_router)
{
    int i,j,x,y;
    for(i=0; i<nr_routere; i++)
    {
        if(!strcmp(a,vector_router[i].IP_router))
            x=i;
        if(!strcmp(b,vector_router[i].IP_router))
            y=i;
    }

    M[x][y] = M[y][x] = 0;
}

void add(int nr_routere, char *a, char *b, int c, int ***M, Router * vector_router)
{
    int i,j,x,y;
    for(i=0; i<nr_routere; i++)
    {
        if(!strcmp(a,vector_router[i].IP_router))
            x=i;
        if(!strcmp(b,vector_router[i].IP_router))
            y=i;
    }
    (*M)[x][y] = (*M)[y][x] = c;
}

void dijkstra(FILE **f,int nr_routere, char *a, char *b, int **M, char **ipuri)
{
    int z = nr_routere, j, *drum, *distanta, *parinte;
    int *vizitat,i;
    vizitat =(int *)malloc(nr_routere * sizeof(int));
    distanta =(int *)malloc(nr_routere * sizeof(int));
    parinte =(int *)malloc(nr_routere * sizeof(int));


    for(i=0; i<nr_routere; i++)
    {
        vizitat[i] = 0 ;
        distanta[i]= INT_MAX;
    }

    int val_pozitie;
    char *e = strtok(a,".");
    e = strtok(NULL,".");
    e = strtok(NULL,".");

    char *t = strtok(b,".");
    t = strtok(NULL,".");
    t = strtok(NULL,".");

    unsigned nod_start = atoi(e)-1, nod_final = atoi(t)-1;
    distanta[nod_start] = 0;

    while(z != 0)
    {
        unsigned nod = index_valoare_minima(nr_routere,distanta,vizitat);

        vizitat[nod] = 1;

        for(j=0; j<nr_routere; j++)
        {
            val_pozitie = M[nod][j];
            if(val_pozitie != 0)
            {
                if(distanta[j] > distanta[nod] + val_pozitie)
                {
                    distanta[j] = distanta[nod] + val_pozitie;
                    parinte[j] = nod;
                }
            }
        }
        z--;
    }
    if(distanta[nod_final] != INT_MAX)
    {
        printf("%d \n", distanta[nod_final]);
        drum = (int *)malloc((distanta[nod_final]+1) * sizeof(int));
        int r = nod_final, l=0;
        while(r != nod_start)
        {
            drum[l++] = r;
            r=parinte[r];
        }
        drum[l++] = nod_start;

        int w;
        for(w=l-1; w>=0; w--)
        {
            fprintf(*f,"%s ",ipuri[drum[w]]);
        }
        fprintf(*f,"\n");

    }
    else
        fprintf(*f,"\n");

}



int main(int argc, char **argv)
{
    FILE *g;
    g = fopen(argv[2], "wt");

    int nr_routere, nr_muchii, nr_operatii, i;
    Router * vector_router;
    int **M;
    char **operatii, **ipuri;
    int *parinte;

    citire_date(argv[1], &nr_routere, &nr_muchii, &nr_operatii, &vector_router, &M, &operatii,&ipuri);
    printf("OB1 \n");
    char *ip1, *ip2;
    for(i=0; i<nr_operatii; i++)
    {

        char *p= strtok(operatii[i]," ");
        printf("%s \n", p);
        if(strcmp(p,"ping") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);

            p=strtok(NULL," ");
            ip2 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip2,p);

            ping(&g,nr_routere,ip1,ip2,M,vector_router);
        }

        if(strcmp(p,"sipn") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);


            sipn(&g,nr_routere,ip1,M,vector_router);
        }

        if(strcmp(p,"si") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);

            si(&g,nr_routere,ip1,M,vector_router);
        }

        if(strcmp(p,"up") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);

            up(nr_routere,ip1,M,&vector_router);
        }

        if(strcmp(p,"broke") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);

            broke(nr_routere,ip1,M,&vector_router);
        }

        if(strcmp(p,"lc") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);

            p=strtok(NULL," ");
            ip2 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip2,p);

            lc(nr_routere,ip1,ip2,M,vector_router);
        }


        int d;
        if(strcmp(p,"add") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);

            p=strtok(NULL," ");
            ip2 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip2,p);

            p=strtok(NULL," ");
            d=atoi(p);

            add(nr_routere,ip1,ip2,d,&M,vector_router);
        }

        if(strcmp(p,"trace") == 0)
        {
            p=strtok(NULL," ");
            ip1 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip1,p);

            p=strtok(NULL," ");
            ip2 = (char *)malloc(sizeof(char) * (strlen(p)+1));
            strcpy(ip2,p);

            dijkstra(&g,nr_routere,ip1,ip2,M,ipuri);

        }
    }
}
