<Query Kind="Statements">
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

var rpp=from p in Bsc_cell_20110729s
        where p.时间.Value.Hour==23
        group p by p.BSC into ttt
		select new 
		{
		BSC=ttt.Key,
		RP_NUM=ttt.Max(e=>e.RP_NUM),
		ultbf=ttt.Average(e=>e.上行TBF建立成功率),
		RPPr=ttt.Average(e=>e.RPPr),
		RPPpschreq=ttt.Average(e=>e.RPPpschreq)
		};
rpp.OrderBy(e=>e.ultbf).Dump();