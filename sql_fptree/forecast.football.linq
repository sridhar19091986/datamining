<Query Kind="Statements">
  <Connection>
    <ID>0d07d716-e2e3-45d3-aaa3-c5ed7010c091</ID>
    <Persist>true</Persist>
    <Provider>SQLCE</Provider>
    <AttachFileName>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\SyncSoccerScore.sdf</AttachFileName>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var match=from p in Match_analysis_results.Take (50)
          join q in Result_tb_libs on p.Result_tb_lib_id  equals q.Result_tb_lib_id 
		  select new 
		  {
		  
		  p.Home_w,
		  p.Home_d,
		  p.Home_l,
		  p.Home_goals,
		  p.Away_goals,
		  q.Full_home_goals,
		  q.Full_away_goals
		  
		  };
		  match.Dump();