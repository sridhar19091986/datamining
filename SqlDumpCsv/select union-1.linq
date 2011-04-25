<Query Kind="Program">
  <Connection>
    <ID>337b631f-296e-4a3b-aa36-09f23a9a9a38</ID>
    <Server>.\SQLEXPRESS</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	var m=from p in Gprs_cell_gis
		  from t in Gprs_cell_rps
		  where p.Cell_no== t.CELL
		  select new {p,t};
		 // m.Dump();
	var n=from q in Gprs_rpps
	      group q by new {q.BSC,q.RPPID} into ttt
		  select new 
		  {BSC=ttt.Key.BSC,RPPID=ttt.Key.RPPID,RPPLOAD=ttt.Sum(e=>e.RPPLOAD)};
		  
	var k=from q in n
	      from t in m
		  where q.BSC==t.p.Bsc && q.RPPID.ToString() ==t.t.RP
		  select new { q.BSC,q.RPPID,q.RPPLOAD,
		  t.p.Bsc,t.p.Cell_name,t.p.Cell_no,t.p.Latitude,t.p.Longitude,t.p.基站代号,t.p.基站名,
		  t.t.CELL,t.t.NOPDCH,t.t.RP};
	
		  k.Dump();
		  
	foreach(var m in  Gprs_tousus)
	    
}

// Define other methods and classes here
public class GPRStuple
{
  public Gprs_tousus tousu;
  public Dictinary<GPRSoss,double> oss;
}
public class GPRSoss
{
  public string 

}
