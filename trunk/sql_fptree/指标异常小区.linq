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

PS总流量10000s.Where(e=>e.上行TBF建立成功率==0).
Select(e=>e.小区名+e.时间.ToString()).ToList().Aggregate((total, next) => total +"、"+ next).Dump();

PS总流量10000s.Where(e=>e.EDGE下行速率==0).Dump();

Gprsredian_news.Where(e=>e.小区号=="20GA061").Dump();