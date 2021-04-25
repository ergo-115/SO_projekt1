#include "data.h"
//wszystkie ponizej potrzebne do kopiowania


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
void create_file(char* p) //tworzy plik docelowy
{
	mode_t rights = S_IRUSR | S_IWUSR | S_IRGRP |S_IWGRP | S_IROTH;
	//prawa: czytanie i pisanie dla wlasciciela, czytanie i pisanie dla grupy, czytanie dla pozostalych
	int f = open(p, O_WRONLY | O_EXCL | O_CREAT, rights); 
	if(f== -1) exit(EXIT_FAILURE);
	close(f);
}

ssize_t write_file(int fd, const void* buffer, size_t scale)
{
	size_t left = scale;
        while(left > 0){
                size_t written = write(fd,buffer,scale);
                if(written == -1) return -1;
                else left -=written;
        }
        assert (left == 0);
        return scale; //zwraca zapisany rozmiar
}

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
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
	syslog(LOG_INFO,"Synchronizacja pliku o nazwie: %s metodą read/write, data: %s",source,asctime(tm));
	unsigned char buffer[15];
	size_t off = 0;
	size_t b_read;
	create_file(dest); //docelowy
	df = open(dest, O_RDWR | O_CREAT,0666);
	do{
		b_read = read(sf,buffer,sizeof(buffer));
		write_file(df,buffer,b_read); //czyta wartosc pliku
		off += b_read;
	}while(b_read==sizeof(buffer));
   }
	close(sf);
	close(df);
}	

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
