<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var tbf=from p in BSCDLTBF_Analysis_0715s
        where p.TTime.Value.Hour==20
        group p by  p.BSC into ttt
		select new 
		{
		  //TTime=ttt.Key.时间,
		  BSC=ttt.Key,
		  DLTBFEST=ttt.Sum(e=>e.DLTBFEST),
		  FAILDLTBFEST=ttt.Sum(e=>e.FAILDLTBFEST),
		  PSCHAREQ=ttt.Sum(e=>e.PSCHAREQ),
		  PREJTFI=ttt.Sum(e=>e.PREJTFI),
		  PREJOTH=ttt.Sum(e=>e.PREJOTH),
		  DLTBF=ttt.Average(e=>e.下行TBF建立成功率)
		};
		tbf.OrderByDescending(e=>e.FAILDLTBFEST).Dump();