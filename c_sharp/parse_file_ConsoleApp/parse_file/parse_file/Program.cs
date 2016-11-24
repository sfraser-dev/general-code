using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Collections.Specialized;
using System.IO;

// Using Windows Forms in c# console application:
// To add reference in c# program right click in your project folders shown in solution 
// explorer on add references-> .Net -> select System.Windows.Forms
using System.Windows.Forms;



namespace parse_file
{
    class Program
    {
        static void Main(string[] args)
        {
            string lineFromFile;
            string[] commentSplit;
            StringCollection noComments = new StringCollection();
            StreamReader fileReader = null;

            // open the file
            try
            {
                fileReader = File.OpenText("C:\\ProgramData\\VisualSoft\\VisualWorks\\DVR\\QualityProfiles\\H264\\Hardware\\VitecEncodingProfiles.dat");
            }
            catch (IOException)
            {
                System.Diagnostics.Debug.Write("Error: Cannot find/open Vitec Encoding Profiles file");
                System.Windows.Forms.MessageBox.Show("Cannot find/open Vitec Encoding Profiles file", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            
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
                if (!((commentSplit[0].Trim()).StartsWith("name", true, null)))
                    continue;
                noComments.Add(commentSplit[0]);
            }
            if (noComments.Count == 0)
            {
                System.Diagnostics.Debug.Write("Error: No profiles have been read from the Vitec Encoder Profiles file");
                System.Windows.Forms.MessageBox.Show("No profiles have been read from the Vitec Encoder Profiles file", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
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
                    System.Diagnostics.Debug.Write("Error: line found in Vitec Encoder Profiles file without three comma separated tokens");
                    System.Windows.Forms.MessageBox.Show("Line found in Vitec Encoder Profiles file without three comma separated tokens", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                
                foreach (string commaToken in commaTokens)
                {
                    // remove white space from start and end of each token
                    commaToken.Trim();
                    // look for tokens that start with "name" (case insensitive; don't need the "video" and "audio" tokens)
                    if (commaToken.StartsWith("name", true, null))
                    {
                        // grab only the profile-name
                        equalTokens = commaToken.Split('=');
                        profileName = equalTokens[1];
                        // add the profile-name to the "old" StringCollection lstProfile (minimise changes for now)
                        lstProfile.Add(profileName.Trim());
                    }
                }
            }

            // close the file
            fileReader.Dispose();
            fileReader = null;

            // debug
            foreach (string s in lstProfile)
            {
                System.Console.WriteLine(s);
            }

            System.Console.WriteLine("Press any key to exit ...");
            Console.ReadKey();
        }
    }
}
