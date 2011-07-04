<Query Kind="SQL">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

SELECT * into BSCGPRS2_20110630
FROM OpenDataSource( 'Microsoft.ACE.OLEDB.12.0',
'Data Source="d:\bsc指标_2011-0623-0630.xlsx";User ID=Admin;Password=;Extended properties=EXCEL 12.0')...[Sheet1$]