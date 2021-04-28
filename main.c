#include "data.h"
#include "funkcje.h"
#include "init.h"
#include <syslog.h>
#include <time.h>

//#include "data.h"
//struct Data config;

/*Programik otrzymuje co najmniej 2 argumenty, ścieżkę źródłową oraz ścieżkę docelową .
 Jeżeli któraś ze ścieżek nie jest katalogiem program powraca natychmiast z komunikatem błędu.*/

//argumenty przekazywane do maina: -t czas (gdzie czas to czas w sekundach)
//-R czy odpalamy rekurencyjnie, czyli czy patrzymy też w podkatalogi
//-S size (gdzie size to próg dzielący male pliki od dużych)

bool sigbreak;
void sig_handler(int signo) {
    if(signo == SIGUSR1) {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        syslog(LOG_INFO,"Obudzenie sie demona, data: %s",asctime(tm));
        sigbreak = true;
    }
}


int main (int argc,char *argv[])
{


    //config zawiera wszystko co jest potrzebne w pracy Demona(ścieżka docelowa, źródłowa, czas czekania)
    //czy praca rekurencyjna itd.
    //walidacjaDane zwróci nam obiekt jak przejdzie, z wartością walidacja równa true
    //w przeciwnym wypadku walidacja w tej strukturze ma wartość false
    //i przez to wiemy, że trzeba zwrócić

    struct Data config=validateData(argc,argv);


    if(config.validation==false)
    {
        exit(EXIT_FAILURE);
    }



    //otwieramy dostęp do logu systemowego, będziemy tam zapisywać informacje
    openlog("Demon Synchronizuący Katalogi", LOG_PID, LOG_USER);


    //zamieniamy program w demona

    pid_t pid;
    int i;

    /* stwórz nowy proces */
    pid = fork ( );
    if (pid == -1)
    {
        errno=-1;
        printf("Nie udało się stworzyć nowego procesu!");
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        syslog(LOG_INFO,"Nie udało się stworzyć nowego procesu demona, czas: %s",asctime(tm));
        closelog();
        exit(EXIT_FAILURE);
    }
    else if (pid != 0)
    {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        syslog(LOG_INFO,"Udalo sie stworzyc nowy proces dla demona, pid = %d , czas: %s", pid, asctime(tm));
        exit (EXIT_SUCCESS);
    }

    /* stwórz nową sesję i grupę procesów */
    if (setsid ( ) == -1)
    {
        errno=-1;
        printf("Nie udało się stworzyć nowej sesji i/lub grupy procesów!");
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        syslog(LOG_INFO,"Nie udało się stworzyć nowej sesji i/lub grupy procesów, czas: %s",asctime(tm));
        closelog();
        exit(EXIT_FAILURE);
    }
    /* ustawianie katalogu roboczego na katalog główny */
    if (chdir ("/") == -1)
    {
        errno=-1;
        printf("Nie udało się ustawić katalogu roboczego na katalog główny!");
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        syslog(LOG_INFO,"Nie udało się ustawić katalogu roboczego na katalog główny, czas: %s",asctime(tm));
        closelog();
        exit(EXIT_FAILURE);
    }
    /* zamkniecie wszystkich plikow otwartych
    te nr_open to maxymalna liczba przechowywanych deskryptorów plików, to znaczy 1024*1024 */
    for (i = 0; i < 1024*1024; i++)
        close (i);

    /* przeadresowanie deskryptorow plików 0, 1, 2 na /dev/null */
    open ("/dev/null", O_RDWR); /* stdin */
    dup (0); /* stdout */
    dup (0); /* stderror */

    //Program nasz stał się demonem

    

    //--------------------------------
    if(signal(SIGUSR1, sig_handler) == SIG_ERR) {
        fprintf(stderr, "błąd sigusr1\n");
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        syslog(LOG_INFO,"Nie udało się obsłużyć SIGUSR1, czas: %s",asctime(tm));
        exit(EXIT_FAILURE);
        closelog();
    }
    //do fora czas w sekundach, np i < 10 to 10 sekund
    time_t t;
    struct tm *tm;
    while(1)
    {
        sigbreak = false;
        for(i = 0; i < (config.timeDelay); i++) 
            {
            defSleep(1);
            if(sigbreak == true)
                break;
            }
        t = time(NULL);
        tm = localtime(&t);
        syslog(LOG_INFO,"Demon rozpoczal swoja prace, data: %s",asctime(tm));

        if(synchro(config)==0)
            {
            t = time(NULL);
            tm = localtime(&t);
            syslog(LOG_INFO,"Sukces synchronizacji, data: %s",asctime(tm));
            }
        
    }
    

    
    t = time(NULL);
    tm = localtime(&t);
    syslog(LOG_INFO,"Demon zakonczyl swoja prace!, data: %s",asctime(tm));

    closelog();

    exit (EXIT_SUCCESS);
}
