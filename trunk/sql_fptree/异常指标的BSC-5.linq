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

var tbf=from p in BSCGPRS2_20110707s
        where p.上行TBF建立成功率>0
				where p.BSC=="SZ34A" 
		//|| p.BSC=="SZ35B"
//        orderby p.下行TBF建立成功率
		select p;
		
	tbf.Dump();