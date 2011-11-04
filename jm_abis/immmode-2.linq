<Query Kind="Statements">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
    <Database>SqlSpatialTest</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var cr=from p in Abis_Imms
       where p.Wait_indication!=null
	   group p by p.Wait_indication into ttt
	   select new {
	   IMMEDIATE_ASSIGN_COMMAND_Wait_indication=ttt.Key,
	   //imm_mode=ttt.Key.Imm_ta,
	   valueCount=ttt.Count(),
	   valueRate=1.0*ttt.Count()/Abis_Imms.Where(e=>e.Wait_indication!=null).Count(),
	    valueDelay=ttt.Average(e=>e.IMMEDIATE_ASSIGN_COMMAND_delayFirst),
	   };
//var abc=cr.ToList();
	   cr.ToList().OrderByDescending(e=>e.valueCount).Dump();
	   
	   Abis_Imms.Where(e=>e.Wait_indication!=null).Take(10).Dump();