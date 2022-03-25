#include "Akinator.h"
#include "../My_Lib/My_Lib.h"

int main (int argc, char *argv[])
{
    OPEN_LOG_FILE;

    struct Node root = {};

    Tree_Constructor (&root);

    if (argc != 3)
        MY_ASSERT (false, "int argc", UNEXP_VAL, ERROR);
    
    if (Read_Tree_From_File (&root, argv[1]) == ERROR)
        return ERROR;

    Choose_Mode (&root, argv[1], argv[2]);

    Tree_Dump (&root, argv[2]);

    Tree_Destructor (&root);

    return 0;
}
