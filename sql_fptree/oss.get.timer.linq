<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var oss=from p in 全网EDGE终端比例s
        select p;
var oss1=from p in Gprsredians
		select p;
var oss2=from p in Gprsredian_ps
		select p;		
var timer=oss.Min(e=>e.时间).ToString()+"----"+oss.Max(e=>e.时间).ToString();
var timer1=oss.Min(e=>e.时间).ToString()+"----"+oss.Max(e=>e.时间).ToString();
var timer2=oss.Min(e=>e.时间).ToString()+"----"+oss.Max(e=>e.时间).ToString();
timer.Dump();
timer1.Dump();
timer2.Dump();