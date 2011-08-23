<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>CellLoadSharing</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var ho=from p in 小区切换查询_0822s
       group p by new {p.小区名,p.邻小区名} into ttt
	   select new 
	   {
	   小区名=ttt.Key.小区名,
	   邻小区名=ttt.Key.邻小区名,
	   切换次数=ttt.Sum(e=>e.切换次数)
	   };
	 
	//ho.Take(1).Dump();
	ho.Dump();
	   