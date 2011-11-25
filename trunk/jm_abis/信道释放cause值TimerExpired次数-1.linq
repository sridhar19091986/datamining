<Query Kind="Statements">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
    <Database>AbisJiangMeng</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var all=Recovered_Sheet1s.Count();
var rl=from p in Recovered_Sheet1s
       group p by p.小区中文名 into ttt
       //group p by p.小区中文名==null into ttt
       select new 
	   {   
	      ttt.Key,
		  信道释放cause值TimerExpired小区占比=1.0*ttt.Count()/all,
		  信道释放cause值TimerExpired次数=ttt.Count(),
	      SDCCH掉话=ttt.Sum(e=>Convert.ToInt32(e.SDCCH掉话)),
		  TCH掉话=ttt.Sum(e=>Convert.ToInt32(e.TCH掉话)),
		  未接通=ttt.Sum(e=>Convert.ToInt32(e.未接通)),
		  SDCCH掉话占比=1.0*ttt.Sum(e=>Convert.ToInt32(e.SDCCH掉话))/ttt.Count(),
		  TCH掉话占比=1.0*ttt.Sum(e=>Convert.ToInt32(e.TCH掉话))/ttt.Count(),
		  未接通占比=1.0*ttt.Sum(e=>Convert.ToInt32(e.未接通))/ttt.Count(),   
	   };
	   rl.OrderByDescending(e=>e.信道释放cause值TimerExpired次数).Dump();