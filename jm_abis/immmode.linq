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

var cr = from p in Abis_Imms
         where p.CS_IMM_delayFirst <1000 || p.CS_IMM_delayFirst ==null
		 group p by new
{
	CS_IMM = p.CS_IMM != null,
	PS_IMM = p.PS_IMM != null,
	REJ_IMM = p.REJ_IMM != null,
	p.Imm_mode,
	Wait_indication = Convert.ToInt32(p.Wait_indication ?? "0") >= 10
}
into ttt
select new
{
	cs_imm = ttt.Key.CS_IMM.GetHashCode(),
	ps_imm = ttt.Key.PS_IMM.GetHashCode(),
	rej_imm = ttt.Key.REJ_IMM.GetHashCode(),
	wait_over_10 = ttt.Key.Wait_indication.GetHashCode(),
	imm_mode = ttt.Key.Imm_mode,
	valueCount = ttt.Count(),
	valueRate = 1.0 * ttt.Count() / Abis_Imms.Count(),
	cs_imm_Delay = ttt.Average(e => e.CS_IMM_delayFirst),
	ps_imm_Delay = ttt.Average(e => e.PS_IMM_delayFirst),
	rej_imm_Delay = ttt.Average(e => e.REJ_IMM_delayFirst),
};
//var abc=cr.ToList();
cr.ToList().OrderByDescending(e => e.valueCount).Dump();
//Abis_Imms.Where(e=>e.Imm_mode==null).Where(e=>e.IMMEDIATE_ASSIGN_COMMAND !=null).Take(1).Dump();
//		Abis_Imms.Where(e=>e.Imm_mode!=null).Where(e=>e.IMMEDIATE_ASSIGN_COMMAND !=null)
//		.Where(e=>e.Imm_mode!="This message assigns a dedicated mode resource")
//		.OrderByDescending(e=>e.IMMEDIATE_ASSIGN_COMMAND_delayFirst).Dump();

//string fil = "This message assigns a downlink TBF to the mobile station identified in the IA Rest Octets IE";

//Abis_Imms.Where(e=>e.Imm_e1!=null).Where(e=>e.Imm_mode==null).Take(10).Dump();