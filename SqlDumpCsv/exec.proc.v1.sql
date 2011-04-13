use RedianHedian
exec stat_hot_gprs '[PS总流量] desc','10000','PS总流量10000'
exec stat_hot_gprs '[平均分配PDCH] desc','10000','平均分配PDCH10000'
exec stat_hot_gprs '[PS立即指派成功率] asc','10000','PS立即指派成功率10000'
exec stat_hot_gprs '[上行TBF建立成功率] asc','10000','上行TBF建立成功率10000'
exec stat_hot_gprs '[下行TBF建立成功率] asc','10000','下行TBF建立成功率10000'
exec stat_hot_gprs '[上行TBF掉线率] asc','10000','上行TBF掉线率10000'
exec stat_hot_gprs '[EDGE下行速率] asc','10000','EDGE下行速率10000'



指标	指标定义	目标
上行TBF接入成功率	1-(PREJTFI+PREJOTH)/PSCHREQ×100%	≥96%
下行TBF接入成功率	1-FAILDLTBFEST/DLTBFEST×100%	≥96%
PS立即指派成功率	1-REJPSIMMASS/(PSIMMASS+REJPSIMMASS)×100%	≥96%
上行TBF掉线率	IAULREL/(PSCHREQ-PREJTFI-PREJOTH)×100%	≤1.5%
EDGE下行速率	RLC层平均吞吐率	≥80kbit/s
投诉黑点解决率	黑点问题解决数/黑点问题总数×100%	≥90%
数据业务黑点的优化规范

use RedianHedian
drop table [西湖村M3]
go
select  * into [西湖村M3]
from [RedianHedian].[dbo].[gprsredian]
where 小区名='西湖村M3'
--
 use RedianHedian
 declare @sqld varchar(8000)
 set @sqld= 'bcp RedianHedian..西湖村M3 out c:\西湖村M3.csv -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqld
 --
 use RedianHedian
 select convert(varchar,MIN(时间),20)+'~'
 +convert(varchar,MAX(时间),20)  from dbo.西湖村M3
