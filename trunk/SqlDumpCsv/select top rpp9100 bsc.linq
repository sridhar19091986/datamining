<Query Kind="Statements">
  <Connection>
    <ID>337b631f-296e-4a3b-aa36-09f23a9a9a38</ID>
    <Server>.\SQLEXPRESS</Server>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var m=from p in PREJOTH50000s
      group p by p.BSC into tt
	  select new 
	  {
	  tt.Key,
	 aa= tt.Sum(e=>e.RPP9100),
	 bb=tt.Sum(e=>e.PREJOTH)
	  
	  };
	  
	  m.OrderByDescending(e=>e.bb).Dump();