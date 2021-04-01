#ifndef DATA_H_INCLUDED 
#define DATA_H_INCLUDED

#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fs.h>
#include <syslog.h>
#include <time.h>


 typedef struct Data
{
    bool ResursiveMode;
    int timeDelay;
    char *sourcePath;
    char *destinationPath;
} data;

#endif
