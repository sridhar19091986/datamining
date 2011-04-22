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
	var ci=from p in Gprs_cis.Take (50)
	       select new {p.Cell_name, lat=L2Lat(p.Latitude),lon=L2Lon(p.Longitude)};
	ci.Dump();
	var ts=from p in Gprs_tousus
	       from q in  ci
	       where p.经度==q.lat && p.纬度==q.lon
		   select new 
		   {
		       p.请求编号,q.Cell_name
		   };
	var a=ts.ToList();
    a.Dump();
}

// Define other methods and classes here
string  ttt;
string L2Lat (string input)
{
    if(input.Length<2) return "1";
	string l=input.Replace("N","").Replace("-","").Replace(".","");
	string a=l.Substring(0,2);
	string b=l.Substring(2,l.Length-2);
	ttt=a+"."+b;
	return ttt;
}
// Define other methods and classes here
string  nnn;
string L2Lon (string input)
{
	if(input.Length<2) return "1";
	string l=input.Replace("E","").Replace("-","").Replace(".","");
	string a=l.Substring(0,3);
	string b=l.Substring(3,l.Length-3);
	nnn=a+"."+b;
	return nnn;
}