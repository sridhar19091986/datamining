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

var tbf=from p in BSCGPRS2s
 where p.BSC=="SZ27B"
        group p by p.RPPID into ttt
		select new 
		{
		   RPPID=ttt.Key,
		   RPPLOAD=ttt.Sum(e=>e.RPPLOAD)
		};
		
	tbf.OrderBy(e=>e.RPPLOAD).Dump();