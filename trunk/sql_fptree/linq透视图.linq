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

var dicta = new Dictionary<string,int >();
var dictb=new Dictionary<string,double >();
var edge=from p in PS总流量10000s  select new {a=p.EDGE下行速率};
	dicta.Add("< 29.8820367616",	edge.Where(e=>e.a< 29.8820367616).Count());
	dictb.Add("< 29.8820367616",1.0*edge.Where(e=>e.a< 29.8820367616).Count()/edge.Count());
	dicta.Add("29.8820367616 - 59.7691382784",	edge.Where(e=>e.a>=29.8820367616 && e.a < 59.7691382784).Count());
	dictb.Add("29.8820367616 - 59.7691382784",1.0*edge.Where(e=>e.a>=29.8820367616 && e.a < 59.7691382784).Count()/edge.Count());
	dicta.Add("59.7691382784 - 82.5278888832",	edge.Where(e=>e.a>=59.7691382784 && e.a <82.5278888832).Count());
	dictb.Add("59.7691382784 - 82.5278888832",1.0*edge.Where(e=>e.a>=59.7691382784 && e.a <82.5278888832).Count()/edge.Count());
	dicta.Add("82.5278888832 - 110.4641005952",	edge.Where(e=>e.a>=82.5278888832 && e.a <110.4641005952).Count());
	dictb.Add("82.5278888832 - 110.4641005952",1.0*edge.Where(e=>e.a>=82.5278888832 && e.a <110.4641005952).Count()/edge.Count());
	dicta.Add(">= 110.4641005952",	edge.Where(e=>e.a>= 110.4641005952).Count());
	dictb.Add(">= 110.4641005952",1.0*edge.Where(e=>e.a>= 110.4641005952).Count()/edge.Count());	
	dicta.Dump();
	dictb.Dump();