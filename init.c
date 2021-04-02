#include "data.h"
#include "funkcje.h"


/*Tutaj nastąpi walidacja danych, jeśli przejdzie zwracamy true, jeśli nie to false i kończymy działanie programu*/ 
bool walidacja(int argc,char *argv[])
{
    if(argc<3)
    {
        errno=-1;
        printf("Zbyt mała liczba argumentów!");
        return false;
    }
    if(isDirectoryAndExists(argv[1]) == false || isDirectoryAndExists(argv[2]) == false)
    {
        errno=-1;
        printf("Podane argumenty nie pasują do żadnej ścieżki");
        return false;
    }
    if(isTargetSubDirOfSrc(argv[1],argv[2]) == true || isSourceSubDirOfTarget(argv[1],argv[2]) == true)
    {
        errno=-1;
        printf("Jeden katalog jest podkatalogiem drugiego!");
        return false;
    }

    //jeśli argumenty to tylko nazwa ścieżek, to zwracamy true
    if(argc == 3)
    {
        return true;
    }

    for(int i=4;i<=argc;i++)
    {
        int length = strlen(argv[i]+1);
        char s;
        s=malloc(length);
        memcpy(s,argv[i],length);
        printf("%s",s);
        return false;
        if( strcmp(argv[i],"-S")==0 || strcmp(argv[i],"-s")==0)
        {
            printf("wchodzę tutaj");
            return false;
            //ktoś mógł nie podać rozmiaru tego, ale podał to jako parametr, przerywamy
            if(argc == i)
            {
                printf("Podano argument -R ale nie podano rozmiaru");
                errno=-1;
                return false;
            }

            char *size;
            memcpy(size,argv[i+1],strlen(argv[i+1]));

            //we have to check if size is a intiger number
            for(int j = 0;j<strlen(size);j++)
            {
                //0 na początku to nie liczba
                if(size[0] == 0)
                {
                    printf("Podana wielkość nie jest liczbą!");
                    errno=-1;
                    return false;
                }
                else if(!isdigit(size[j]))
                {
                    printf("Podana wielkość nie jest liczbą!");
                    errno=-1;
                    return false;
                }
            }
            i++;
        }
        else if(argv[i] == "-t" || argv[i] == "-T")
        {
            if(i==argc)
            {
                printf("Podane argument -t ale nie podano czasu");
                errno=-1;
                return false;
            }

            char *time;
            //trzeba to skopiować w ten sposób
            memcpy(time,argv[i+1],strlen(argv[i+1]));

            for(int j = 0;j<strlen(time);j++)
            {
                if(time[0]==0)
                {
                    printf("Podana wielkość nie jest liczbą!");
                    errno=-1;
                    return false;
                }
                else if(!isdigit(time[j]))
                {
                    printf("Podana wielkość nie jest liczbą!");
                    errno=-1;
                    return false;
                }
            }
            i++;
        }
    }
    return true;
}

//miałek taką wizję by ta metoda zwróciła nam dane startowe
struct Data GetStartData(int argc,char *argv[])
{
    struct Data data;





    return data;
}



