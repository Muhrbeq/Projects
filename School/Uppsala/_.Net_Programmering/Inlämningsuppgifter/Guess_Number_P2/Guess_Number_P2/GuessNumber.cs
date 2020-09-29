using System;
using System.Collections.Generic;
using System.Text;

namespace Guess_Number_P2
{
    class GuessNumber
    {
        private bool runGame;
        private Score gameScore;
        private HighScore scoreList;

        private string uName = "";

        public GuessNumber()
        {
            gameScore = new Score();
            scoreList = new HighScore();
        }

        public void PlayGame(string userAnswer)
        {
            /* Init variables */
            Random randomNumber = new Random();
            List<int> guessArray = new List<int>();
            List<Score> scoreArray = new List<Score>();

            if (WillPlay(userAnswer))
            {
                runGame = true;
                /* Init variables */
                int guessTheNumber = randomNumber.Next(1, 101);
                int amountOfGuesses = 0;
                int tempGuess = 0;

                while(runGame)
                {
                    Console.WriteLine("A random number has been generated between 1-100, please try to guess the number");
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
                    while (guessTheNumber == tempGuess)
                    {
                        //Make sure not the same number
                        guessTheNumber = randomNumber.Next(1, 101);
                    }                        

                    /* Ask the user if they want to continue playing */
                    Console.WriteLine("Do you want to continue playing? [y/n]");
                    runGame = WillPlay(Console.ReadLine());
                }
                
            }
            if (guessArray.Count > 0)
            {
                if (GetUserName())
                {
                    //Save run, foreach in guessarray save username + score
                    for(int i = 0; i < guessArray.Count; i++)
                    {
                        gameScore = new Score();
                        gameScore.Name = uName;
                        gameScore.Guess = guessArray[i];
                        scoreArray.Add(gameScore);
                    }
                    scoreList.SaveScore(scoreArray);
                }
            }
        }

        /// <summary>
        /// Evaluates if the user wants to play or not
        /// </summary>
        /// <param name="answer">y for yes, n for no</param>
        /// <returns>returns true if y, false if n</returns>
        public bool WillPlay(string answer)
        {
            if(answer == "y")
            {
                return true;
            }
            else if(answer == "n")
            {
                return false;
            }
            else
            {
                /* Tells the user to only answer y/n */
                Console.WriteLine("Please only write y or n");
                /* return a new answer until y/n given */
                return WillPlay(Console.ReadLine());
            }
        }

        /// <summary>
        /// Gets user name from console line
        /// </summary>
        /// <returns>returns true when username has been given</returns>
        private  bool GetUserName()
        {
            Console.Write("Please state your username: ");
            uName = Console.ReadLine();

            if(uName == String.Empty)
            {
                Console.WriteLine("Please use a user name");
                return GetUserName();
            }
            return true;
        }
    }
}
