using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace Guess_Number_P2
{
    class UI
    {
        private HighScore scoreList = new HighScore();
        private List<Score> printList = new List<Score>();

        public UI()
        {

        }

        public string DrawUI()
        {
            printList = scoreList.PrintScore();
            Console.WriteLine("Welcome to Guess the Number! \r\nHere you can see the previous guesses:");
            if((printList.Count == 0) || printList[0].Guess == 0)
            {
                Console.WriteLine("Noone has played before, be the first one on the List!");
            }
            else
            {               
                foreach (Score score in printList.OrderBy(x => x.Guess))
                {
                    Console.WriteLine(score.Name + ": " + score.Guess);
                }
            }

            Console.WriteLine("Do you want to play? [y/n]");

            return Console.ReadLine();
        }

        public void EndUI()
        {
            Console.WriteLine("Thank you for playing!");
        }
    }
}
