<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
   
	string mdbfile=@"D:\指标\SZ51_55指标0605-0611.mdb";
	
	//FG_小区小时GPRS性能
	//现网cdd_Channel
	//SELECT * INTO osspfc_gbtable
	
	string bulk=@"
	
	insert into osspfc_gbtable select *
	FROM 
	OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="+"\""+mdbfile+"\""+@";User ID=Admin;Password=' )...FG_小区小时GPRS性能";
	
		this.ExecuteCommand(bulk);	

		bulk.Dump();
	
}

// Define other methods and classes here