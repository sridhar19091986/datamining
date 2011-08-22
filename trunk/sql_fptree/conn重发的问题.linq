<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>JiangMengGnInterface</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var conn=from p in Gn_Connect1xes
		 group p by p.Dest_IP into ttt
		 select new 
		 {
		 Dest_IP = ttt.Key,
		 Dest_IP_Count= ttt.Count(),
		 Dest_IP_Error_Count= ttt.Where(e=>e.Connect_Repeat>0).Count(),
		Dest_IP_Error_Rate= 1.0*ttt.Where(e=>e.Connect_Repeat>0).Count()/ttt.Count(),
		 };
	
	conn.OrderByDescending(e=>e.Dest_IP_Error_Count).Dump();
	
