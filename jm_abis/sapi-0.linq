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

var sapi=from p in Abis_Imms       
		 group p by new 
		 {
		 sapi=p.Sapi==null?0:1,
		 wait_indication=p.Wait_indication==null?0:1,
		 ps_imm=p.PS_IMM==null?0:1,
		 cs_imm=p.CS_IMM==null?0:1
		 } into ttt	 
		 select new 		 
		 {
		    channelRequired=1,
			rej_imm=ttt.Key.wait_indication,
			ttt.Key.ps_imm,
			ttt.Key.cs_imm,
			sd_access=ttt.Key.sapi,
			cs_imm_delay=ttt.Average(e=>e.CS_IMM_delayFirst),
			sd_access_delay=ttt.Average(e=>e.RSL_SAPI_delayFirst),
			valueCount=ttt.Count(),
			valueRate=1.0*ttt.Count()/ Abis_Imms.Count()
		 };
sapi.Dump();
//var tbf=@"This message assigns an uplink TBF or is the second message of two in a two-message assignment of an uplink or downlink TBF"; 
//sapi.Dump();Abis_Imms.Where(e=>e.Imm_mode!=tbf).Take(10).Dump();
//Abis_Imms.Dump();
//.Where(e=>e.Sapi!=null).Where(e=>e.Sapi==3).Dump();

//Abis_Imms.Where(e=>e.Wait_indication