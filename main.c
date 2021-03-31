#include "data.h"
#include "funkcje.h"
#include "init.h"
//#include "data.h"
//struct Data config;

/*Programik otrzymuje co najmniej 2 argumenty, ścieżkę źródłową oraz ścieżkę docelową .
 Jeżeli któraś ze ścieżek nie jest katalogiem program powraca natychmiast z komunikatem błędu.*/

//argumenty przekazywane do maina: -t czas (gdzie czas to czas w sekundach)
//-R czy odpalamy rekurencyjnie, czyli czy patrzymy też w podkatalogi
//-S size (gdzie size to próg dzielący male pliki od dużych)



int main (int argc,char *argv[])
{
    if(walidacja(argc,argv) == false)
    {
        return -1;
    }

    //config zawiera wszystko co jest potrzebne w pracy Demona(ścieżka docelowa, źródłowa, czas czekania)
    //czy praca rekurencyjna itd.
    struct Data config;

    config=GetStartData(argc,argv);


    //otwieramy dostęp do logu systemowego, będziemy tam zapisywać informacje
    openlog("Demon Synchronizuący Katalogi", 0, LOG_USER);


    //przypisuję do zmiennej sourcePath pierwszy argument który jest ścieżką źródłową
    //a do zmiennej destinationPath drugi argument programu, który jest ścieżką docelową

    //otrzymaliśmy tylko 2 argumenty - ścieżkę źródłową i ścieżkę docelową
    char *sourcePath, *destinationPath;
    memcpy(sourcePath,argv[1],strlen(argv[1]));
    memcpy(destinationPath,argv[2],strlen(argv[2]));
    //recursive mode równa się true, jeśli podamy mu taki argument
    //wtedy program działa rekurencyjnie

    bool recursiveMode = false;

    //differenceSize służy do określenia różnicy między małymi a dużymi plikami
    int differenceSize = 0;

    int timeDelay=0;

    
    for(int i=3;i<=argc;i++)
    {
        if( strcmp(argv[i],"-R") == 0 || strcmp(argv[i],"-r") == 0)
        {
            if(recursiveMode == true)
            {
                printf("Podano ponownie ten sam argument");
                errno=-1;
                return-1;
            }
            recursiveMode = true;
        }
        else if( strcmp(argv[i],"-S")==0 || strcmp(argv[i],"-s")==0)
        {
            //ktoś mógł nie podać rozmiaru tego, ale podał to jako parametr, przerywamy
            if(argc == i)
            {
                printf("Podano argument -R ale nie podano rozmiaru");
                errno=-1;
                return-1;
            }
            if(differenceSize != 0)
            {
                printf("Podano ponownie ten sam argument");
                errno=-1;
                return-1;
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
                    return-1;
                }
                else if(!isdigit(size[j]))
                {
                    printf("Podana wielkość nie jest liczbą!");
                    errno=-1;
                    return-1;
                }
            }
            differenceSize = atoi(size);
        }
        else if(argv[i] == "-t" || argv[i] == "-T")
        {
            if(i==argc)
            {
                printf("Podane argument -t ale nie podano czasu");
                errno=-1;
                return -1;
            }
            char *time;
            memcpy(time,argv[i+1],strlen(argv[i+1]));


            for(int j = 0;j<strlen(time);j++)
            {
                if(time[0]==0)
                {
                    printf("Podana wielkość nie jest liczbą!");
                    errno=-1;
                    return-1;
                }
                else if(!isdigit(time[j]))
                {
                    printf("Podana wielkość nie jest liczbą!");
                    errno=-1;
                    return-1;
                }
            }
            timeDelay = atoi(time);

        }
    }

    //jeśli nie zmieniono czasu oczekiwania
    if(timeDelay==0)
    {
        timeDelay = 5000;
    }

    

    //zamieniamy program w demona

    pid_t pid;
    int i;

    /* stwórz nowy proces */
    pid = fork ( );
    if (pid == -1)
    {
        errno=-1;
        printf("Nie udało się stworzyć nowego procesu!");
        return -1;
    }
    else if (pid != 0)
        exit (EXIT_SUCCESS);

    /* stwórz nową sesję i grupę procesów */
    if (setsid ( ) == -1)
    {
        errno=-1;
        printf("Nie udało się stworzyć nowej sesji i/lub grupy procesów!");
        return -1;
    }
    /* ustaw katalog roboczy na katalog główny */
    if (chdir ("/") == -1)
    {
        errno=-1;
        printf("Nie udało się ustawić katalogu roboczego na katalog główny!");
        return -1;
    }
    /* zamknij wszystkie pliki otwarte - użycie opcji NR_OPEN to przesada, lecz działa
    te nr_open to maxymalna liczba przechowywanych deskryptorów plików, to znaczy 1024*1024 */
    for (i = 0; i < 1024*1024; i++)
        close (i);

    /* przeadresuj deskryptory plików 0, 1, 2 na /dev/null */
    open ("/dev/null", O_RDWR); /* stdin */
    dup (0); /* stdout */
    dup (0); /* stderror */

    /*Program nasz stał się demonem, można tutaj wykonać jego czynności, poniżej należy wykonać czynności demona... */

    sleep(timeDelay);

    return 0;
}
