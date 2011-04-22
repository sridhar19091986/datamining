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

var m=from p in PREJOTH50000s
      where p.时间.Value.Hour.ToString()=="18"
      group p by p.BSC into tt
	  let aCount=tt.Sum(e=>e.RPP0040)+tt.Sum(e=>e.RPP4160)+tt.Sum(e=>e.RPP6180)+tt.Sum(e=>e.RPP8190)+tt.Sum(e=>e.RPP9100)
	  select new 
	  {
	  tt.Key,
	  RPP0040=tt.Sum(e=>e.RPP0040),
	  RPP4160=tt.Sum(e=>e.RPP4160),
	  RPP6180=tt.Sum(e=>e.RPP6180),
	  RPP8190=tt.Sum(e=>e.RPP8190),
	  RPP9100=tt.Sum(e=>e.RPP9100),
	  RPP0040Rate=1.0*tt.Sum(e=>e.RPP0040)/(aCount==0?1:aCount),
	  RPP4160Rate=1.0*tt.Sum(e=>e.RPP4160)/(aCount==0?1:aCount),
	  RPP6180Rate=1.0*tt.Sum(e=>e.RPP6180)/(aCount==0?1:aCount),
	  RPP8190Rate=1.0*tt.Sum(e=>e.RPP8190)/(aCount==0?1:aCount),
	  RPP9100Rate=1.0*tt.Sum(e=>e.RPP9100)/(aCount==0?1:aCount),
	  PREJOTH=tt.Sum(e=>e.PREJOTH)
	  };
	  m.OrderByDescending(e=>e.PREJOTH).Dump();