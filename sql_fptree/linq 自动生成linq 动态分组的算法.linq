<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>JiangMengGnInterface</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

string linqdump="";
Type type = typeof(Gn_Connect1x);
foreach (var fields in type.GetFields())
{
	string field = "p."+fields.Name;
	linqdump=@"
	var m"+fields.Name+@" = from p in Gn_Connect1xes.ToList()
			   group p by "+field+@" into ttt
			   select new
	{
		"+fields.Name+@" = ttt.Key,
		"+fields.Name+@"_Count = ttt.Count(),
		"+fields.Name+@"_Repeat_Count = ttt.Where(e => e.Connect_Repeat > 0).Count(),
		"+fields.Name+@"_Repeat_Rate = 1.0 * ttt.Where(e => e.Connect_Repeat > 0).Count() / ttt.Count(),
	};
	m"+fields.Name+@".Where(e=>e."+fields.Name+@"_Repeat_Count>0).OrderByDescending(e=>e."+fields.Name+@"_Repeat_Count).Dump();
    ";
	linqdump.Dump();
}