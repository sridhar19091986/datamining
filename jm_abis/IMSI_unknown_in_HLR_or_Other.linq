<Query Kind="Statements">
  <Connection>
    <ID>76932288-1882-4022-a8fb-c01693c0810c</ID>
    <Persist>true</Persist>
    <Provider>Devart.Data.MySql</Provider>
    <CustomCxString>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAACiCz6fEkH469J/EfadE5bbAAAAAASAAACgAAAAEAAAAA79yvaUcfgezEFsepufT99AAAAAbde1n+Xb2JSTWeHnLtJn7AJnlFc/Ofn9rrM7q0XPNd6/6NElIyt853XHqyewFu3xdtnCU4IW0mj8CwbouIFAuRQAAAD0I/FHWDdneXeyR/kT4lQDV9Gb/A==</CustomCxString>
    <Server>localhost</Server>
    <Database>mysql</Database>
    <UserName>root</UserName>
    <Password>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAADkrYJ5ii5dOlB54Q4g4VNRAAAAAASAAACgAAAAEAAAALRZpGA5TrMbTQf5Q7s/6/IIAAAAo4mX5DXQQiEUAAAArcrXZk87oLcEm3N3crj52U5a5Ak=</Password>
    <EncryptCustomCxString>true</EncryptCustomCxString>
    <DriverData>
      <StripUnderscores>false</StripUnderscores>
      <QuietenAllCaps>false</QuietenAllCaps>
      <Port>3306</Port>
    </DriverData>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

var imsi=from p in Tmp_tables

         group p by p.Imsi into ttt
		 
		 select new 
		 
		 {
		    IMSI_unknown_in_HLR_or_Other=ttt.Key,
			
			Location_Updating_Reject_Count=ttt.Count()
		 
		 };
		 
	imsi.OrderByDescending(e=>e.Location_Updating_Reject_Count).Dump();