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

var hedian=from p in Redian_hedians
           where p.时间.Value.Hour==21
           group p by p.BSC into ttt
		   select new 
		   {
		      BSC= ttt.Key,
			  T话务量=ttt.Average(e=>e.T话务量),
			  H话务比=ttt.Average(e=>e.H话务比),
			  T拥塞率=ttt.Average(e=>e.T拥塞率),
			  干扰系数=ttt.Average(e=>e.干扰系数),
			  上行TBF建立成功率=ttt.Average(e=>e.上行TBF建立成功率),
			  下行TBF建立成功率=ttt.Average(e=>e.下行TBF建立成功率),
			  EDGE下行速率=ttt.Average(e=>e.EDGE下行速率),
			  EDGE终端比例=ttt.Average(e=>e.EDGE终端比例),
			  上行TBF异常释放=ttt.Average(e=>e.上行TBF异常释放)
		   };
		   
	hedian.OrderBy(e=>e.EDGE下行速率).Dump();