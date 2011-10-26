<Query Kind="Program">
  <Connection>
    <ID>65e0892b-2c67-48c8-9d4f-3aa69a59de16</ID>
    <Server>.\SQLEXPRESS</Server>
    <AttachFile>true</AttachFile>
    <UserInstance>true</UserInstance>
    <AttachFileName>&lt;ApplicationData&gt;\LINQPad\Samples\LINQ in Action\lia6-8.mdf</AttachFileName>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
		  StreamReader sr = new StreamReader("RLMFP");
		  var t1 =
			from line in sr.Lines()
			let items = line.Split(',')
			where ! line.StartsWith("#")
			select String.Format("{0}{1}{2}",
				items[1].PadRight(16),
				items[2].PadRight(16),
				items[3].PadRight(16));
		  var t2 =
			from line in t1
			select line.ToUpper();
		  foreach (var t in t2)
			Console.WriteLine(t);
		  sr.Close();
	
}

// Define other methods and classes here
  public static class StreamReaderSequence
  {
	public static IEnumerable<string> Lines(this StreamReader source)
	{
	  String line;
	  if (source == null)
		  throw new ArgumentNullException("source");
	  while ((line = source.ReadLine()) != null)
	  {
		yield return line;
	  }
	}
  }
