<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>CellLoadSharing</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
   	
	//FG_小区小时GPRS性能
	//现网cdd_Channel

//    此2行修改
	string mdbfile=@"d:\小区基础数据2011-08-05.mdb";
	int i=0;

	
	string bulknew=@"
	SELECT * INTO 小区基础数据0803
	FROM 
	OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="+"\""+mdbfile+"\""+@";User ID=Admin;Password=' )...小区基础数据";
	
	string bulkinsert=@"
	
	insert into BSCGPRS2 select *
	FROM 
	OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="+"\""+mdbfile+"\""+@";User ID=Admin;Password=' )...BSCGPRS2";

		
		if(i==0)
		{bulknew.Dump();
		this.ExecuteCommand(bulknew);bulknew.Dump();
		}
	    else
		{
      	this.ExecuteCommand(bulkinsert);bulkinsert.Dump();
		}
		
	
}

// Define other methods and classes here