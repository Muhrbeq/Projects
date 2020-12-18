#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char GetName(void);
bool ContinueProgram(void);

int main()
{
    bool continueProgram = true;
    char nameOfUser [20];

    printf("What is your name: \r\n");

    fgets(nameOfUser, 20, stdin);

    printf("Hello %s\r\n", nameOfUser);

    do
    {
        continueProgram = ContinueProgram();

    }while(continueProgram);


    return 0;
}

void MainMenu(void)
{
    printf("");
}

bool ContinueProgram(void)
{
    /* Do you want to continue the program? */
    char continueProg [10];

    printf("Do you want to continue? 1 for yes, 0 for no");

    fgets(continueProg, 10, stdin);
    if(atoi(continueProg) == 1)
    {
        return true;
    }
    else if(atoi(continueProg) == 0)
    {
        return false;
    }
    else
    {
        return ContinueProgram();
    }
}
