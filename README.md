# Linuxowy Demon synchronizujący dwa podkatalogi

	
## Opis:
Po włączeniu, demon zaczyna synchronizować podane katalogi, zweryfikuje poprawność podanych danych i zanotuje wszystko w logu systemowym.
Demon reaguje na sygnał SIGUSR1, po którego podaniu zostaje wybudzony i zaczyna synchronizację.

## Przyjmowane parametry:
- R / -r - wywołanie rekurencyjne
- T <czas> / -t <czas> - ustawienie czasu w sekundach, po których demon rozpocznie pracę
- S <rozmiar> / -s <rozmiar> - ustawienie rozmiaru w bajtach, który rozdziela małe pliki od większych plików, od czego zależy metoda kopiowania

## Sposób użycia:
./synchDemon <ścieżka_źródłowa> <ścieżka_docelowa> [-r/-R] [-t/-T czas_w_sekundach] [-s/-S rozmiar]

## Wysłanie sygnału:
kill -SIGUSR1 <pid> 
gdzie <pid> to numer procesu przydzielonego przy tworzeniu demona

## Przykłady:
./synchDemon <ścieżka_źródłowa> <ścieżka_docelowa> -R -T 25 -S 100
./synchDemon <ścieżka_źródłowa> <ścieżka_docelowa> -R
./synchDemon <ścieżka_źródłowa> <ścieżka_docelowa>
