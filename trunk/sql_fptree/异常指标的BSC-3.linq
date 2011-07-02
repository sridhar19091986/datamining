<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var ttime=BSCGPRS2_20110626s.Min(e=>e.时间).Value;
		var mtime=BSCGPRS2_20110626s.Max(e=>e.时间).Value;

var tbf=from p in BSCGPRS2_20110626s
        where p.下行TBF建立成功率>0
        group p by p.BSC into ttt

		select new 
		{
		   BSC=ttt.Key,
		   开始时间=ttime,
		   结束时间=mtime,
		   下行TBF建立成功率=ttt.Average(e=>e.下行TBF建立成功率)
		};
		
	tbf.OrderBy(e=>e.下行TBF建立成功率).Dump();