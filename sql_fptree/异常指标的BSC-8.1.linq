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
				  //where ttime.Hour == 21
				  where p.BSC=="SZ47D"
				  //group p by p.BSC into ttt
				  select new 
				  {p.时间,
				  p.下行TBF建立成功率,
				  RPP告警=1.00*(p.RPP8190+p.RPP9100)/(p.RPP0040+p.RPP4160+p.RPP6180+p.RPP8190+p.RPP9100),
				  p.T损坏率,
				  p.PDCH复用度};
	tbf.Dump();