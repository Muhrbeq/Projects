using System;

namespace Guess_Number_P2
{
    class Program
    {
        

        static void Main(string[] args)
        {
            UI userInt = new UI();
            GuessNumber gN = new GuessNumber();

            /* Play the game */
            gN.PlayGame(userInt.DrawUI());

            /* Thank the player for playing */
            userInt.EndUI();
        }
    }
}
