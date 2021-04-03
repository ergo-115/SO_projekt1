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
    
    for(int i=3;i<argc;i++)
    {
        
        if( strcmp(argv[i],"-S")==0 || strcmp(argv[i],"-s")==0)
        {
            
            //ktoś mógł nie podać rozmiaru tego, ale podał to jako parametr, przerywamy
            if(argc-1 == i)
            {
                printf("Podano argument -S ale nie podano rozmiaru");
                errno=-1;
                return false;
            }
            
            //0 na początku to nie liczba
                if(argv[i][0] == '0')
                {
                    printf("Podana wielkość przy parametrze -S nie jest liczbą!");
                    errno=-1;
                    return false;
                }

            //we have to check if size is a intiger number
            
                int liczba = atoi(argv[i+1]);
                if(lenHelper(liczba) != strlen(argv[i+1]))
                {
                    printf("Podana wielkość przy parametrze -S nie jest liczbą!");
                    errno=-1;
                    return false;
                }
            
            i++;
        }
        else if(strcmp(argv[i], "-t")==0 || strcmp(argv[i] , "-T")==0)
        {
            
//ktoś mógł nie podać rozmiaru tego, ale podał to jako parametr, przerywamy
            if(argc-1 == i)
            {
                printf("Podano argument -T ale nie podano rozmiaru");
                errno=-1;
                return false;
            }
            
            //0 na początku to nie liczba
                if(argv[i][0] == '0')
                {
                    printf("Podana wielkość przy parametrze -T nie jest liczbą!");
                    errno=-1;
                    return false;
                }

            //we have to check if size is a intiger number
            
                int liczba = atoi(argv[i+1]);
                if(lenHelper(liczba) != strlen(argv[i+1]))
                {
                    printf("Podana wielkość przy parametrze -T nie jest liczbą!");
                    errno=-1;
                    return false;
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



