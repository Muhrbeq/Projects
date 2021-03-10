#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

//static void AskForName(void);

char name[20];
int amountOfQuestionsTest = 15;

void AskForName(void);
int MainGUI(void);
void PracticeGUI(void);
void TestGUI(void);
int ChooseCalculation(void);
void PracticeCalculation(int testScenario);
int TestCalculation(int testScenario);
int SaveFile(int testResult);
void DisplayBestResult();
void DisplayTestHistory();

int main()
{
    int mainChoice = 0;
    int calculationChoice = 0;
    int result;
    int testCorrectAnswer = 0;
    double resultOfTest = 0.0d;

    AskForName();

    printf("Welcome %s\r\n", name);

    do
    {
        /* Show the user the main GUI */
        mainChoice = MainGUI();

        /* If practice */
        if(mainChoice == 1)
        {
            do
            {
                PracticeGUI();
                /* Check which choice the user makes */
                calculationChoice = ChooseCalculation();
                if(calculationChoice != 4)
                {
                    /* Practice on that */
                    PracticeCalculation(calculationChoice);
                }
            }while(calculationChoice != 4);
        }
        /* If test */
        else if(mainChoice == 2)
        {
            do
            {
                TestGUI();
                /* Check which choice the user makes */
                calculationChoice = ChooseCalculation();
                if(calculationChoice != 4)
                {
                    /* Test on that */
                    testCorrectAnswer = TestCalculation(calculationChoice);
                    resultOfTest = 100.0 * testCorrectAnswer/amountOfQuestionsTest;
                    printf("Result of test: %f percent!\r\n", resultOfTest);
                    SaveFile(testCorrectAnswer);
                }
            }while(calculationChoice != 4);
        }
        else if(mainChoice == 3)
        {
            DisplayTestHistory();
        }
        else if(mainChoice == 4)
        {
            DisplayBestResult();
        }
    /* If quit */
    }while(mainChoice != 5);


    return 0;
}

void AskForName(void)
{
    printf("Enter your name: ");

    fgets(name, 20, stdin);

    strtok(name, "\n");
}

int MainGUI(void)
{
    int choice;

    printf("You can choose:\r\n");

    printf("\t1. Do practices\r\n"
           "\t2. Complete a test\r\n"
           "\t3. Display all results\r\n"
           "\t4. Display the best result\r\n"
           "\t5. Quit the program\r\n");

    do
    {
        printf("Enter your choice between 1-5: ");
        scanf("%d", &choice);
    }while(choice < 1 || choice > 5);

    return choice;
}

void PracticeGUI(void)
{
    printf("Now you can choose to do practice on: \r\n");
}

void TestGUI(void)
{
    printf("Now you can choose what to do test on: \r\n");
}

int ChooseCalculation(void)
{
    int calcChoice = 0;
    printf("\t1. Addition\r\n"
           "\t2. Subtraction\r\n"
           "\t3. Addition and subtractions\r\n"
           "\t4. Go back\r\n");

    do
    {
        printf("Enter your choice between 1-4: ");
        scanf("%d", &calcChoice);
    }while(calcChoice < 1 || calcChoice > 4);

    return calcChoice;
}

void PracticeCalculation(int testScenario)
{
    /* Init variables */
    srand(time(NULL));
    bool correctAns;
    //char ans[4] = {'\0'};
    int ans;
    bool aBigger = true;
    int additionOrSubtraction = testScenario;

    /* Define how many correct answers necessary */
    int amountOfCorrectAnswerRequiredPractice = 10;

    int correctAnswers = 0;

    /* While not enough correct answers given */
    while(correctAnswers < amountOfCorrectAnswerRequiredPractice)
    {
        correctAns = false;
        int a = rand() % 100 + 1;
        int b = rand() % 100 + 1;

        if(testScenario != 3)
        {
            additionOrSubtraction = additionOrSubtraction + 2;
        }
        else
        {
            additionOrSubtraction++;
        }

        if(a > b)
        {
            aBigger = true;
        }
        else
        {
            aBigger = false;
        }

        do
        {
            if(additionOrSubtraction % 2 != 0)
            {
                if(a + b > 100)
                {
                    a = a/2;
                    b= b/2;
                }
                printf("How much is %d + %d? \t Answer: %d", b, a, b+a);
                //fgets(ans, 4, stdin);
                scanf("%d", &ans);

                //if(atoi(ans) == b + a)
                if(ans == b + a)
                {
                    /* Correct answer, add one to the answer counter */
                    printf("Correct, Very good!\r\n");
                    correctAnswers++;
                    correctAns = true;
                }
                else
                {
                    printf("Wrong, Try again\r\n");
                }
            }
            else
            {
                if(aBigger)
                {
                    printf("How much is %d - %d? \t Answer: %d", a, b, a-b);
                    //fgets(ans, 4, stdin);
                    scanf("%d", &ans);

                    //if(atoi(ans) == a - b)
                    if(ans == a - b)
                    {
                        /* Correct answer, add one to the answer counter */
                        printf("Correct, Very good!\r\n");
                        correctAnswers++;
                        correctAns = true;
                    }
                    else
                    {
                        printf("Wrong, Try again\r\n");
                    }
                }
                else
                {
                    printf("How much is %d - %d? \t Answer: %d", b, a, b-a);
                    //fgets(ans, 4, stdin);
                    scanf("%d", &ans);

                    //if(atoi(ans) == b - a)
                    if(ans == b - a)
                    {
                        /* Correct answer, add one to the answer counter */
                        printf("Correct, Very good!\r\n");
                        correctAnswers++;
                        correctAns = true;
                    }
                    else
                    {
                        printf("Wrong, Try again\r\n");
                    }
                }

            }
        /* Loop until correct answer given */
        }while(!correctAns);
    }
}

int TestCalculation(int testScenario)
{
    /* Init variables */
    srand(time(NULL));
    bool correctAns;
    //char ans[4] = {'\0'};
    int ans;
    bool aBigger = true;
    int additionOrSubtraction = testScenario;

    /* Define how many correct answers necessary */

    int currentGuess = 0;
    int correctAnswers = 0;

    /* While not enough correct answers given */
    while(currentGuess < amountOfQuestionsTest)
    {
        correctAns = false;
        int a = rand() % 100 + 1;
        int b = rand() % 100 + 1;

        if(testScenario != 3)
        {
            additionOrSubtraction = additionOrSubtraction + 2;
        }
        else
        {
            additionOrSubtraction++;
        }

        if(a > b)
        {
            aBigger = true;
        }
        else
        {
            aBigger = false;
        }

        if(additionOrSubtraction % 2 != 0)
        {
            if(a + b > 100)
            {
                a = a/2;
                b= b/2;
            }
            printf("How much is %d + %d? \t Answer: %d", b, a, b+a);
            //fgets(ans, 4, stdin);
            scanf("%d", &ans);

            if(ans == b + a)
            {
                /* Correct answer, add one to the answer counter */
                printf("Correct, Very good!\r\n");
                correctAnswers++;
            }
            else
            {
                printf("Wrong\r\n");
            }
        }
        else
        {
            if(aBigger)
            {
                printf("How much is %d - %d? \t Answer: %d", a, b, a-b);
                //fgets(ans, 4, stdin);
                scanf("%d", &ans);

                if(ans == a - b)
                {
                    /* Correct answer, add one to the answer counter */
                    printf("Correct, Very good!\r\n");
                    correctAnswers++;
                }
                else
                {
                    printf("Wrong\r\n");
                }
            }
            else
            {
                printf("How much is %d - %d? \t Answer: %d", b, a, b-a);
                //fgets(ans, 4, stdin);
                scanf("%d", &ans);
                if(ans == b - a)
                {
                    /* Correct answer, add one to the answer counter */
                    printf("Correct, Very good!\r\n");
                    correctAnswers++;
                }
                else
                {
                    printf("Wrong\r\n");
                }
            }

        }
        currentGuess++;
    }
    return correctAnswers;
}

int SaveFile(int testResult)
{
    printf("testResult in SaveFile: %d\r\n", testResult);
    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        char fileWay[256];
        sprintf(fileWay, "%s/resultHistory.txt", cwd);
        char inputToFile[32];
        sprintf(inputToFile, "%s:%d\n", name, testResult);

        FILE *fp;

        fp = fopen(fileWay, "a+");
        fputs(inputToFile, fp);
        fclose(fp);

        return 1;
    }
    else
    {
        printf("error");
        return 0;
    }

}

void DisplayTestHistory()
{
    FILE *fp;
    char str[50];
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        char fileWay[256];
        sprintf(fileWay, "%s/resultHistory.txt", cwd);

        fp = fopen(fileWay, "r");
        if(fp == NULL)
        {
            return;
        }
        printf("Name : Amount of correct answers\n");
        while(fgets(str, 50, fp) != NULL)
        {
            printf(str);
        }
    }
}

void DisplayBestResult()
{
    FILE *fp;
    char str[50];
    char cwd[256];
    int highestScore = 0;
    int highScore = 0;
    char nameOfHighScore[30];
    char *currentNameHighScore;

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        char fileWay[256];
        sprintf(fileWay, "%s/resultHistory.txt", cwd);

        fp = fopen(fileWay, "r");
        if(fp == NULL)
        {
            return;
        }
        while(fgets(str, 50, fp) != NULL)
        {
            currentNameHighScore = strtok(str, ":");
            // = tokenOfStr;
            highScore = atoi(strtok(NULL, ":"));
            if(highScore > highestScore)
            {
                highestScore = highScore;
                strcpy(nameOfHighScore, currentNameHighScore);
            }
        }

        printf("Highest number of correct answers: %d, made by : %s\n", highestScore, nameOfHighScore);
    }
}
