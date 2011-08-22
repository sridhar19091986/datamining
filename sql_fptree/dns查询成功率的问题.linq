<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>JiangMengGnInterface</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var conn=from p in DNS_Flows
		 group p by p.Query_Type into ttt
         select new 
		 {
		 Query_Type= ttt.Key,
		 Query_Type_Count= ttt.Count(),
		 Query_Type_Error_Count= ttt.Where(e=>e.Reply_Code!="No error").Count(),
		 Query_Type_Error_Rate= 1.0*ttt.Where(e=>e.Reply_Code!="No error").Count()/ttt.Count(),
		 };
	
	conn.OrderByDescending(e=>e.Query_Type_Error_Count).Dump();
	
var dns=from p in DNS_Flows
		 group p by p.Query_Name into ttt	 
		 select new 
		 {
		Query_Name= ttt.Key,
		Query_Name_Count= ttt.Count(),
	Query_Name_Error_Count= ttt.Where(e=>e.Reply_Code!="No error").Count(),
		 Query_Name_Error_Rate= 1.0*ttt.Where(e=>e.Reply_Code!="No error").Count()/ttt.Count(),
		 };
	
	dns.OrderByDescending(e=>e.Query_Name_Error_Count).Dump();