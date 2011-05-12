<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>mytest</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	  string drop=@"drop table mytest.dbo.PS总流量10000";
	  string create=@"
	  select top 10000 * 
	  into mytest.dbo.PS总流量10000
      from Gprsredian_ps
      order by PS总流量 desc
	  ";
	  //this.ExecuteCommand(drop);
	  this.ExecuteCommand(create);
}

// Define other methods and classes here