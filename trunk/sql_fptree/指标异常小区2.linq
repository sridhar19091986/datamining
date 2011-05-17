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



//Gprsredian_ps.Where(e=>e.小区号=="01DA211").Dump();

//Gprsredian_ps.Where(e=>e.GPRS每线下行用户>22).Dump();

Gprsredian_ps.Select(e=>e.BSC).Distinct().Dump();
