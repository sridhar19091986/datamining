<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
    string ins=null;
	var rpp=from p in EMGPRS_20110715s
	       group p by new {p.时间,p.BSC} into ttt
	        select new 	
			{
			  ttime=ttt.Key.时间,
			  bsc=ttt.Key.BSC,
			  rpp= Variance(ttt.Select(e=>(int)e.RPPLOAD)),
			};
	foreach(var a in rpp)
	{
        ins=@"insert into _EMGPRS_20110715s(TTime,BSC,RPPload) VALUES(
	'" + a.ttime + "','" + a.bsc + "','" + a.rpp   + "')";
	//ins.Dump();
			this.ExecuteCommand(ins);
	}
}
// Define other methods and classes here
	public  double Variance(IEnumerable<int> source) 
	{ 
		double avg = source.Average();
		double d = source.Aggregate(0.0, (total, next) => total += Math.Pow(next - avg, 2)); 
		return d / source.Count();
	}