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

//var cell_rp=from p in Gprs_cis.Take (50);
//
//var bsc_rp=from p in Gprs_rpps.Take (50);
//
//var cell_gis=from p in cGprs_cell_rps.Take (50);


var m=from p in Gprs_cis.Take (50)
	  from t in Gprs_cell_rps.Take (50)
	  where p.Cell_name == t.CELL
	  select new 
	  {
	     p.BSC,p.Cell_name,p.Cellid,p.Latitude,p.Longitude,
		 t.CELL,t.NOPDCH,t.RP
	  
	  };