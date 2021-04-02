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




