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

var tbf = from p in  BSCGPRS2_20110707s
		          let ttime=Convert.ToDateTime(p.时间)
				  where p.BSC=="SZ47D"
				  select new 
				  {p.时间,
				  p.下行TBF建立成功率,
				  RPP告警=1.00*(p.RPP8190+p.RPP9100)/(p.RPP0040+p.RPP4160+p.RPP6180+p.RPP8190+p.RPP9100),
				  p.T损坏率,
				  p.PDCH复用度};
var rpp=from p in EMGPRS_20110707s
 where p.BSC=="SZ47D"
		group p by new {p.时间,p.RPPID} into ttt
		select new 
		{
		   时间=ttt.Key.时间,
		   RPPID=ttt.Key.RPPID,
		   RPPLOAD=ttt.Sum(e=>e.RPPLOAD)
		};
var tbfrpp=from p in tbf
           join q in rpp on p.时间 equals q.时间 
		   select new {p.时间,p.下行TBF建立成功率,q.RPPID,q.RPPLOAD};
var tbfr=from p in tbfrpp
         where p.RPPLOAD>0
         group p by p.RPPID into tt
		 select new {tt.Key,rppload=tt.Max(e=>e.RPPLOAD),dltbf=tt.Max(e=>e.下行TBF建立成功率)};
tbfr.OrderBy(e=>e.dltbf).Dump();