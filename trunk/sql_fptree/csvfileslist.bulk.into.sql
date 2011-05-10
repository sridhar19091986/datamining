<Query Kind="Program">
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{

	string[]   dirs   =   Directory.GetFiles(@"D:\csv文件\csv文件\0");

	foreach   (string   dir   in   dirs)
	{
	 string bulk = @"BULK INSERT gsmredian
				  FROM '"+dir+@"'
				  WITH
				  (
				  FIRSTROW = 2,
				  FIELDTERMINATOR = ',',
				  ROWTERMINATOR = '\n'
				  )
				  ";
		bulk.Dump();
	}

}

