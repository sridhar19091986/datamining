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
       where p.时间.Value.Hour==21
       group p by p.小区号 into ttt
	   select new 
	   {
	   Cell=ttt.Key,
	   Tchal=ttt.Max(e=>e.T可用信道)-ttt.Max(e=>e.EDGE下行激活信道),
	   Ttraffic=ttt.Average(e=>e.T话务量)
	   };
var hft=from p in hf
       where p.Ttraffic>0 && p.Tchal>0
        select new 
	{
	小区21点一周平均=p.Cell,
	T可用信道除以T话务量=p.Tchal/p.Ttraffic,
	OD为0时的半速率=(1-p.Tchal/p.Ttraffic)*2.0
	};
	//hft.Where(e=>e.OD为0时的半速率>0.2).Count();
	hft.OrderBy(e=>e.T可用信道除以T话务量).Dump();
