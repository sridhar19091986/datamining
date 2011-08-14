<Query Kind="SQL">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

SELECT [t0].[DLTBFEST], [t0].[FAILDLTBFEST], [t0].[PSCHAREQ], [t0].[PREJTFI], [t0].[PREJOTH], [t0].[TTime], [t0].[BSC], [t1].[时间], [t1].[BSC] AS [BSC2], [t1].[PCU分配失败数], [t1].[PCU拥塞率], [t1].[设备利用率], [t1].[上行TBF建立成功率], [t1].[下行TBF建立成功率], [t1].[RPP告警次数], [t1].[GSLSCAN], [t1].[GSLUTIL], [t1].[GSL0040], [t1].[GSL4160], [t1].[GSL6180], [t1].[GSL8190], [t1].[GSL9100], [t1].[FAILMOVECELL], [t1].[RPP0040], [t1].[RPP4160], [t1].[RPP6180], [t1].[RPP8190], [t1].[RPP9100], [t1].[LCCELLMOV], [t1].[LCCELLMOVREJ], [t1].[LCHIRPPLOAD], [t1].[LCMSSUPRFC], [t1].[LCPARREJ], [t1].[LCRELBUSYHI3], [t1].[LCRELIDLEHI3], [t1].[RP_NUM]

,100.00*([t1].[RPP8190]+[t1].[RPP9100])/([t1].[RPP0040]+[t1].[RPP4160]+[t1].[RPP6180]+[t1].[RPP8190]+[t1].[RPP9100]) as RPP

into BSCDLTBF_Analysis_0715

FROM [ALLCELLGPRS2_20110715] AS [t0]
INNER JOIN [BSCGPRSCHECK_20110715] AS [t1] ON ([t0].[BSC] = [t1].[BSC]) AND ([t0].[TTime] = [t1].[时间])

where ([t1].[RPP0040]+[t1].[RPP4160]+[t1].[RPP6180]+[t1].[RPP8190]+[t1].[RPP9100])>0