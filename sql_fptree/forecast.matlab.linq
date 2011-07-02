<Query Kind="Program">
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	 //Get the path to the executable you wish to run from a setting in web.config
			var executablePath =@"D:\My Documents\MATLAB\mygrnn.exe";
			//Create a process to execute the executable. Redirecting the output.
			var proc = new Process();
			proc.StartInfo = new ProcessStartInfo
			{
				CreateNoWindow = true,
				ErrorDialog = false,
				FileName = executablePath,
				RedirectStandardError = true,
				RedirectStandardOutput = true,
				//Arguments = "\r\n  mygrnn \r\n",
				UseShellExecute = false //Very important do not leave this out.
			};
			proc.Start(); //Execute the executable.
			string lblOutput = proc.StandardOutput.ReadToEnd(); //use the results
			string lblErrorMessages = proc.StandardError.ReadToEnd(); //Show any error output from the executable
			lblOutput.Dump();
			lblErrorMessages.Dump();
}

// Define other methods and classes here
