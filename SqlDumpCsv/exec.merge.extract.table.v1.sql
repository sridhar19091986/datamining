USE [RedianHedian]
go
drop table gprsredian_ps
go
select * into gprsredian_ps
from (
SELECT b.*,
a.PSIMMASS as PPSIMMASS,
a.REJPSIMMASS as   REJPSIMMASS,
a.PSCHREQ as   PSCHREQ,
a.PREJTFI as   PREJTFI,
a.PREJOTH as   PREJOTH
FROM dbo.PS as a,dbo.gprsredian as b
where a.ʱ��=b.ʱ�� and a.С����=b.С����
) as c


