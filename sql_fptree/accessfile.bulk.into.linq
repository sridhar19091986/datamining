<Query Kind="Program">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
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
	string mdbfile=@"d:\FG_BSC小时GPRS流量2011-09-06.mdb";
	int i=0;

	
	string bulknew=@"
	SELECT * INTO FG_BSC小时GPRS流量
	FROM 
	OPENDATASOURCE ('Microsoft.Jet.OLEDB.4.0','Data Source="+"\""+mdbfile+"\""+@";User ID=Admin;Password=' )...FG_BSC小时GPRS流量";
	
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