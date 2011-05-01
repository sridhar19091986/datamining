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

var ttt=from p in Pdchredian1000s
        select p;
var m=from p in ttt

      let stepbystep=0.1*ttt.Count()
      select new 
	  
	  {
	  
	  GPRS下行流量= (int)(p.GPRS下行流量/((ttt.Max(e=>e.PS下行流量)-ttt.Min(e=>e.PS下行流量))/stepbystep))+0.001
	  
	  
	  };
	  
	  m.Dump();
	  