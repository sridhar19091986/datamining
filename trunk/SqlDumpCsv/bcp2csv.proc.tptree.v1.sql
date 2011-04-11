use RedianHedian
go
if exists(select * from sysobjects where name='stat_hot_gprs' and type='p')
drop procedure stat_hot_gprs
go
CREATE PROCEDURE stat_hot_gprs(@sortField VARCHAR(50),@sortTopCount VARCHAR(50),@outFile VARCHAR(50))
as 
BEGIN
 --临时表
if exists(select * from sysobjects where id=object_id('RedianHedian..bcpRedianSysobjects'))
drop table bcpRedianSysobjects
if exists(select * from sysobjects where id=object_id('RedianHedian..bcpMasterSysobjects'))
drop table bcpMasterSysobjects
 --输出原始表
 declare @sqlc varchar(8000)
 declare @sqld varchar(8000)
 set @sqlc='select top '+@sortTopCount+' *
            into bcpRedianSysobjects
            from [RedianHedian].[dbo].[gprsredian]
            order by '+@sortField 
 exec(@sqlc)
 set @sqld= 'bcp RedianHedian..bcpRedianSysobjects out c:\'+@outFile+'.csv -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqld
--统计关联表
 declare @sqla varchar(8000)
 declare @sqlb varchar(8000)
 set @sqla='select 
       floor([GPRS下行流量]/33212.22)+0.001         as   [GPRS下行流量]
      ,floor([GPRS总流量]/36036.8)+0.002            as   [GPRS总流量]
      ,floor([GPRS下行流量]/44319.86)+0.003         as   [EDGE下行流量]
      ,floor([EDGE总流量]/51550.08)+0.004           as   [EDGE总流量]
      ,floor([PS下行流量]/20164.68)+0.005           as   [PS下行流量]
      ,floor([PS总流量]/21407.48)+0.006             as   [PS总流量]
      ,floor([GPRS下行速率]/0.617902)+0.007         as   [GPRS下行速率]
      ,floor([EDGE下行速率]/3.153366)+0.008         as   [EDGE下行速率]
      ,floor([EDGE每信道下行速率]/1.105154)+0.009   as   [EDGE每信道下行速率]
      ,floor([GPRS每信道下行速率]/0.032104)+0.010   as   [GPRS每信道下行速率]
      ,floor([GPRS每PFC下行流量]/0.729792)+0.011    as   [GPRS每PFC下行流量]
      ,floor([EDGE每PFC下行流量]/0.697372)+0.012    as   [EDGE每PFC下行流量]
      ,floor([下行TBF建立成功率]/1.280918)+0.013    as   [下行TBF建立成功率]
      ,floor([上行TBF建立成功率]/1.05192)+0.014     as   [上行TBF建立成功率]
      ,floor([上行TBF异常释放]/5.98)+0.015          as   [上行TBF异常释放]
      ,floor([PS立即指派成功率]/1.255536)+0.016     as   [PS立即指派成功率]
      ,floor([上行TBF掉线率]/0.006236)+0.017        as   [上行TBF掉线率]
      ,floor([平均分配PDCH]/1.0365)+0.018           as   [平均分配PDCH]
      ,floor([平均使用PDCH]/1.144444)+0.019         as   [平均使用PDCH]
      ,floor([GPRS下行激活信道]/1.168722)+0.020     as   [GPRS下行激活信道]
      ,floor([GPRS每线下行用户]/0.192506)+0.021     as   [GPRS每线下行用户]
      ,floor([EDGE下行激活信道]/0.504778)+0.022     as   [EDGE下行激活信道]
      ,floor([EDGE每线下行用户]/0.223866)+0.023     as   [EDGE每线下行用户]
      ,floor([PDCH复用度]/0.181192)+0.024           as  [PDCH复用度]
      into bcpMasterSysobjects
      from bcpRedianSysobjects'
 exec(@sqla)
 set @sqlb= 'bcp RedianHedian..bcpMasterSysobjects out c:\'+@outFile+'.txt -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqlb
End

