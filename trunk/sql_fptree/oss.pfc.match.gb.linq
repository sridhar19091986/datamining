<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var pfc = from p in Redian_hedians

		  where p.BSC == ""

		  where p.时间 = ""

		  select new

{

	lac = "",
	ci = "",
	ci_name = p.小区名,
	stat_time = p.时间,
	available_pdch = (p.EDGE下行速率+p.GPRS下行速率)/2,
	use_pdch = (p.EDGE每PFC下行流量 +p.GPRS每PFC下行流量) / 2,   //平均pfc标识业务识别
	assignment_pdch_rate = 1
}