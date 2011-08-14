<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var rpp=from p in Bsc_cell_20110729s
        where p.时间.Value.Hour==20
        group p by p.BSC into ttt
		select new 
		{
		BSC=ttt.Key,
		RP_NUM=ttt.Max(e=>e.RP_NUM),
		上行TBF建立成功率=ttt.Average(e=>e.上行TBF建立成功率),
		RPP告警比=ttt.Average(e=>e.RPPr),
		GSL利用率=ttt.Average(e=>e.设备利用率),
		RPPpschreq=ttt.Average(e=>e.RPPpschreq)
		
		};
var rpp1=from p in rpp
       let rp_plan=(int)(((double)(p.RPPpschreq)/43)*p.RP_NUM)+1
        select new 
		{
		p.BSC,
		p.RP_NUM,
		p.上行TBF建立成功率,
		p.RPP告警比,
		p.GSL利用率,
		p.RPPpschreq,
		RPP_NUM需求数=rp_plan,
		RPP_NUM增加数=rp_plan-p.RP_NUM,
		};
		rpp1.OrderBy(e=>e.上行TBF建立成功率).Dump();
		
		//rpp1.OrderByDescending(e=>e.RPP_NUM增加数).Dump();