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

bool sigbreak;
void sig_handler(int signo){
	if(signo == SIGUSR1){
		sigbreak = true;
		printf("sygnał\n");
	}
}

int main (int argc,char *argv[])
{

    
    //config zawiera wszystko co jest potrzebne w pracy Demona(ścieżka docelowa, źródłowa, czas czekania)
    //czy praca rekurencyjna itd.
    //walidacjaDane zwróci nam obiekt jak przejdzie, z wartością walidacja równa true
    //w przeciwnym wypadku walidacja w tej strukturze ma wartość false
    //i przez to wiemy, że trzeba zwrócić

    struct Data config=WalidacjaDane(argc,argv);


    if(config.walidacja==false)
    {
        return -1;
    }



    //otwieramy dostęp do logu systemowego, będziemy tam zapisywać informacje
    openlog("Demon Synchronizuący Katalogi", LOG_PID , LOG_USER);

    

    //zamieniamy program w demona

    pid_t pid;
    int i;

    /* stwórz nowy proces */
    pid = fork ( );
    if (pid == -1)
    {
        errno=-1;
        //kiedys trzeba bedzie pozamieniac częśc printf na syslog jak jest w zadaniu
        //syslog(LOG_ERR, "Nie udało się stworzyć nowego procesu!");
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

    //sleep(config.timeDelay);
    
    //--------------------------------
    if(signal(SIGUSR1, sig_handler) == SIG_ERR){
	fprintf(stderr, "błąd sigusr1\n");
	exit(EXIT_FAILURE);
	}
    //do fora czas w sekundach, np i < 10 to 10 sekund
    sigbreak = false;
    for(i = 0; i < (config.timeDelay/1000); i++){
	defSleep(1);
	if(sigbreak == true)
		break;
    }
    
    
    
    
    
    
    
    
    
    

    //closelog();

    return 0;
}
