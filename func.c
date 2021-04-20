#include "header.h"

int arch(char *FilePath, char *ArchiverPath, int depth)
{    
    int OpenArchive=open(ArchiverPath, O_CREAT|O_WRONLY|O_APPEND, ALLOW);

    if (OpenArchive==-1)
    {
        printf("\tCouldn't create or open <%s>\n", ArchiverPath);
        return OpenArchive;
    }

    DIR *Folder=opendir(FilePath);
    if(Folder==NULL)
    {
        printf("\tCouldn't open <%s>\n", FilePath);
        return -1;
    }


    chdir(FilePath);
    struct stat type;
    struct dirent *dir_ptr=readdir(Folder);
    while(dir_ptr!=NULL)
    {
        if (strcmp((*dir_ptr).d_name, ".") != 0 && strcmp((*dir_ptr).d_name, "..") != 0)
        {
            lstat((*dir_ptr).d_name, &type); //getting info about content of directory
            if (!S_ISDIR(type.st_mode))
            {
                arch_file((*dir_ptr).d_name, OpenArchive, depth, ""); //packing file
                                             //
            }
            else
            {   
                arch((*dir_ptr).d_name, "temporary.zipper", depth+1);             //
                arch_file("temporary.zipper", OpenArchive, depth+1, (*dir_ptr).d_name);   // packing folder + it's content
                remove("temporary.zipper"); 
            }
        }
        dir_ptr=readdir(Folder);

    }
    chdir(".."); //getting to the upper level
    closedir(Folder);
    close(OpenArchive);
    return 0;
}

int arch_file(char *FilePath, int OpenArchive, int depth, char *FolderName)
{
    int size=0;
    int OpenFile=open(FilePath, O_RDONLY);
    struct stat SizeOfFile;
    lstat(FilePath, &SizeOfFile);

    size = SizeOfFile.st_size;

    if (OpenFile==-1)
    {
        printf("\tCouldn't open the file\n");
        return OpenFile;
    }

    if (OpenArchive==-1)
    {
        printf("\tCouldn't create or open the archive\n");
        return OpenArchive;
    }

    if (strcmp("", FolderName)==0)
    {  
        if (write(OpenArchive, FilePath, MAXFILENAME)==-1)
            printf("\tError archiving.\n");
    }
    else
    {
        if(write(OpenArchive, FolderName, MAXFILENAME)==-1)
            printf("\tError archiving.\n");
    }


    write(OpenArchive, &depth, sizeof(int));    //infomation about how deep file was held
    write(OpenArchive, &size, sizeof(int));     //writing information about the file

    char buff;
    
    while(size)
    {
        read(OpenFile, &buff, sizeof(char));
        write(OpenArchive, &buff, sizeof(char));
        --size;
    }
    close(OpenFile);
    return 0;
}

int unarch_file(char *FileName, int size, int ArchiveDescr)
{
   int OpenNewFile=open(FileName, O_CREAT|O_WRONLY, ALLOW);     //creating file after unpacking info about it
   char buff;
        while(size)
        {
            read(ArchiveDescr, &buff, sizeof(char));
            write(OpenNewFile, &buff, sizeof(char));
            --size;
        }
        close(OpenNewFile);
    return 0;
}

int unarch(char *ArchivePath, int depth, char *ZipFolder)
{
    int ArchiveOpen=open(ArchivePath, O_RDONLY);

    if (ArchiveOpen==-1)
    {
        printf("\tCouldn't open the archive down the path!\n");
        return ArchiveOpen;
    }

    DIR *Folder=opendir(ZipFolder);    

    if (Folder == NULL)
    {
        mkdir(ZipFolder, S_IRWXO|S_IRWXU|S_IRWXG);      //making folder in case it was not found or created before
    }

    closedir(Folder);

    chdir(ZipFolder);

    int size;
    int depthfinder;
    char FileName[MAXFILENAME];

    while(read(ArchiveOpen, FileName, MAXFILENAME))                 //unpacking cycle
    {
        read(ArchiveOpen, &depthfinder, sizeof(int));
        read(ArchiveOpen, &size, sizeof(int)); 
        if (depthfinder==depth+1)
        {
            unarch_file("unzippertemp.zipper", size, ArchiveOpen);
            unarch("unzippertemp.zipper", depth+1, FileName);
            remove("unzippertemp.zipper");
        }
        else if (depth==depthfinder)
            unarch_file(FileName, size, ArchiveOpen);                    //unpacking files, which were laying in the root of a folder
        else
        {
            printf("\tUnarchiving error\n"); 
            return -1;
        }    
    }
    close(ArchiveOpen);
    chdir("..");  //getting to the upper level
    remove(ArchivePath);
    return 0;
}