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

var cell=from p in CELL_20110729s
         group p by new {p.时间,p.BSC} into ttt
		 select new 
		 {
		 时间=ttt.Key.时间,
		 BSC=ttt.Key.BSC,
		 PSCHREQ=ttt.Sum(e=>e.PSCHREQ),
		 PREJTFI=ttt.Sum(e=>e.PREJTFI),
		 PREJOTH=ttt.Sum(e=>e.PREJOTH)
		 };

var bsc1=from p in BSC_20110729s
        join q in cell on new {p.时间,p.BSC} equals new {q.时间,q.BSC} 
		select new {p,q.PSCHREQ,q.PREJTFI,q.PREJOTH};

bsc1.Take(1).Dump();