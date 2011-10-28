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

var valueSum=Abis_DTMs.Where(e=>e.DumpFor=="EndFlowByFlowDesigner").Count();

var sus=from p in Abis_DTMs
        //where p.ERROR_INDication !=null
		where p.DumpFor=="EndFlowByFlowDesigner"
		//where p.DTMReject==null
        group p by new 
		{ERROR_INDication=p.ERROR_INDication==null?0:1,
		Main_DCCH_Assignment_Command=p.Main_DCCH_Assignment_Command ==null?0:1,
		Packet_Assignment_Command=p.Packet_Assignment_Command==null?0:1,
		DTM_Assignment_Command=p.DTM_Assignment_Command==null?0:1,
		DTM_Reject=p.DTMReject ==null?0:1}
		
		into ttt
		select new 
		{
		  DTM_Request=1,
		   DTM_Reject=ttt.Key.DTM_Reject,
		   //DTM_Assignment_Failure=ttt.Key.DTM_Assignment_Failure,
		   Main_DCCH_Assignment_Command=ttt.Key.Main_DCCH_Assignment_Command,
		   Packet_Assignment_Command=ttt.Key.Packet_Assignment_Command,
		   DTM_Assignment_Command=ttt.Key.DTM_Assignment_Command,
		   ERROR_INDication=ttt.Key.ERROR_INDication,
		   valueCount=ttt.Count(),
		   //valueDelay=ttt.Average(e=>e.PacketTimems),
		   valueRate=1.0*ttt.Count()/valueSum,
		};

sus.OrderByDescending(e=>e.valueCount).OrderBy(e=>e.Main_DCCH_Assignment_Command).OrderBy(e=>e.DTM_Reject).Dump();

//Abis_DTMs.Where(e=>e.ERROR_INDication !=null).Dump();