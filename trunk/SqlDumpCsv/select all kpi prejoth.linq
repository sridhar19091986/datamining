<Query Kind="Statements">
  <Connection>
    <ID>337b631f-296e-4a3b-aa36-09f23a9a9a38</ID>
    <Server>.\SQLEXPRESS</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var prejoth=from p in Gprsredian_ps
            where p.时间.Value.Hour.ToString()=="18"
            group p by p.小区号 into tt
			select new 
			{
			   cell=tt.Key,
			   上行TBF建立成功率=tt.Average(e=>e.上行TBF建立成功率),
			   PS立即指派成功率=tt.Average(e=>e.PS立即指派成功率),
			   PREJOTH=tt.Sum(e=>e.PREJOTH)
			   
			};
prejoth.OrderByDescending(e=>e.PREJOTH).Dump();