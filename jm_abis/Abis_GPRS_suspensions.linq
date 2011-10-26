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


var valueSum=Abis_GPRS_suspensions.Where(e=>e.DumpFor=="EndFlowByFlowDesigner").Count();

var sus=from p in Abis_GPRS_suspensions
        //where p.ERROR_INDication !=null
		where p.DumpFor=="EndFlowByFlowDesigner"
        group p by new {p.Suspension_cause,p.ERROR_INDication} into ttt
		select new 
		{
		   ttt.Key.Suspension_cause,
		   ttt.Key.ERROR_INDication,
		   valueCount=ttt.Count(),
		   //valueDelay=ttt.Average(e=>e.PacketTimems),
		   valueRate=1.0*ttt.Count()/valueSum,
		};

sus.OrderByDescending(e=>e.valueCount).Dump();

Abis_GPRS_suspensions.Where(e=>e.ERROR_INDication !=null).Dump();