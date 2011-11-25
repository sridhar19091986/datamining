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
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var cr=from p in Abis_Imms
       //where p.Imm_mode=="This message assigns a dedicated mode resource"
	   where p.Imm_mode!=null
	   group p by new {ta_diff=p.Cr_access_delay==p.Imm_ta} into ttt
	   select new {
	   IMMEDIATE_ASSIGN_COMMAND_MsgType=ttt.Key.ta_diff,
	   //imm_mode=ttt.Key.Imm_ta,
	   valueCount=ttt.Count(),
	   valueRate=1.0*ttt.Count()/Abis_Imms.Count(),
	    cs_IMM_Delay=ttt.Average(e=>e.CS_IMM_delayFirst),
		 ps_IMM_Delay=ttt.Average(e=>e.PS_IMM_delayFirst),
		  rej_IMM_Delay=ttt.Average(e=>e.REJ_IMM_delayFirst),
	   };
//var abc=cr.ToList();
	   //cr.ToList().OrderByDescending(e=>e.valueCount).Dump();
	     cr.Dump();