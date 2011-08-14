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

var tbf=from p in SZ47G_DLTBFs
        //where p.上行TBF建立成功率>0
		//|| p.BSC=="SZ35B"
		//where p.时间.Value.Hour==21
		//where p.EDGE每线下行用户<10
        //orderby p.下行TBF建立成功率
		//select p;
		select new {p.小区名,p.时间,p.PDCH复用度,p.EDGE终端使用EDGE比例,p.T损坏率,p.下行TBF建立成功率};

var tbf1000=tbf.OrderBy(e=>e.下行TBF建立成功率).Take(1000);

var pdch=tbf.Where(e=>e.EDGE终端使用EDGE比例>0).Where(e=>e.PDCH复用度<6).Where(e=>e.下行TBF建立成功率<80).OrderBy(e=>e.EDGE终端使用EDGE比例).Take(10);
		
	pdch.Dump();