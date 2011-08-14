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

var ttime=BSCGPRS2_20110707s.Min(e=>e.时间).Value;
var mtime=BSCGPRS2_20110707s.Max(e=>e.时间).Value;
var busyhour = from p in BSCGPRS2_20110626s
					   group p by p.时间 into ttt
					   select new { 时间 = ttt.Key,  T话务量 = ttt.Sum(e => e.T话务量) };
var bh = busyhour.OrderByDescending(e => e.T话务量).FirstOrDefault().时间;
var tbf=from p in BSCGPRS2_20110707s
        //where p.时间.Value.Hour == bh.Value.Hour
        where p.RPP0040+p.RPP4160+p.RPP6180+p.RPP8190+p.RPP9100>0
        group p by p.BSC into ttt
		select new 
		{
		   BSC=ttt.Key,
		   开始时间=ttime,
		   结束时间=mtime,
		   //Ttime = bh.Value.Hour,
		   上行TBF建立成功率=ttt.Average(e=>e.上行TBF建立成功率),
		   //PS=ttt.Average(e=>e.PS流量),
		   RPPr=1.0*ttt.Sum(e=>e.RPP8190+e.RPP9100)/ttt.Sum(e=>e.RPP0040+e.RPP4160+e.RPP6180+e.RPP8190+e.RPP9100)
		};
		
	tbf.OrderBy(e=>e.上行TBF建立成功率).Dump();
	
	