USE [RedianHedian]
go
drop table mytest.dbo.gprsredian_ps
go
select * into mytest.dbo.gprsredian_ps
from (
SELECT a.*,
--ʱ��]
--С����]
--С����]
--BSC]
b.GPRS��������,	b.GPRS������,	b.EDGE��������,	b.EDGE������,	b.PS��������,	
b.PS������,	b.GPRS��������,	b.EDGE��������,	b.EDGEÿ�ŵ���������,	b.GPRSÿ�ŵ���������,	
b.GPRSÿPFC��������,	b.EDGEÿPFC��������,	b.����TBF�����ɹ���,	b.����TBF�����ɹ���,
b.����TBF�쳣�ͷ�,	b.PS����ָ�ɳɹ���,	b.����TBF������,	
b.ƽ������PDCH,	b.ƽ��ʹ��PDCH,	b.GPRS���м����ŵ�,	b.GPRSÿ�������û�,	
b.EDGE���м����ŵ�,	b.EDGEÿ�������û�,	b.PDCH���ö�,	
b.PSIMMASS,	b.REJPSIMMASS,	b.PSCHREQ,	b.PREJTFI,	b.PREJOTH,	b.FAILDLTBFEST
FROM dbo.gsmredian as a,dbo.gprsredian_new b
where a.ʱ��=b.ʱ�� and a.С����=b.С����
) as c


