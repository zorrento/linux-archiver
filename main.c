
#include "header.h"

int main(int argc, char *argv[])
{

    if (strcmp(argv[1], "arch")==0)
    {
        int a = 0;
        printf("\n\tArchiving <%s> into the <%s>\n", argv[2], argv[3]);
        a = arch(argv[2], argv[3], 0);
        if(a==0)
            printf("\n\t<%s> is archived!\n\n", argv[2]);
    }
    else if (strcmp(argv[1], "unarch")==0)
    {
        int a = 0;
        printf("\n\tUnarchiving the <%s> into the <%s>\n", argv[2], argv[3]);
        a = unarch(argv[2], 0, argv[3]);
        if(a==0)
            printf("\n\t<%s> is unarchived!\n\n", argv[2]);
    }
    else
    {
        printf("Error. Unknown arguments\n");
    }

    return 0;
}