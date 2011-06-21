<Query Kind="Statements">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

//sz16B 23:00  6.7  //SZ31B  23:00  6.8

var pfc = from p in Osspfc_gbtables
		  where (p.BSC == "SZ16B" && p.时间.Value.Day==7 )||  ( p.BSC == "SZ31B"    && p.时间.Value.Day==8)
		  where p.时间.Value.Hour== 23    
		  select new
{
	lac = "",
	ci = "",
	ci_name = p.小区名,
	stat_time = p.时间,
	用户平均下行速率 = (p.EDGE下行速率+p.GPRS下行速率)/2,     //用户平均下行速率
	平均每PFC下行流量 = (p.EDGE每PFC下行流量 +p.GPRS每PFC下行流量) / 2,   //平均pfc标识业务识别
	平均每信道下行速率 = (p.EDGE每信道下行速率+p.GPRS每信道下行速率)/2  //小区平均下行速率
};

pfc.Dump();