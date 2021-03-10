using System;
using System.Collections.Generic;
using System.Text;

namespace Guess_Number_P2
{
    class Score
    {
        private string _Name;
        public string Name
        {
            set { _Name = value; }
            get { return _Name; }
        }

        private int _Guess;
        public int Guess
        {
            set { _Guess = value; }
            get { return _Guess; }
        }

        public Score()
        {

        }
    }
}
