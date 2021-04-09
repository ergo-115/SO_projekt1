#ifndef FUNKCJE_H_INCLUDED 
#define FUNKCJE_H_INCLUDED


bool isTargetSubDirOfSrc(const char* source,const char* target);
bool isSourceSubDirOfTarget(const char* source,const char* target);
bool isDirectoryAndExists(const char* path);
int lenHelper(unsigned x);
off_t getSize(char* path);
time_t getTime(char* path);
mode_t getPerms(char* path);
void defSleep(int seconds);

#endif 
