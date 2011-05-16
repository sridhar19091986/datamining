<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
   
	string mdbfile=@"D:\FG.mdb";
	
	string bulk=@"
	SELECT * 
	INTO gprsredian
	FROM 
	OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="+"\""+mdbfile+"\""+@";User ID=Admin;Password=' )...FG_小区小时RAU";
	
		this.ExecuteCommand(bulk);	

		bulk.Dump();
	
}

// Define other methods and classes here
