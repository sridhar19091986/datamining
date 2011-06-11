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

var pdch=from p in Gprsredian_news
         group p by p.小区号 into ttt
		 select new 
		 {
		  ci=ttt.Key,
		  pdch使用率=ttt.Max(e=>e.平均使用PDCH)/ttt.Max(e=>e.平均分配PDCH),
		  pdch=ttt.Max(e=>e.PDCH复用度),
		 
		 };
		
//		 N={(NE/R)*(NEU/NET)}
		 
	pdch.Where(e=>e.pdch_r>0.8).Where(e=>e.pdch_m>6).Dump();