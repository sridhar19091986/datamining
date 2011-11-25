<Query Kind="Statements">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
    <Database>imm_jm</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var im=from p in Abis_Imms
//       where p.Tn==null
//	   where p.PS_IMM==null
//	   where p.REJ_IMM==null
       where p.CS_IMM !=null
	   orderby p.CS_IMM_delayFirst descending
	  //where p.REJ_IMM !=null
	  //where p.Wait_indication==null
	   select p;
	
	
	im.Dump();