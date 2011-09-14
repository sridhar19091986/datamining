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

double based=1000000.0;
var tbf=from p in CELLGPRS2_0908s
        where p.时间.Value.Hour==20
        group p by 1==1 into ttt
		select new 
		{
		  BSC=ttt.Key,
		  DLTBFEST=1.0*ttt.Sum(e=>e.DLTBFEST/based),
		  小区清空BUFFER总数=1.0*ttt.Sum(e=>e.小区清空BUFFER总数/based),
		  FAILDLANSW=1.0*ttt.Sum(e=>e.FAILDLANSW/based),
		  FAILDLTBFEST=1.0*ttt.Sum(e=>e.FAILDLTBFEST/based),
		  FAILDLANSW下行TBF申请占比=1.0*ttt.Sum(e=>e.FAILDLANSW/based)/ttt.Sum(e=>e.下行TBF申请/based),
		  FAILDLANSW下行TBF失败占比=1.0*ttt.Sum(e=>e.FAILDLANSW/based)/(ttt.Sum(e=>e.FAILDLANSW/based)+ttt.Sum(e=>e.FAILDLTBFEST/based)),
		  LDISEST=1.0*ttt.Sum(e=>e.LDISEST/based),
		  LDISTFI=1.0*ttt.Sum(e=>e.LDISTFI/based)/ttt.Sum(e=>e.小区清空BUFFER总数/based),
		  LDISRR=1.0*ttt.Sum(e=>e.LDISRR/based)/ttt.Sum(e=>e.小区清空BUFFER总数/based),
		  LDISOTH=1.0*ttt.Sum(e=>e.LDISOTH/based)/ttt.Sum(e=>e.小区清空BUFFER总数/based),
		  FLUDISC=1.0*ttt.Sum(e=>e.FLUDISC/based)/ttt.Sum(e=>e.小区清空BUFFER总数/based)
		};
	tbf.Dump();