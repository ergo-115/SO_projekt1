#include "data.h"
//wszystkie ponizej potrzebne do kopiowania


//czy sciezka podana do target jest podkatalogiem source (czyli np.
// src = /home/....../projekt/abc i target = /home/...../projekt zwroci 1(true), inaczej 0

//czy sciezka w ogóle istnieje powinno byc sprawdzone wczesniej

//czy katalog docelowy jest podkatalogiem katalogu zrodlowego
bool isTargetSubDirOfSrc(const char* source,const char* target){

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
		free(absPath);
		return false;
	}

	
	//bedzie zwrocone false, jeśli będzie coś w stylu /home/...../abc/ zamiast /home/..../abc
	if(strcmp(absPath, path) != 0) {
		free(absPath);
		return false;
	}
	free(absPath);
	
	//sprawdzenie praw do execute, jesli brak to errno bedzie na eacces
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
int lenHelper(unsigned liczba) {
	if(liczba == 0) return 1;
    float helper = (float)liczba;
    int length=0;
    while(helper>=1)
    {
        length ++;
        helper /= 10;
    }
    return length;
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
//oczekiwanie x sekund
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




//kopiowanie pliku
void copy_file(char* source, char* dest, bool pom) //bool: jesli duzy to  mmap ; glowna funkcja kopiowania
{
   int sf = open(source, O_RDONLY); //otwieranie pliku
   int df;
   if(pom)
   {
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
	syslog(LOG_INFO,"Synchronizacja pliku o nazwie: %s metodą mmap, data: %s",source,asctime(tm));		
	size_t fsize = lseek(sf,0,SEEK_END);
	char *src=mmap(NULL,fsize, PROT_READ,MAP_PRIVATE,sf,0);
	df = open(dest, O_RDWR | O_CREAT,0666); //plik docelowy, prawa
	ftruncate(df,fsize);
	char *cel=mmap(NULL,fsize, PROT_READ | PROT_WRITE,MAP_SHARED,df,0);
	memcpy(cel,src,fsize);
	munmap(src,fsize);
	munmap(cel,fsize);
   }
   else
   {
	time_t t = time(NULL);//kopiowanie malych plikow
    	struct tm *tm = localtime(&t);
	syslog(LOG_INFO,"Synchronizacja pliku o nazwie: %s metodą read/write, data: %s",source,asctime(tm));
	char *buffer = malloc(sizeof(char) * 64);
	//size_t off = 0;
	size_t b_read;
	df = open(dest, O_RDWR | O_CREAT,0666);
	
	
	while(b_read = read (sf, buffer, sizeof(buffer)))
    {
        write (df, buffer,b_read);
    }
	
	free(buffer);
   }
	close(sf);
	close(df);
	
}	
//główna funkcja, slużąca do synchronizacji
int synchro(Data config){ 

    DIR *sourceFolder;
    DIR *destFolder; 
    struct dirent* dirent; //directory entry 
    dirent = malloc(sizeof(struct dirent));

    int fileOrDir=0; 

    mode_t perms;
	bool bLargeFile = false;

    struct stat srcStat;
    struct stat destStat;

    char pathSource[strlen(config.sourcePath)+20]; 
    char pathDest[strlen(config.destinationPath)+20];

	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

    if(sourceFolder = opendir(config.sourcePath)){ //kopiowanie
		while((dirent=readdir(sourceFolder)) != NULL){ //NULL oznacza koniec plików w folderze lub błąd 

			if( (strcmp(dirent->d_name, ".")==0) || (strcmp(dirent->d_name,"..")==0) ) continue; 

			strcpy(pathSource, config.sourcePath); 
			strcat(pathSource,"/"); 
			strcat(pathSource, dirent->d_name); 

			strcpy(pathDest, config.destinationPath); 
			strcat(pathDest, "/"); 
			strcat(pathDest, dirent->d_name);

			stat(pathSource, &srcStat);
			stat(pathDest, &destStat); 

			perms = getPerms(pathSource); 

			if (S_ISDIR(perms)) 
				fileOrDir = 0; //entry jest folderem 
			else if(S_ISREG(perms)) 
				fileOrDir = 1; //entry jest plikiem 
			else 
			fileOrDir = -1; 

			if(fileOrDir==0){  //jesli sciezka wskazuje folder 

				if(config.RecursiveMode==true){ 
					Data newConfig;
					newConfig = config;
					newConfig.sourcePath = pathSource;
					newConfig.destinationPath = pathDest;
					if(lstat(pathDest, &destStat)!=0){ //folder nie istnieje w dest
						mkdir(pathDest, perms); //nwm czy tak moze byc
					}
					t = time(NULL);
					tm = localtime(&t);
					syslog(LOG_INFO,"Synchronizacja podkatalogu o nazwie: %s, data: %s",newConfig.sourcePath,asctime(tm)); //dirent->d_name
					synchro(newConfig);
				} 
				continue; 
			} 
			else if (fileOrDir == 1){  //jesli sciezka wskazuje zwykly plik 
				if(srcStat.st_size<config.size)
					bLargeFile = false;
				else
					bLargeFile = true;

            	if(lstat(pathDest, &destStat)!=0){ //plik nie istnieje w dest
				
               		copy_file(pathSource, pathDest, bLargeFile);
            	}
            	else if(getTime(pathSource)>getTime(pathDest)){ //plik istnieje, ale data modyfikacji jest starsza
					copy_file(pathSource, pathDest, bLargeFile);
            	}
        	}
		}
		closedir(sourceFolder);
	}
	
	if(destFolder = opendir(config.destinationPath)){ //usuwanie
		while((dirent=readdir(destFolder)) != NULL){ //NULL oznacza koniec plików w folderze lub błąd 
        	if( (strcmp(dirent->d_name, ".")==0) || (strcmp(dirent->d_name,"..")==0) ) continue; 

        	strcpy(pathSource, config.sourcePath); 
        	strcat(pathSource,"/"); 
        	strcat(pathSource, dirent->d_name); 

        	strcpy(pathDest, config.destinationPath); 
        	strcat(pathDest, "/"); 
        	strcat(pathDest, dirent->d_name);

        	stat(pathSource, &srcStat);
        	stat(pathDest, &destStat); 

       		perms = getPerms(pathDest); 

        	if (S_ISDIR(perms)) 
            	fileOrDir = 0; //entry jest folderem 
        	else if(S_ISREG(perms)) 
            	fileOrDir = 1; //entry jest plikiem 
        	else 
        		fileOrDir = -1; 

        	if(fileOrDir==0){  //jesli sciezka wskazuje folder 
				Data newConfig;
				newConfig = config;
				newConfig.sourcePath = pathSource;
				newConfig.destinationPath = pathDest;
				if(lstat(pathSource, &srcStat)!=0){ //folderu nie ma w source, wiec go usuwam z jego plikami
					t = time(NULL);
					tm = localtime(&t);
					syslog(LOG_INFO,"Podkatalog nie znajduje się w folerze zrodlowym. Usuwanie podkatalogu: %s, data: %s",pathDest,asctime(tm)); //dirent->d_name
					synchro(newConfig);
					rmdir(pathDest); // usuwanie folderu pathDest
				}
				else
				if(config.RecursiveMode==true){ 
					synchro(newConfig);
				} 
            	continue; 
        	} 
			else if (fileOrDir == 1){  //jesli sciezka wskazuje zwykly plik 				
				if(lstat(pathSource, &srcStat)!=0){
					t = time(NULL);
					tm = localtime(&t);
					syslog(LOG_INFO,"Plik nie znajduje się w folerze zrodlowym. Usuwanie pliku: %s, data: %s",pathDest,asctime(tm)); //dirent->d_name
						
					if(remove(pathDest)==0){
						t = time(NULL);
						tm = localtime(&t);
						syslog(LOG_INFO,"Usunieto plik: %s, data: %s",dirent->d_name,asctime(tm)); //dirent->d_name
					}
					else{
						t = time(NULL);
						tm = localtime(&t);
						syslog(LOG_INFO,"Nie usunieto pliku: %s, data: %s",dirent->d_name,asctime(tm)); //dirent->d_name
					}
				}
			}
    	}
		closedir(destFolder);
	}
	return 0; 
} 
