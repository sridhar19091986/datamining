
USE [RedianHedian]
drop table gprsredian_ps_bsc_dist
go
select  *
into gprsredian_ps_bsc_dist
from gprsredian_ps_bsc
where [BSC]='SZ37A'
go
USE [RedianHedian]
drop table PREJOTH10000
go
go
exec ExtractTable 'gprsredian_ps_bsc_dist','[Ê±¼ä] asc','10000','PREJOTH10000'
