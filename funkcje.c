#include "data.h"

//czy sciezka podana do target jest podkatalogiem source (czyli np.
// src = /home/....../projekt/abc i target = /home/...../projekt zwroci 1(true), inaczej 0

//czy sciezka w ogóle istnieje powinno byc sprawdzone wczesniej

//czy katalog docelowy jest podkatalogiem katalogu zrodlowego
bool isTargetSubDirOfSrc(const char* source,const char* target){
	//te 5 linijek raczej do wykasowania
	//char inPathSize[4096];
	//char outPathSize[4096];
	//char *absPathSource, *absPathTarget;
	//absPathSource = realpath(source, inPathSize);
	//absPathTarget = realpath(target, outPathSize);
	char* subDirSrc = strdup(source);
	dirname(subDirSrc);
	if(strcmp(subDirSrc, target) == 0){
		free(subDirSrc);
		return true;
	}
	free(subDirSrc);
	return false;
}
//czy katalog zrodlowy jest podkatalogiem docelowego
bool isSourceSubDirOfTarget(const char* source,const char* target){
	//to samo co wyzej
	//char inPathSize[4096];
	//char outPathSize[4096];
	//char *absPathSource, *absPathTarget;
	//absPathSource = realpath(source, inPathSize);
	//absPathTarget = realpath(target, outPathSize);
	char* subDirTarget = strdup(target);
	dirname(subDirTarget);
	if(strcmp(subDirTarget, source) == 0){
		free(subDirTarget);
		return true;
	}
	free(subDirTarget);
	return false;
}

//czy cos pod sciezka istnieje i czy jest to katalog, do wywolania na poczatku
bool isDirectoryAndExists(const char* path){
	//char buffer[1024];
	char *absPath = NULL;
	//te realpath z wartoscia NULL tworzy bufor, dlatego pozniej jest free()
	absPath = realpath(path, NULL);
	if(!absPath){
		//printf("sciezka nie istnieje");
		free(absPath);
		return false;
	}

	
	//wywali się tzn false, jeśli będzie coś w stylu /home/...../abc/ zamiast /home/..../abc
	if(strcmp(absPath, path) != 0) {
		free(absPath);
		return false;
	}
	free(absPath);
	
	//sprawdzenie praw do execute, jesli brak to errno bedzie na eacces
	//co ciekawe, dziala tylko dla folderow, i zwykle pliki automatycznie maja ustawiane errno na eacces
	//wiec te 4 linijki "pod czy jest katalogiem" można chyba by skasować, poki co zostawiam
	int perms = 0;
	perms = access(path, X_OK);
	if(errno == EACCES)
		return false;

	//czy jest katalogiem
	struct stat statt;
	stat(path, &statt);
	if(!(S_ISDIR(statt.st_mode)))
		return false;
	
	return true;
}
int lenHelper(unsigned x) {
    if (x >= 1000000000) return 10;
    if (x >= 100000000)  return 9;
    if (x >= 10000000)   return 8;
    if (x >= 1000000)    return 7;
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}
//na podstawie sciezki do pliku
//zwraca rozmiar pliku
off_t getSize(char* path){
    struct stat statS;
    if(stat(path, &statS) != 0)
        return -1;
    
    return statS.st_size;
}
//zwraca czas ostatniej modyfikacji danych w pliku
time_t getTime(char* path) 
{
    struct stat statT;
    if(stat(path, &statT) == -1)
        exit(EXIT_FAILURE);

    return statT.st_mtime;
}
//zwraca uprawnienia do pliku
mode_t getPerms(char* path)
{
    struct stat statP;
    if(stat(path, &statP)==-1)
        exit(EXIT_FAILURE);
        
    return statP.st_mode;
}

void defSleep(int seconds){
	int msec = seconds * 1000;

	long pause = msec * (CLOCKS_PER_SEC/1000);
	clock_t clkNow, clkNext;
	clkNow = clock();
	clkNext = clkNow;
	while((clkNow - clkNext) < pause){
		clkNow = clock();
	}
}

