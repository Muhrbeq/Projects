using System;
using System.Collections;
using System.Collections.Generic;

namespace Guess_Number_P1
{
    class Program
    {
        static void Main(string[] args)
        {
            Random randomNumber = new Random();

            /* Init variables */
            int guessTheNumber = randomNumber.Next(1, 101);
            int amountOfGuesses = 0;
            int tempGuess = 0;
            int continuePlay = 1;
            List<int> guessArray = new List<int>();

            /* Welcome user */
            Console.WriteLine("Welcome to Guess number generator!");
            Console.WriteLine("A random number has been generated, please try to guess the number");

            /* If user wants to continue */
            while (continuePlay == 1)
            {
                while (tempGuess != guessTheNumber)
                {
                    /* Guess the number */
                    Console.Write("Guess the number: ");
                    tempGuess = int.Parse(Console.ReadLine());

                    amountOfGuesses++;

                    /* If correct, reset counter and add amount of guesses to array */
                    if (tempGuess == guessTheNumber)
                    {
                        Console.WriteLine("Correct Guess!");
                        Console.WriteLine("Amount of guesses to get it right: " + amountOfGuesses);
                        guessArray.Add(amountOfGuesses);
                        amountOfGuesses = 0;
                        
                    }
                    else
                    {
                        /* Wrong, state if too high or too low */
                        Console.WriteLine("Wrong Guess!");

                        if (tempGuess > guessTheNumber)
                        {
                            Console.WriteLine("Guess was too high!");
                        }
                        else
                        {
                            Console.WriteLine("Guess was too low!");
                        }

                        Console.WriteLine("Amount of guesses made: " + amountOfGuesses);
                    }
                }
                /* When correct guess has been made, do another random number */
                guessTheNumber = randomNumber.Next(1, 101);

                /* Ask the user if they want to continue playing */
                Console.WriteLine("Do you want to continue playing? 1 for Yes, 0 for no");
                continuePlay = int.Parse(Console.ReadLine());
            }
            
            /* Print the resulting guess array */
            for(int i = 0; i < guessArray.Count; i++)
            {
                Console.WriteLine("Run " + (i + 1) + ": Required " + guessArray[i] + " guesses!");
            }

        }
    }
}
