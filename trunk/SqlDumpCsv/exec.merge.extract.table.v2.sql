USE [RedianHedian]
go
drop table mytest.dbo.gprsredian_ps
go
select * into mytest.dbo.gprsredian_ps
from (
SELECT a.*,
--时间]
--小区名]
--小区号]
--BSC]
b.GPRS下行流量,	b.GPRS总流量,	b.EDGE下行流量,	b.EDGE总流量,	b.PS下行流量,	
b.PS总流量,	b.GPRS下行速率,	b.EDGE下行速率,	b.EDGE每信道下行速率,	b.GPRS每信道下行速率,	
b.GPRS每PFC下行流量,	b.EDGE每PFC下行流量,	b.下行TBF建立成功率,	b.上行TBF建立成功率,
b.上行TBF异常释放,	b.PS立即指派成功率,	b.上行TBF掉线率,	
b.平均分配PDCH,	b.平均使用PDCH,	b.GPRS下行激活信道,	b.GPRS每线下行用户,	
b.EDGE下行激活信道,	b.EDGE每线下行用户,	b.PDCH复用度,	
b.PSIMMASS,	b.REJPSIMMASS,	b.PSCHREQ,	b.PREJTFI,	b.PREJOTH,	b.FAILDLTBFEST
FROM dbo.gsmredian as a,dbo.gprsredian_new b
where a.时间=b.时间 and a.小区号=b.小区号
) as c


