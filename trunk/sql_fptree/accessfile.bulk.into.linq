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
   
	string mdbfile=@"c:\SZ24B_hedian.mdb";
	
	string bulk=@"
	SELECT * 
	INTO SZ24B_hedian
	FROM 
	OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="+"\""+mdbfile+"\""+@";User ID=Admin;Password=' )...fg";
	
		this.ExecuteCommand(bulk);	

		bulk.Dump();
	
}

// Define other methods and classes here