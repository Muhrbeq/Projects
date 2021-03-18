using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Guess_Number_P2
{
    class HighScore
    {
        static private string projectDir = Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName;

        private FileStream fStream;
        private StreamWriter sWriter;
        private StreamReader sReader;

        public HighScore()
        {

        }

        /// <summary>
        /// Saves to a file
        /// </summary>
        /// <param name="scoreList">List of scores to save</param>
        /// <returns>return true if everything worked</returns>
        public bool SaveScore(List<Score> scoreList)
        {
            try
            {
                /* Check if file exists */
                if (!File.Exists(projectDir + @"\testdoc.txt"))
                {
                    /* If not, create file */
                    fStream = new FileStream(projectDir + @"\testdoc.txt", FileMode.Create);

                    /* Close stream */
                    fStream.Close();
                }

                /* Append to file so that old Highscore is not deleted */
                fStream = new FileStream(projectDir + @"\testdoc.txt", FileMode.Append);

                /* Write to file */
                using (sWriter = new StreamWriter(fStream, Encoding.UTF8))
                {
                    foreach (Score userScore in scoreList)
                    {
                        sWriter.WriteLine(userScore.Name + "," + userScore.Guess);
                    }
                }
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// Returns a list of scores (Unsorted)
        /// </summary>
        /// <returns>Returns a list of scores (Unsorted)</returns>
        public List<Score> PrintScore()
        {
            List<Score> printS = new List<Score>();

            try
            {
                /* Check if file exists */
                if(File.Exists(projectDir + @"\testdoc.txt"))
                {
                    /* Open for reading */
                    fStream = new FileStream(projectDir + @"\testdoc.txt", FileMode.Open);
                    
                    /* Read using StreamReader */
                    using (sReader = new StreamReader(fStream, Encoding.UTF8))
                    {
                        /* As long as there is another row to read */
                        while(sReader.Peek() >= 0)
                        {
                            /* Read and split line so that name and guess is seperated */
                            Score tempScore = new Score();
                            string line = sReader.ReadLine();
                            string[] linePart = line.Split(',');

                            /* Assign to Score */
                            tempScore.Name = linePart[0];
                            tempScore.Guess = int.Parse(linePart[1]);

                            /* Add score to list */
                            printS.Add(tempScore);
                        }
                    }
                }
                else
                {
                    return null;
                }
            }
            catch
            {
                return null;
            }

            return printS;
        }
    }
}
