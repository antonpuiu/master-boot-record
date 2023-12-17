#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NR_MAX_LITERE_FISIER 30
void CREATE_PARTITION(FILE *f,char NUME_PARTITIE[8],int MAX_D,int MAX_F)
{
    int i,j,MAXD,MAXF,*TOTAL_SPACE,ok,TOTAL_SPACE_NEED;
     char NUME_PARTITII[]="/dev/sd",NUMAR_PARTITII;
     unsigned char *ADRESE,POZITIE_PARTITIE_NOUA,c;
     fseek(f,0,SEEK_SET);     //Setez cursorul la inceputul fisierului.
     NUMAR_PARTITII=fgetc(f);      //Aflam din fisier numarul de partitii, acest numar fiind la inceputul fisierului.
     ADRESE=malloc(NUMAR_PARTITII*sizeof(char));  //Aloc dinamic memorie pentru fiecare adresa la care se gaseste cate o partitie.
     TOTAL_SPACE=malloc(NUMAR_PARTITII*sizeof(int));
     for(i=0;i<NUMAR_PARTITII;i++)      //In acest 'for' aflu adresele de inceput ale fiecarei partitii.
     {
          c=fgetc(f);
          while(c==0)
          {
               c=fgetc(f);
          }
          ADRESE[i]=c;
     }
     for(i=0;i<NUMAR_PARTITII;i++)      //Aflu total_space pentru fiecare partitie in parte.
     {
          TOTAL_SPACE[i]=0;
          fseek(f,0,SEEK_SET);
          fseek(f,ADRESE[i]+12,SEEK_CUR);    //Mut cursorul in locul unde gasesc 'MAXD' in fisierul binar.
          MAXD=fgetc(f); //Retin 'MAXD'.
          fseek(f,0,SEEK_SET);
          fseek(f,ADRESE[i]+16,SEEK_CUR);    //Mut cursorul in locul unde gasesc 'MAXF" in fisierul binar.
          MAXF=fgetc(f); //Retin 'MAXF'.
          TOTAL_SPACE[i]=20+MAXF*12+MAXD*12+MAXD*MAXF*12;    //Calculez total_space.
      }
      i=1;     //Acesta este un contor pentru a merge din adresa in adresa in fisierul binar.
      ok=1;
      while(1)
      {
            fseek(f,4*i,SEEK_SET);
            if(ftell(f)==ADRESE[0])     //Are conditia de oprire atunci cand cursorul ajunge la prima adresa de partitie.
               break;
            i++;    //Cresc contorul pentru a trece la urmatorul pas daca nu am gasit loc liber.
            c=fgetc(f);  //Retin valoarea din fisierul binar.
            if(c==0)     //Daca este 0, se poate introduce o partitie noua.
               {
                    ok=1;     
                    POZITIE_PARTITIE_NOUA=ftell(f)-1;       //Retin pozitia.
                    break;
               }

            if(c!=0)
               ok=0;
      }
      if(ok==0)
          {
               printf("INVALID\n");
          }
      if(ok==1)
      {
          for(i=0;i<NUMAR_PARTITII-1;i++)
          {
               TOTAL_SPACE_NEED=20+(MAX_F*12)+(MAX_D*12)+(MAX_D*MAX_F*12); //Calculez spatiul total de care este nevoie pentru noua partitie.
               if(ADRESE[i+1]-ADRESE[i]-TOTAL_SPACE[i]>=TOTAL_SPACE_NEED)  //Daca am loc
               {
                    fseek(f,POZITIE_PARTITIE_NOUA,SEEK_SET);     //incep introducerea partitiei in fisierul binar.
                    fputc((int)ADRESE[i]+TOTAL_SPACE[i],f);
                    printf("%d\n",(int)ADRESE[i]+TOTAL_SPACE[i]);
                    fseek(f,(int)ADRESE[i]+TOTAL_SPACE[i]+4,SEEK_SET);
                    for(j=0;j<8;j++)
                    {
                        fprintf(f,"%c",NUME_PARTITIE[j]);
                    }
                    fseek(f,(int)ADRESE[i]+TOTAL_SPACE[i]+12,SEEK_SET);
                    fputc(MAX_D,f);
                    fseek(f,(int)ADRESE[i]+TOTAL_SPACE[i]+16,SEEK_SET);
                    fputc(MAX_F,f);
                    break;
                }
                else
                    printf("INVALID\n");
          }
      }
}
void FORMAT_PARTITION(FILE *f,char NUME_PARTITIE[8])
{
     int i,j,ok;
     char NUMAR_PARTITII,**NUME_PARTITII,ADRESA_SFARSIT_FISIER;
     unsigned char *ADRESE,c;
     fseek(f,0,SEEK_SET);     //Setez cursorul la inceputul fisierului.
     NUMAR_PARTITII=fgetc(f);      //Aflam din fisier numarul de partitii, acest numar fiind la inceputul fisierului.
     ADRESE=malloc(NUMAR_PARTITII*sizeof(char));  //Aloc dinamic memorie pentru fiecare adresa la care se gaseste cate o partitie.
     NUME_PARTITII=malloc(NUMAR_PARTITII*sizeof(char));
     for(i=0;i<NUMAR_PARTITII;i++)      //In acest 'for' aflu adresele de inceput ale fiecarei partitii.
     {
          c=fgetc(f);
          while(c==0)
          {
               c=fgetc(f);
          }
          ADRESE[i]=c;
     }
     for(i=0;i<NUMAR_PARTITII;i++)
     {
        NUME_PARTITII[i]=malloc(8*sizeof(char));  //Aloc dinamic memorie pentru numele fiecarei partitii.
     }
     for(i=0;i<NUMAR_PARTITII;i++)
     {
        fseek(f,0,SEEK_SET);
        fseek(f,ADRESE[i]+4,SEEK_CUR);
        for(j=0;j<8;j++)
        {
            NUME_PARTITII[i][j]=fgetc(f);    //Retin numele tuturor partitiilor.
        }
     }
     for(i=0;i<NUMAR_PARTITII;i++)
     {
        for(j=0;j<8;j++) //Prin aceste 2 for-uri verific daca partitia introdusa exista in fisierul binar.
        {
            if(NUME_PARTITII[i][j]==NUME_PARTITIE[j])
            {
                ok=1;              
            }
            if(NUME_PARTITII[i][j]!=NUME_PARTITIE[j])
            {
                ok=0;
            }
        }
        if(ok==1)   //Daca am gasit partitia, opresc cautarea.
            break;
     }
     if(ok==0)
        printf("INVALID\n");  //Daca nu, opresc programul.
     if(ok==1)
     {
        printf("SUCCES\n");   //Afisez mesajul de succes si incep formatarea.
        for(i=0;i<NUMAR_PARTITII;i++)
        {
            fseek(f,0,SEEK_SET);
            fseek(f,ADRESE[i]+20,SEEK_CUR);
            if(i+1<NUMAR_PARTITII)
            {
                while(ftell(f)<ADRESE[i+1])
                {
                    fputc(0,f);
                }
            }
            if(i==NUMAR_PARTITII-1)
            {
                fseek(f,0,SEEK_END);
                ADRESA_SFARSIT_FISIER=ftell(f);
                fseek(f,0,SEEK_SET);
                fseek(f,ADRESE[i]+20,SEEK_CUR);
                while(ftell(f)<ADRESA_SFARSIT_FISIER)
                {
                    fputc(0,f);
                }
            }
        }
     }
}
void VERIFICARE_MBR(FILE *f)
{
     int i,j,MAXD,MAXF,TOTAL_SPACE,USED_SPACE,NR_DIR,NR_FIS;
     char NUME_PARTITII[]="/dev/sd",NUMAR_PARTITII,c;
     unsigned char *ADRESE;
     fseek(f,0,SEEK_SET);     //Setez cursorul la inceputul fisierului.
     NUMAR_PARTITII=fgetc(f);      //Aflam din fisier numarul de partitii, acest numar fiind la inceputul fisierului.
     ADRESE=malloc(NUMAR_PARTITII*sizeof(char));  //Aloc dinamic memorie pentru fiecare adresa la care se gaseste cate o partitie.
     for(i=0;i<NUMAR_PARTITII;i++)      //In acest 'for' aflu adresele de inceput ale fiecarei partitii.
     {
          c=fgetc(f);
          while(c==0)
          {
               c=fgetc(f);
          }
          ADRESE[i]=c;
     }
     for(i=0;i<NUMAR_PARTITII;i++)      //Verific daca fisierul binar are adresele scrise corect.
     {
          fseek(f,0,SEEK_SET);
          fseek(f,ADRESE[i]+4,SEEK_CUR);     //Setez cursorul la 4 pozitii dupa adresa de inceput a unei partitii. Acolo se regaseste numele partitiei.
          for(j=0;j<7;j++)
          {
               if((c=fgetc(f)) != NUME_PARTITII[j])    //Compar numele pe care il gasesc in fisier cu numele '/dev/sd'.

               {
                    printf("INVALID\n");
                    exit(EXIT_FAILURE); //Inchei programul in cazul in care se gasesc diferente cu mesaj de eroare.
               }
          }
     }
     printf("SUCCES\n");      //In cazul in care nu exista diferente de nume, afisez 'SUCCES'.
     for(i=0;i<NUMAR_PARTITII;i++)      //Aflu used_space, total_space si numele partitiei pentru fiecare partitie in parte.
     {
          TOTAL_SPACE=0;
          USED_SPACE=0;
          fseek(f,0,SEEK_SET);
          fseek(f,ADRESE[i]+4,SEEK_CUR);         //Mut cursorul la inceputul numelui partitiei.
          for(j=0;j<=7;j++)
          {
               c=fgetc(f);
               putc(c,stdout);     //Afisez numele partitiei.
          }
          fseek(f,0,SEEK_SET);
          fseek(f,ADRESE[i]+12,SEEK_CUR);    //Mut cursorul in locul unde gasesc 'MAXD' in fisierul binar.
          MAXD=fgetc(f); //Retin 'MAXD'.
          fseek(f,0,SEEK_SET);
          fseek(f,ADRESE[i]+16,SEEK_CUR);    //Mut cursorul in locul unde gasesc 'MAXF" in fisierul binar.
          MAXF=fgetc(f); //Retin 'MAXF'.
          TOTAL_SPACE=MAXF*12+MAXD*12+MAXD*MAXF*12;    //Calculez total_space. Aceasta variabila isi va pierde informatia dupa ce trec la urmatoarea partitie.
          NR_DIR=0; //Initializez numarul directoarelor cu '0'.
          NR_FIS=0; //Initializez numarul fisierelor cu '0'.
            if(MAXF != 0 )         //Aflu numarul fisierelor si numarul directoarelor.
            {
                fseek(f,0,SEEK_SET);
                fseek(f,ADRESE[i]+20,SEEK_CUR);       //Mut cursorul in locul in care incep informatiile despre directoare.
                for(j=1;j<=MAXD;j++)
                {
                c=fgetc(f);   //Retin numarul de caractere al numelui directorului.
                if(c!=0)      //In cazul in care este diferit de '0', inseamna ca exista un fisier.
                    NR_DIR++; //In consecinta, cresc numarul de directoare.
                fseek(f,0,SEEK_SET);
                fseek(f,ADRESE[i]+20+(12+(12*MAXF))*j,SEEK_CUR); //Mut cursorul la urmatoarea aparitie a informatiei despre director.
                }
                fseek(f,0,SEEK_SET);
                fseek(f,ADRESE[i]+20+12,SEEK_CUR);     //Mut cursorul in pozitia in care se regasesc informatii despre fisiere din directoare.
                for(j=1;j<=MAXF;j++)
                {
                    c=fgetc(f);    //Variabila 'c' retine numarul de caractere al numelui fisierului.
                    if(c!=0)       //Daca variabila 'c' este diferita de '0', inseamna ca exita fisierul, in consecinta cresc numarul de fisiere.
                        NR_FIS++;
                    fseek(f,0,SEEK_SET);
                    fseek(f,ADRESE[i]+20+(12*j),SEEK_CUR);  //Mut cursorul la urmatoarea aparitie a informatiilor despre fisiere din directoare.
                }
            }
          USED_SPACE=NR_FIS*12+NR_DIR*12+NR_DIR*NR_FIS*12;  //Aflu 'used_space' pentru partitia respectiva.
          printf(" %d %d",TOTAL_SPACE,USED_SPACE);    //Afisez 'total_space' si 'used_space'.
          printf("\n");
     }
}
void DELETE_PARTITION(FILE *f,char *comanda)
{
     int i,j,ok,NUMAR_PARTITII_MBR;
     char NUMAR_PARTITII,c,**NUME_PARTITII;
     unsigned char *ADRESE,d;
     fseek(f,0,SEEK_SET);     //Setez cursorul la inceputul fisierului.
     NUMAR_PARTITII=fgetc(f);      //Aflam din fisier numarul de partitii, acest numar fiind la inceputul fisierului.
     ADRESE=malloc(NUMAR_PARTITII*sizeof(char));  //Aloc dinamic memorie pentru fiecare adresa la care se gaseste cate o partitie.
     NUME_PARTITII=malloc(NUMAR_PARTITII*sizeof(char));
     for(i=0;i<NUMAR_PARTITII;i++)      //In acest 'for' aflu adresele de inceput ale fiecarei partitii.
     {
          c=fgetc(f);
          while(c==0)
          {
               c=fgetc(f);
          }
          ADRESE[i]=c;
     }
     for(i=0;i<NUMAR_PARTITII;i++)
     {
        NUME_PARTITII[i]=malloc(8*sizeof(char));  //Aloc dinamic memorie pentru numele tuturor partitiilor.
     }
     for(i=0;i<NUMAR_PARTITII;i++)
     {
        fseek(f,0,SEEK_SET);
        fseek(f,ADRESE[i]+4,SEEK_CUR);
        for(j=0;j<8;j++)
        {
            NUME_PARTITII[i][j]=fgetc(f);
        }
     }

     for(i=0;i<NUMAR_PARTITII;i++)
     {
        for(j=0;j<8;j++)
        {
            if(NUME_PARTITII[i][j]==comanda[j])
            {
                ok=1;
            }
            if(NUME_PARTITII[i][j]!=comanda[j])
            {
                ok=0;
            }
        }
        if(ok==1)
            break;
     }
     if(ok==1)
        {
            fseek(f,0,SEEK_SET);
            while(1)
            {
                d=fgetc(f);
                if(d==ADRESE[i])
                {
                    ungetc(d,f);
                    fputc(0,f);
                    fseek(f,0,SEEK_SET);
                    NUMAR_PARTITII_MBR=fgetc(f);
                    NUMAR_PARTITII_MBR=NUMAR_PARTITII_MBR-1;
                    fseek(f,0,SEEK_SET);
                    fputc(NUMAR_PARTITII_MBR,f);
                    printf("SUCCES\n");
                    break;
                }
            }
        }
     if(ok==0)
        printf("INVALID\n");
}
int main(int argc, char *argv[])
{
     int i,MAXD,MAXF,FINAL_FISIER,ok=0,element;
     char argumente[4],*array[40],segment[3][16],nume_partitie[8],*token,nume_partitie_final[8];
     FILE *mbr,*op;     //Fisierul cu care vom lucra.
     argumente[0]=1;
     argumente[1]=2;
     argumente[2]=3;
     argumente[3]=4;
     if(atoi(argv[1])== argumente[0])
     {
        mbr=fopen("mbr.bin","rb"); //Deschidem fisierul cu drepturi de citire si specificam tipul fisierului.
        if(!mbr)  //Daca nu se poate deschide fisierul
        {
          perror("ERROR");    //Afisam mesajul 'ERROR' si motivul.
          exit(EXIT_FAILURE);    //Incheiem programul cu mesaj de eroare.
        }
        VERIFICARE_MBR(mbr);
     }
     if(atoi(argv[1]) == argumente[2])
     {
        mbr=fopen("mbr.bin","rb+");
        op=fopen("mbr.op","rt");
        if(!mbr)  //Daca nu se poate deschide fisierul
        {
          perror("ERROR MBR");    //Afisam mesajul 'ERROR' si motivul.
          exit(EXIT_FAILURE);    //Incheiem programul cu mesaj de eroare.
        }
        if(!op)  //Daca nu se poate deschide fisierul
        {
          perror("ERROR OP");    //Afisam mesajul 'ERROR' si motivul.
          exit(EXIT_FAILURE);    //Incheiem programul cu mesaj de eroare.
        }
        fseek(op,0,SEEK_END);
        FINAL_FISIER=ftell(op);
        fseek(op,0,SEEK_SET);
        while(1)
        {
            ok=0;
            fgets(array,30,op);
            token=strtok(array," ");
            for(i=0;i<16;i++)
            {
                segment[0][i]=token[i];
            }
            if(ftell(op)==FINAL_FISIER && strstr(segment[0],"DELETE_PARTITION"))
            {
                fseek(op,-9,SEEK_CUR);
                fscanf(op,"%s",&nume_partitie_final);
                ok=1;
            }
            if(ftell(op)==FINAL_FISIER && strstr(segment[0],"CREATE_PARTITION"))
            {
                fseek(op,-9,SEEK_CUR);
                fscanf(op,"%s",element);
                ok=1;
            }
            if(ftell(op)==FINAL_FISIER && strstr(segment[0],"FORMAT_PARTITION"))
            {
               fseek(op,-9,SEEK_CUR);
               fscanf(op,"%s",&nume_partitie_final);
               ok=1;
            }
            if(strstr(segment[0],"DELETE_PARTITION"))
            {
                if(ok==1)
                {
                    DELETE_PARTITION(mbr,nume_partitie_final);
                    break;
                }
                token=strtok(NULL,"\n");
                if(token==NULL)
                    break;
                for(i=0;i<8;i++)
                {
                    nume_partitie[i]=token[i];
                }
                DELETE_PARTITION(mbr,nume_partitie);
            }
            if(strstr(segment[0],"CREATE_PARTITION"))
            {
                token=strtok(NULL," ");
                if(token==NULL)
                    break;
                for(i=0;i<8;i++)
                {
                    nume_partitie[i]=token[i];
                }
                token=strtok(NULL," ");
                MAXD=atoi(token);
                fscanf(op,"%c",&token);
                //token=strtok(NULL,"\n");
                if(token==NULL)
                    break;
                if(ok==1)
                {
                    CREATE_PARTITION(mbr,nume_partitie,MAXD,element);
                    break;
                }
                MAXF=atoi(token);
                CREATE_PARTITION(mbr,nume_partitie,MAXD,MAXF);
            }
            if(strstr(segment[0],"FORMAT_PARTITION"))
            {
                token=strtok(NULL,"\n");
                if(token==NULL)
                {
                    break;
                }
                for(i=0;i<8;i++)
                {
                    nume_partitie[i]=token[i];
                }
                if(ok==1)
                {
                    FORMAT_PARTITION(mbr,nume_partitie_final);
                    break;
                }
                FORMAT_PARTITION(mbr,nume_partitie);
            }
        }
     }
     return 0;
}
