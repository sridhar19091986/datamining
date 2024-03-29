/****** Script for SelectTopNRows command from SSMS  ******/
USE [RedianHedian]
go
drop table temp
go
SELECT [时间],[BSC],sum([REJPSIMMASS]) as REJPSIMMASS,sum([PREJOTH]) as PREJOTH
into temp
FROM [RedianHedian].[dbo].[gprsredian_ps]
group by [时间],[BSC]
go
drop table gprsredian_ps_bsc
go
select distinct * into gprsredian_ps_bsc
from (select b.*,
a.REJPSIMMASS as   REJPSIMMASS,
a.PREJOTH as   PREJOTH from temp as a ,rppbsc as b
where a.时间=b.时间 and a.[BSC]=b.[BSC]) as c

