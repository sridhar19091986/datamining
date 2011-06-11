<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var hf=from p in Gprsredian_ps
       group p by p.时间.Value.Hour into ttt
	   select new 
	   {
	   ttime=ttt.Key,
	   Tchal=ttt.Max(e=>e.T可用信道)-ttt.Max(e=>e.EDGE下行激活信道),
	   Ttraffic=ttt.Average(e=>e.T话务量)
	   };
var hft=from p in hf
        select new 
	{
	一周平均时间=p.ttime,
	全网T可用信道除以T话务量=p.Tchal/p.Ttraffic
	};
	hft.OrderBy(e=>e.一周平均时间).Dump();
