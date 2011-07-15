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

var tbf=from p in EMGPRS_20110707s
 where p.BSC=="SZ47D"
 where p.时间.Value.Day==3
  where p.时间.Value.Hour==10
        group p by new {p.时间,p.RPPID} into ttt
		select new 
		{
		   时间=ttt.Key.时间,
		   RPPID=ttt.Key.RPPID,
		   RPPLOAD=ttt.Sum(e=>e.RPPLOAD)
		};
		
	tbf.OrderBy(e=>e.RPPLOAD).Dump();