<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	  string drop=@"drop table gprsredian_ps";
	  string create=@"
	  select * into  gprsredian_ps
from (
SELECT a.*,b.EDGE终端比例
FROM Gprsredian as a,全网EDGE终端比例 as b
where a.时间=b.时间 and a.小区号=b.小区号
) as c
	  ";
	  //this.ExecuteCommand(drop);
	  this.ExecuteCommand(create);
}

// Define other methods and classes here