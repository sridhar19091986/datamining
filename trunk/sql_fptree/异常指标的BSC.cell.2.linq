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

var tbf=from p in Cell_20110707s
        where p.时间.Value.Hour==21
		where p.BSC=="SZ47D"
        group p by  p.小区名 into ttt
        select new {ttt.Key,
		FAILDLTBFEST=ttt.Sum(e=>e.FAILDLTBFEST),
		FAILDLTBFESTr=1.0*ttt.Sum(e=>e.FAILDLTBFEST)/Cell_20110707s
		.Where(e=>e.BSC=="SZ47D")
		.Where(e=>e.时间.Value.Hour==21).Sum(e=>e.FAILDLTBFEST),
		下行TBF建立成功率=ttt.Average(e=>e.下行TBF建立成功率),
		T损坏率=ttt.Average(e=>e.T损坏率),
		PDCH复用度=ttt.Average(e=>e.PDCH复用度),
		EDGE终端使用EDGE比例=ttt.Average(e=>e.EDGE终端使用EDGE比例),
		H话务比=ttt.Average(e=>e.H话务比)
		};	
		tbf.OrderByDescending(e=>e.FAILDLTBFEST).Dump();