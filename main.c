#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<linux/fs.h>
#include <errno.h>


/*Programik otrzymuje co najmniej 2 argumenty, ścieżkę źródłową oraz ścieżkę docelową .
 Jeżeli któraś ze ścieżek nie jest katalogiem program powraca natychmiast z komunikatem błędu.*/

//argumenty przekazywane do maina: -t czas (gdzie czas to czas w sekundach)
//-R czy odpalamy rekurencyjnie, czyli czy patrzymy też w podkatalogi
//-S size (gdzie size to próg dzielący male pliki od dużych)

int main (int argc,char *argv[])
{
    if(argc<3)
    {
        errno=-1;
        printf("Zbyt mała liczba argumentów!");
        return -1;
    }

    //przypisuję do zmiennej sourcePath pierwszy argument który jest ścieżką źródłową
    //a do zmiennej destinationPath drugi argument programu, który jest ścieżką docelową
    char sourcePath[]=argv[1], destinationPath=argv[2];

    //otrzymaliśmy tylko 2 argumenty - ścieżkę źródłową i ścieżkę docelową
    if(argc==3)
    {

    }

    //orzymaliśmy ścieżkę źródłową oraz ścieżkę docelową oraz parametr -R, mówiący że ma wszystko nastąpić reurencyjnie
    else if(argc==4)
    {

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

    //demon śpi przez 5 minut, a w delay() podajemy w argumencie milisekundy
    delay(1000*60*5);

    return 0;
}