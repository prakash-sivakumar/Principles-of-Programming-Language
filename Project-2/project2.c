/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        printf("Error: missing argument\n");
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);

    // TODO: Read the input grammar at this point from standard input

    /*
       Hint: You can modify and use the lexer from previous project
       to read the input. Note that there are only 4 token types needed
       for reading the input in this project.

       WARNING: You will need to modify lexer.cc and lexer.h to only
       support the tokens needed for this project if you are going to
       use the lexer.
     */

    switch (task) {
        case 1:
            // TODO: perform task 1.
            break;

        case 2:
            // TODO: perform task 2.
            break;

        case 3:
            // TODO: perform task 3.
            break;

        case 4:
            // TODO: perform task 4.
            break;

        case 5:
            // TODO: perform task 5.
            break;

        default:
            printf("Error: unrecognized task number %d\n", task);
            break;
    }
    return 0;
}
