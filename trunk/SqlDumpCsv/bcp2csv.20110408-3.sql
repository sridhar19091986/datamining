use tempdb
go
drop view vw_bcpMasterSysobjects
go
create view vw_bcpMasterSysobjects
as select top 10000
       [GPRS下行流量]=floor([GPRS下行流量]/33212.22)+0.001
      ,[GPRS总流量]=floor([GPRS总流量]/36036.8)+0.002
      ,[EDGE下行流量]=floor([GPRS下行流量]/44319.86)+0.003
      ,[EDGE总流量]=floor([EDGE总流量]/51550.08)+0.004
      ,[PS下行流量]=floor([PS下行流量]/20164.68)+0.005
      ,[PS总流量]=floor([PS总流量]/21407.48)+0.006
      ,[GPRS下行速率]=floor([GPRS下行速率]/0.617902)+0.007
      ,[EDGE下行速率]=floor([EDGE下行速率]/3.153366)+0.008
      ,[EDGE每信道下行速率]=floor([EDGE每信道下行速率]/1.105154)+0.009
      ,[GPRS每信道下行速率]=floor([GPRS每信道下行速率]/0.032104)+0.010
      ,[GPRS每PFC下行流量]=floor([GPRS每PFC下行流量]/0.729792)+0.011
      ,[EDGE每PFC下行流量]=floor([EDGE每PFC下行流量]/0.697372)+0.012
      ,[下行TBF建立成功率]=floor([下行TBF建立成功率]/1.280918)+0.013
      ,[上行TBF建立成功率]=floor([上行TBF建立成功率]/1.05192)+0.014
      ,[上行TBF异常释放]=floor([上行TBF异常释放]/5.98)+0.015
      ,[PS立即指派成功率]=floor([PS立即指派成功率]/1.255536)+0.016
      ,[上行TBF掉线率]=floor([上行TBF掉线率]/0.006236)+0.017
      ,[平均分配PDCH]=floor([平均分配PDCH]/1.0365)+0.018
      ,[平均使用PDCH]=floor([平均使用PDCH]/1.144444)+0.019
      ,[GPRS下行激活信道]=floor([GPRS下行激活信道]/1.168722)+0.020
      ,[GPRS每线下行用户]=floor([GPRS每线下行用户]/0.192506)+0.021
      ,[EDGE下行激活信道]=floor([EDGE下行激活信道]/0.504778)+0.022
      ,[EDGE每线下行用户]=floor([EDGE每线下行用户]/0.223866)+0.023
      ,[PDCH复用度]=floor([PDCH复用度]/0.181192)+0.024
 from [RedianHedian].[dbo].[gprsredian]
 order by [PS总流量] desc
go
declare @sql varchar(8000)
select @sql = 'bcp tempdb..vw_bcpMasterSysobjects out c:\sysobjects.txt -c -t, -T -S' + @@servername
exec master..xp_cmdshell @sql