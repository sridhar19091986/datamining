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

var tbf=from q in BSCGPRSCHECK_20110715s
        where q.时间.Value.Hour==21
		where q.RPP0040+q.RPP4160+q.RPP6180+q.RPP8190+q.RPP9100>0
		group q by q.BSC into ttt
		select new {
		BSC=ttt.Key,
		RPPrate=100.0*ttt.Sum(p=>(p.RPP8190+p.RPP9100))/ttt.Sum(p=>(p.RPP0040+p.RPP4160+p.RPP6180+p.RPP8190+p.RPP9100)),
		RPP=ttt.Sum(p=>p.RPP告警次数),
		RPnum=ttt.Max(p=>p.RP_NUM),
		GSL=ttt.Average(p=>p.设备利用率),
		PCU=ttt.Average(p=>p.PCU拥塞率),
		failmove=ttt.Sum(p=>p.FAILMOVECELL),
		ultbf=ttt.Average(p=>p.上行TBF建立成功率),
		dltbf=ttt.Average(p=>p.下行TBF建立成功率)
		};
		
		tbf.Dump();
		