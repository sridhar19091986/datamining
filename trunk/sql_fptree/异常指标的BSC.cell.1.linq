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
        where p.下行TBF建立成功率<80
		//where p.时间.Value.Hour==20
		//where p.GPRS下行激活信道>0
		where p.EDGE终端比例>87
		select p;
//        select new 
//		{
//		  p.下行TBF建立成功率,
//		  掉话=p.上行质差掉话+p.下行质差掉话+p.上行弱信号掉话+p.下行弱信号掉话+p.全质差掉话+p.全弱信号掉话
//		};
		
		tbf.OrderByDescending(e=>e.FAILDLTBFEST).Dump();