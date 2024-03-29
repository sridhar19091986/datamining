use tempdb
go
drop view vw_bcpMasterSysobjects
go
create view vw_bcpMasterSysobjects
as select 
       [GPRS下行流量]='GPRS下行流量'+cast(floor([GPRS下行流量]/33212.22) as nvarchar)
      ,[GPRS总流量]='GPRS总流量'+cast(floor([GPRS总流量]/36036.8) as nvarchar)
      ,[EDGE下行流量]='EDGE下行流量'+cast(floor([GPRS下行流量]/44319.86) as nvarchar)
      ,[EDGE总流量]='EDGE总流量'+cast(floor([EDGE总流量]/51550.08) as nvarchar)
      ,[PS下行流量]='PS下行流量'+cast(floor([PS下行流量]/20164.68) as nvarchar)
      ,[PS总流量]='PS总流量'+cast(floor([PS总流量]/21407.48) as nvarchar)
      ,[GPRS下行速率]='GPRS下行速率]'+cast(floor([GPRS下行速率]/0.617902) as nvarchar)
      ,[EDGE下行速率]='EDGE下行速率'+cast(floor([EDGE下行速率]/3.153366) as nvarchar)
      ,[EDGE每信道下行速率]='EDGE每信道下行速率'+cast(floor([EDGE每信道下行速率]/1.105154) as nvarchar)
      ,[GPRS每信道下行速率]='GPRS每信道下行速率'+cast(floor([GPRS每信道下行速率]/0.032104) as nvarchar)
      ,[GPRS每PFC下行流量]='GPRS每PFC下行流量'+cast(floor([GPRS每PFC下行流量]/0.729792) as nvarchar)
      ,[EDGE每PFC下行流量]='EDGE每PFC下行流量'+cast(floor([EDGE每PFC下行流量]/0.697372) as nvarchar)
      ,[下行TBF建立成功率]='下行TBF建立成功率'+cast(floor([下行TBF建立成功率]/1.280918) as nvarchar)
      ,[上行TBF建立成功率]='上行TBF建立成功率'+cast(floor([上行TBF建立成功率]/1.05192) as nvarchar)
      ,[上行TBF异常释放]='上行TBF异常释放'+cast(floor([上行TBF异常释放]/5.98) as nvarchar)
      ,[PS立即指派成功率]='PS立即指派成功率'+cast(floor([PS立即指派成功率]/1.255536) as nvarchar)
      ,[上行TBF掉线率]='上行TBF掉线率'+cast(floor([上行TBF掉线率]/0.006236) as nvarchar)
      ,[平均分配PDCH]='平均分配PDCH'+cast(floor([平均分配PDCH]/1.0365) as nvarchar)
      ,[平均使用PDCH]='平均使用PDCH'+cast(floor([平均使用PDCH]/1.144444) as nvarchar)
      ,[GPRS下行激活信道]='GPRS下行激活信道'+cast(floor([GPRS下行激活信道]/1.168722) as nvarchar)
      ,[GPRS每线下行用户]='GPRS每线下行用户'+cast(floor([GPRS每线下行用户]/0.192506) as nvarchar)
      ,[EDGE下行激活信道]='EDGE下行激活信道'+cast(floor([EDGE下行激活信道]/0.504778) as nvarchar)
      ,[EDGE每线下行用户]='EDGE每线下行用户'+cast(floor([EDGE每线下行用户]/0.223866) as nvarchar)
      ,[PDCH复用度]='PDCH复用度'+cast(floor([PDCH复用度]/0.181192) as nvarchar)
 from [RedianHedian].[dbo].[psredian1000]
go
declare @sql varchar(8000)
select @sql = 'bcp tempdb..vw_bcpMasterSysobjects out c:\sysobjects.txt -c -t, -T -S' + @@servername
exec master..xp_cmdshell @sql

