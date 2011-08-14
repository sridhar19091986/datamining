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

var ttime=SZ47G_DLTBFs.Min(e=>Convert.ToDateTime(e.时间));
		var mtime=SZ47G_DLTBFs.Max(e=>Convert.ToDateTime(e.时间));

var tbf=from p in SZ47G_DLTBFs
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