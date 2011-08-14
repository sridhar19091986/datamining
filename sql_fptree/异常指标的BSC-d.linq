<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var cell=from p in Bsc_cell_20110729s
         where p.BSC=="SZ22B"
		 select p;
		 
		 cell.OrderBy(e=>e.时间).Dump();
		 
		 //cell.Count().Dump();