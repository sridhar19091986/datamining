

select distinct Ê±¼ä,BSC,
RPP0040,RPP4160,RPP6180,RPP8190,RPP9100,
REJPSIMMASS,PREJOTH
into gprsredian_ps_bsc_dist
from gprsredian_ps_bsc
go
USE [RedianHedian]
drop table PREJOTH10000
go
go
exec ExtractTable 'gprsredian_ps_bsc_dist','[PREJOTH] desc','10000','PREJOTH10000'
