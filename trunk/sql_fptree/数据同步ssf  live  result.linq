<Query Kind="Statements">
  <Connection>
    <ID>de1efc10-c1b2-46a1-ab93-0ce1e36ac160</ID>
    <Persist>true</Persist>
    <Provider>SQLCE</Provider>
    <AttachFileName>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\SyncSoccerScore.sdf</AttachFileName>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var lt=from p in Match_analysis_results
       //join q in Live_Table_libs on p.Live_table_lib_id equals q.Live_table_lib_id
	   join t in Result_tb_libs on p.Result_tb_lib_id equals t.Result_tb_lib_id
	   select new {p,t};

var nomatch=lt.ToLookup(e=>e.p.Live_table_lib_id);
   //nomatch.Dump();
   int i=0;
   foreach(var p in  Live_Table_libs)
   {
     Console.Write(p.Match_time);
	 if(nomatch.Contains(p.Live_table_lib_id))
     p.Match_time=nomatch[p.Live_table_lib_id].Select(e=>e.t.Match_time).First();
	 Console.WriteLine(p.Match_time);	 
	 Console.Write(i++);
}
this.SubmitChanges();