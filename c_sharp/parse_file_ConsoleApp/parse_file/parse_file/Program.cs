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
            StreamReader fileReader = File.OpenText("C:\\ProgramData\\VisualSoft\\VisualWorks\\DVR\\QualityProfiles\\H264\\Hardware\\VitecEncodingProfiles.dat");
            string lineFromFile;
            string [] commentSplit;
            StringCollection noComments = new StringCollection();
            // read each line
            while ((lineFromFile = fileReader.ReadLine()) != null)
            {
                // ignore blank lines
                if (lineFromFile.Length == 0)
                    continue;
                // remove comments
                commentSplit = lineFromFile.Split('#');
                // ignore lines that are just comments
                if (commentSplit[0].Length == 0)
                    continue;
                // ignore lines that don't specifically start with "name"
                if (!((commentSplit[0].Trim()).StartsWith("name")))
                    continue;
                noComments.Add(commentSplit[0]);
            }

            StringCollection lstProfile = new StringCollection();
            string [] commaTokens;
            string[] equalTokens;
            string profileName;
            foreach (string noCommentLine in noComments)
            {
                // split each line into tokens (delimited by commas)
                commaTokens = noCommentLine.Split(',');
                if (commaTokens.Count() != 3)
                {
                    // todo: handle error
                }
                foreach (string commaToken in commaTokens)
                {
                    // remove white space from start and end of each token
                    commaToken.Trim();
                    // look for tokens that start with "name" (don't need the "video" and "audio" tokens)
                    if (commaToken.StartsWith("name"))
                    {
                        // grab only the profile-name
                        equalTokens = commaToken.Split('=');
                        profileName = equalTokens[1];
                        // add the profile-name to the "old" StringCollection lstProfile (minimise changes for now)
                        lstProfile.Add(profileName.Trim());
                    }
                }
            }

            // debug
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
