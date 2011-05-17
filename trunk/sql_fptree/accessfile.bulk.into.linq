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
   
	string mdbfile=@"D:\全网EDGE终端比例.mdb";
	
	string bulk=@"
	SELECT * 
	INTO 全网EDGE终端比例
	FROM 
	OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="+"\""+mdbfile+"\""+@";User ID=Admin;Password=' )...FG";
	
		this.ExecuteCommand(bulk);	

		bulk.Dump();
	
}

// Define other methods and classes here