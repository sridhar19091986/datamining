<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>mytest</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>


double[] data=new double[4]{0.039,0.09,0.23,0.46};

var dicta = new Dictionary<string,int >();
var dictb=new Dictionary<string,double >();
var edge=from p in PS总流量10000s  select new {a=p.上行TBF掉线率};
	dicta.Add("< "+data[0].ToString(),	edge.Where(e=>e.a< data[0]).Count());
	dictb.Add("< "+data[0].ToString(),1.0*edge.Where(e=>e.a< data[0]).Count()/edge.Count());
	dicta.Add(data[0].ToString()+" - "+data[1].ToString(),edge.Where(e=>e.a>=data[0] && e.a < data[1]).Count());
	dictb.Add(data[0].ToString()+" - "+data[1].ToString(),1.0*edge.Where(e=>e.a>=data[0] && e.a < data[1]).Count()/edge.Count());
	dicta.Add(data[1].ToString()+" - "+data[2].ToString(),edge.Where(e=>e.a>=data[1] && e.a < data[2]).Count());
	dictb.Add(data[1].ToString()+" - "+data[2].ToString(),1.0*edge.Where(e=>e.a>=data[1] && e.a < data[2]).Count()/edge.Count());
	dicta.Add(data[2].ToString()+" - "+data[3].ToString(),edge.Where(e=>e.a>=data[2] && e.a < data[3]).Count());
	dictb.Add(data[2].ToString()+" - "+data[3].ToString(),1.0*edge.Where(e=>e.a>=data[2] && e.a < data[3]).Count()/edge.Count());
	dicta.Add(">= "+data[3].ToString(),	edge.Where(e=>e.a>= data[3]).Count());
	dictb.Add(">= "+data[3].ToString(),1.0*edge.Where(e=>e.a>= data[3]).Count()/edge.Count());	
	dicta.Dump();
	dictb.Dump();