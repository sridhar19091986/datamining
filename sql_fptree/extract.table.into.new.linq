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
	  string drop=@"drop table hedian上行TBF掉线率10000";
	  string create=@"
	  select top 10000 * 
	  into hedian上行TBF掉线率10000
      from hedian
      order by 上行TBF掉线率 desc
	  ";
	  //this.ExecuteCommand(drop);
	  this.ExecuteCommand(create);
}

// Define other methods and classes here