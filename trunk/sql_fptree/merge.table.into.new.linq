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
	  string drop=@"drop table mytest.dbo.gprsredian_ps";
	  string create=@"
	  select * into mytest.dbo.gprsredian_ps
from (
SELECT a.*,b.PS总流量,
b.上行TBF异常释放,	b.PS立即指派成功率,	b.上行TBF掉线率,	
b.平均分配PDCH,	b.平均使用PDCH,	b.GPRS下行激活信道,	b.GPRS每线下行用户,	
b.EDGE下行激活信道,	b.EDGE每线下行用户,	b.PDCH复用度,	
b.PSIMMASS,	b.REJPSIMMASS,	b.PSCHREQ,	b.PREJTFI,	b.PREJOTH,	b.FAILDLTBFEST
FROM dbo.gsmredian as a,dbo.gprsredian_new b
where a.时间=b.时间 and a.小区号=b.小区号
) as c
	  ";
	  this.ExecuteCommand(drop);
	  this.ExecuteCommand(create);
}

// Define other methods and classes here
