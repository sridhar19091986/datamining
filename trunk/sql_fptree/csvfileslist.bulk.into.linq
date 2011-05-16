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
	string[]   dirs   =   Directory.GetFiles(@"D:\新建文件夹\新建文件夹");
	foreach   (string   dir   in   dirs)
	{
	 string bulk = @"BULK INSERT gprsredian
				  FROM '"+dir+@"'
				  WITH
				  (
				  FIRSTROW = 2,
				  FIELDTERMINATOR = ',',
				  ROWTERMINATOR = '\n'
				  ); ";
		this.ExecuteCommand(bulk);	
//		this.ExecuteQuery(bulk);
		bulk.Dump();
	}
	
}