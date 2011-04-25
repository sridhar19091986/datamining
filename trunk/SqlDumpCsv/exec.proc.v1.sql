use RedianHedian
exec stat_hot_gprs '[PS������] desc','10000','PS������10000'
exec stat_hot_gprs '[ƽ������PDCH] desc','10000','ƽ������PDCH10000'
exec stat_hot_gprs '[PS����ָ�ɳɹ���] asc','10000','PS����ָ�ɳɹ���10000'
exec stat_hot_gprs '[����TBF�����ɹ���] asc','10000','����TBF�����ɹ���10000'
exec stat_hot_gprs '[����TBF�����ɹ���] asc','10000','����TBF�����ɹ���10000'
exec stat_hot_gprs '[����TBF������] asc','10000','����TBF������10000'
exec stat_hot_gprs '[EDGE��������] asc','10000','EDGE��������10000'

exec ExtractTable 'gprsredian_ps','[PS������] desc','10000','PS������10000'
exec ExtractTable 'BSCQOS','[ALLPDCHPCUFAIL] desc','10000','ALLPDCHPCUFAIL10000'
exec ExtractTable 'gprsredian_ps_bsc','[ALLPDCHPCUFAIL] desc','10000','ALLPDCHPCUFAIL10000'


ָ��	ָ�궨��	Ŀ��
����TBF����ɹ���	1-(PREJTFI+PREJOTH)/PSCHREQ��100%	��96%
����TBF����ɹ���	1-FAILDLTBFEST/DLTBFEST��100%	��96%
PS����ָ�ɳɹ���	1-REJPSIMMASS/(PSIMMASS+REJPSIMMASS)��100%	��96%
����TBF������	IAULREL/(PSCHREQ-PREJTFI-PREJOTH)��100%	��1.5%
EDGE��������	RLC��ƽ��������	��80kbit/s
Ͷ�ߺڵ�����	�ڵ���������/�ڵ�����������100%	��90%
����ҵ��ڵ���Ż��淶

use RedianHedian
drop table [������M3]
go
select  * into [������M3]
from [RedianHedian].[dbo].[gprsredian]
where С����='������M3'
--
 use RedianHedian
 declare @sqld varchar(8000)
 set @sqld= 'bcp RedianHedian..������M3 out c:\������M3.csv -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqld
 --
 use RedianHedian
 select convert(varchar,MIN(ʱ��),20)+'~'
 +convert(varchar,MAX(ʱ��),20)  from dbo.������M3
--
select * into gprsredian_ps
from (
SELECT b.*,a.PS����ָ���� as PS����ָ����,a.PS����ָ��ʧ���� as   PS����ָ��ʧ����
FROM dbo.PS as a,dbo.gprsredian as b
where a.ʱ��=b.ʱ�� and a.С����=b.С����
) as c
--