<Query Kind="Program">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
    <Database>SqlSpatialJiangmeng</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{

	
	
}

// Define other methods and classes here
private string getNeighCell(string ServiceCell,string frequency,string bsic)
{
   	var nei=MCOMNEIGHs.ToLookup(e=>e.Cell);
	
	var cdd=MCOMCARRIERs.ToLookup(e=>e.BCCH+"-"+e.BSIC);
	
	var cells=cdd[frequency+bsic];
	
	var ncells=nei[ServiceCell].FirstOrDefault().Ncell.Split(" ");
	
	var ncell=cells.Intersect(ncells).FirstOrDefault();
	
	return ncell;
	
	
}