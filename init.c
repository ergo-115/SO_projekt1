#include "data.h"
#include "funkcje.h"


/*Tutaj nastąpi walidacja danych, jeśli przejdzie zwracamy true, jeśli nie to false i kończymy działanie programu*/ 
struct Data WalidacjaDane(int argc,char *argv[])
{
    struct Data data;
    data.sourcePath = argv[1];
    data.destinationPath = argv[2];
    data.size=1024*1024;
    data.timeDelay=60*5;
    data.RecursiveMode=false;
    data.walidacja = true;
    if(argc<3)
    {
        errno=-1;
        printf("Zbyt mała liczba argumentów!\n");
        data.walidacja = false;
        return data;
    }
    if(isDirectoryAndExists(argv[1]) == false || isDirectoryAndExists(argv[2]) == false)
    {
        errno=-1;
        printf("Podane argumenty nie pasują do żadnej ścieżki\n");
        data.walidacja = false;
        return data;
    }
    if(isTargetSubDirOfSrc(argv[1],argv[2]) == true || isSourceSubDirOfTarget(argv[1],argv[2]) == true)
    {
        errno=-1;
        printf("Jeden katalog jest podkatalogiem drugiego!\n");
        data.walidacja = false;
        return data;
    }

    //jeśli argumenty to tylko nazwa ścieżek, to zwracamy true
    if(argc == 3)
    {
        return data;
    }
    
    for(int i=3;i<argc;i++)
    {
        
        if( strcmp(argv[i],"-S")==0 || strcmp(argv[i],"-s")==0)
        {
            
            //ktoś mógł nie podać rozmiaru tego, ale podał to jako parametr, przerywamy
            if(argc-1 == i)
            {
                printf("Podano argument -S ale nie podano rozmiaru\n");
                errno=-1;
                data.walidacja = false;
                return data;
            }
            
            //0 na początku to nie liczba
                if(argv[i][0] == '0')
                {
                    printf("Podana wielkość przy parametrze -S nie jest liczbą!\n");
                    errno=-1;
                    data.walidacja = false;
                    return data;
                }

            //we have to check if size is a intiger number
            
                int liczba = atoi(argv[i+1]);
                if(lenHelper(liczba) != strlen(argv[i+1]))
                {
                    printf("Podana wielkość przy parametrze -S nie jest liczbą!\n");
                    errno=-1;
                    data.walidacja = false;
                    return data;
                }
                data.size = liczba;
            
            i++;
        }
        else if(strcmp(argv[i],"-r") == 0 || strcmp(argv[i],"-R") == 0)
        {
            data.RecursiveMode = true;
        }
        else if(strcmp(argv[i], "-t")==0 || strcmp(argv[i] , "-T")==0)
        {
            
//ktoś mógł nie podać rozmiaru tego, ale podał to jako parametr, przerywamy
            if(argc-1 == i)
            {
                printf("Podano argument -T ale nie podano rozmiaru\n");
                errno=-1;
                data.walidacja = false;
                return data;
            }
            
            //0 na początku to nie liczba
                if(argv[i][0] == '0')
                {
                    printf("Podana wielkość przy parametrze -T nie jest liczbą!\n");
                    errno=-1;
                    data.walidacja = false;
                    return data;
                }

            //we have to check if size is a intiger number
            
                int liczba = atoi(argv[i+1]);
                if(lenHelper(liczba) != strlen(argv[i+1]))
                {
                    printf("Podana wielkość przy parametrze -T nie jest liczbą!\n");
                    errno=-1;
                    data.walidacja = false;
                    return data;
                }
            data.timeDelay = liczba;
            i++;
        }
        
    }
    return data;
}



