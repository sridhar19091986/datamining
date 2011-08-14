<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>CellLoadSharing</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var lsa=from p in A_SoLSAs
        where p.Solsa !=null
        group p by new {p.SoLSA_m_MsgType,p.Solsa} into ttt
		select new 
		{
		SoLSA_Message=ttt.Key.SoLSA_m_MsgType,
		SoLSA_Value=ttt.Key.Solsa,
		mintime=ttt.Min(e=>e.PacketTime),
		maxtime=ttt.Max(e=>e.PacketTime),
		ttimer=ttt.Max(e=>e.PacketTime)-ttt.Min(e=>e.PacketTime),
		TMSI_count=ttt.Count()
		};
		
	lsa.Dump();