<Query Kind="Statements">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
    <Database>AbisJiangMeng</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var cr=from p in Abis_Imms

       group p by p.Cr_access_delay into ttt
	   
	   select new {
	   CHANnel_ReQuireD_Access_Delay=ttt.Key,
	   RACH_min_distance=0.554*Convert.ToDouble(ttt.Key),
	   valueCount=ttt.Count(),
	   
	   valueRate=1.0*ttt.Count()/Abis_Imms.Count()
	   
	   };
//var abc=cr.ToList();

	   cr.ToList().OrderBy(e=>Convert.ToDouble(e.CHANnel_ReQuireD_Access_Delay)).Dump();