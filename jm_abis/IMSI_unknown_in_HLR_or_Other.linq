<Query Kind="Statements">
  <Connection>
    <ID>76932288-1882-4022-a8fb-c01693c0810c</ID>
    <Persist>true</Persist>
    <Driver Assembly="IQDriver" PublicKeyToken="5b59726538a49684">IQDriver.IQDriver</Driver>
    <Provider>Devart.Data.MySql</Provider>
    <CustomCxString>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAACTNuYXkfk66zdd04KiO8wGAAAAAASAAACgAAAAEAAAAIOK7HFTUbC4NvHwmY54AThAAAAAYKS1JdvPz5iWCN+bVClSqdEdGOTZYMPe388yxJbFLsrQLNQremir6pv+EuhDT42GyAqHjKDtruU3cS1x7UlBXBQAAAA9C6jhKoipfqKkln91vqhRHssrBA==</CustomCxString>
    <Server>localhost</Server>
    <UserName>root</UserName>
    <Password>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAADkrYJ5ii5dOlB54Q4g4VNRAAAAAASAAACgAAAAEAAAALRZpGA5TrMbTQf5Q7s/6/IIAAAAo4mX5DXQQiEUAAAArcrXZk87oLcEm3N3crj52U5a5Ak=</Password>
    <EncryptCustomCxString>true</EncryptCustomCxString>
    <Database>test</Database>
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
		 
		 
		 
