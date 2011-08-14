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

var tbf=from p in FG_BSC小时GPRS流量0803s
        group p by p.BSC into ttt
		select new 
		{
		ttt.Key,
		tbf=ttt.Average(e=>e.下行TBF建立成功率)
		};
	tbf.OrderBy(e=>e.tbf).Dump();