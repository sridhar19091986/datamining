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
      string siteno="HalfRateEdge1day";
	  string drop=@"
                if exists(select * from sysobjects where type = 'U' and name = '"+siteno+@"') 
                drop table "+siteno;
	  string create=@"
	  select 
	  H话务比,
	  数据业务占比,
	  EDGE下行速率,
	  EDGE每线下行用户
	  into "+siteno+@"
      from hedian
	  ";
	  this.ExecuteCommand(drop);
	  this.ExecuteCommand(create);
}

// Define other methods and classes here