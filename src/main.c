#include "../include/Akinator.h"
#include "My_Lib.h"

int Check_Argc (const int argc, const int expected)
{
    return (argc == expected) ? 0 : 1;
}

int main (int argc, char *argv[])
{
    Open_Log_File ("Akinator");

    MY_ASSERT (Check_Argc (argc, 2) == 0, "Check_Args ()", FUNC_ERROR, ERROR);

    MY_ASSERT (Akinator (argv[1]) != ERROR, "Akinator ()", FUNC_ERROR, ERROR);

    return 0;
}
