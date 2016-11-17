using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Collections.Specialized;
using System.IO;

namespace parse_file
{
    class Program
    {
        static void Main(string[] args)
        {
            // open the file
            StreamReader fileReader = File.OpenText("C:\\ProgramData\\VisualSoft\\VisualWorks\\DVR\\QualityProfiles\\H264\\Hardware\\VitecEncodingProfiles.txt");
            string lineFromFile;
            string [] arrLinesWithoutComments;
            StringCollection collSelectedLinesFromFile = new StringCollection();
            // read each line
            while ((lineFromFile = fileReader.ReadLine()) != null)
            {
                // ignore blank lines
                if (lineFromFile.Length == 0)
                    continue;
                // remove comments
                arrLinesWithoutComments = lineFromFile.Split('#');
                // ignore lines that are just comments
                if (arrLinesWithoutComments[0].Length == 0)
                    continue;
                collSelectedLinesFromFile.Add(arrLinesWithoutComments[0]);
            }

            StringCollection lstProfile = new StringCollection();
            string [] tokens;
            foreach (string selectedLine in collSelectedLinesFromFile)
            {
                // split each selected line into tokens (delimited by commas)
                tokens = selectedLine.Split(',');
                foreach (string token in tokens)
                {
                    // find tokens that start with "name"
                    if (token.StartsWith("name"))
                    {
                        // add the trimmed tokens (starting with "name") to the StringCollection lstProfile
                        lstProfile.Add(token.Trim());
                    }
                }
            }

            foreach (string s in lstProfile)
            {
                System.Console.WriteLine(s);
            }

            fileReader.Close();
            System.Console.WriteLine("Press any key to exit ...");
            Console.ReadKey();
        }
    }
}
