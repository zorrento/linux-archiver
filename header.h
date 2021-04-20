#define ALLOW 00666 //code of restriction to open and write to folder
#define MAXFILENAME 50

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int arch(char *FilePath, char* ArchiverPath, int depth); 
int arch_file(char *FilePath, int OpenArchive, int depth, char *FolderName); 
int unarch_file(char *FileName, int size, int ArchiveDescr); 
int unarch(char *ArchivePath, int depth, char *ZipFolder); 