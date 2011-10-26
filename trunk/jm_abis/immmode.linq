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
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var cr=from p in Abis_Imms
	   group p by new {p.IMMEDIATE_ASSIGN_COMMAND_MsgType,p.Imm_mode} into ttt
	   select new {
	   IMMEDIATE_ASSIGN_COMMAND_MsgType=ttt.Key.IMMEDIATE_ASSIGN_COMMAND_MsgType,
	   imm_mode=ttt.Key.Imm_mode,
	   valueCount=ttt.Count(),
	   valueRate=1.0*ttt.Count()/Abis_Imms.Count(),
	    valueDelay=ttt.Average(e=>e.IMMEDIATE_ASSIGN_COMMAND_delayFirst),
	   };
//var abc=cr.ToList();
	   cr.ToList().OrderByDescending(e=>e.valueCount).Dump();
	    //Abis_Imms.Where(e=>e.Imm_mode==null).Where(e=>e.IMMEDIATE_ASSIGN_COMMAND !=null).Take(1).Dump();
		Abis_Imms.Where(e=>e.Imm_mode!=null).Where(e=>e.IMMEDIATE_ASSIGN_COMMAND !=null)
		.Where(e=>e.Imm_mode!="This message assigns a dedicated mode resource").OrderByDescending(e=>e.IMMEDIATE_ASSIGN_COMMAND_delayFirst).Dump();
		