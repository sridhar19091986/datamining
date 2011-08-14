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

var tbf = from p in Bsc_cell_20110729s
          where p.时间.Value.Hour==20
		  group p by p.BSC into ttt
          select new 
		  {
		  BSC=ttt.Key,
		  上行TBF建立成功率=ttt.Average(p=>p.上行TBF建立成功率),
		 设备利用率=ttt.Average(p=> p.设备利用率),
		 RPPr=ttt.Average(p=>  p.RPPr),
		  下行TBF建立成功率=ttt.Average(p=> p.下行TBF建立成功率),
		  T损坏率=ttt.Average(p=> p.T损坏率),
		 EDGE终端使用EDGE比例=ttt.Average( p=> p.EDGE终端使用EDGE比例),
		  PDCH复用度=ttt.Average(p=> p.PDCH复用度),
		  };
		  tbf.Where(e=>e.上行TBF建立成功率<95 || e.下行TBF建立成功率<95).Dump();