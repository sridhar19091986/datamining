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
      string siteno="樟阁村d3";
	  string drop=@"
                if exists(select * from sysobjects where type = 'U' and name = '"+siteno+@"') 
                drop table "+siteno;
	  string create=@"
	  select 
*
	  into "+siteno+@"
      from gprsredian_ps
	  where 小区名='"+siteno+@"'
      order by 时间 
	  ";
	  this.ExecuteCommand(drop);
	  this.ExecuteCommand(create);
}

// Define other methods and classes here