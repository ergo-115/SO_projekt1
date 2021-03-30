#include <libgen.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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

//zawsze mozna pozbyc sie biblioteki stdbool gdyby zrobic cos w stylu 'int isSource... return 1 else return 0', ale ostatecznie na jedno wychodzi bo te wartosci system sam zamienia na typ int
//deklaracja w main w stylu bool isSourceSubDirOfTarget(const char* source,const char* target);
