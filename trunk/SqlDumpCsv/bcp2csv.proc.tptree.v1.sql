use tempdb
go
if exists(select * from sysobjects where name='stat_hot_gprs' and type='p')
drop procedure stat_hot_gprs
go
if exists(select * from sysobjects where name='vw_bcpRedianSysobjects' and type='v')
drop view vw_bcpMasterSysobjects
go
if exists(select * from sysobjects where name='vw_bcpMasterSysobjects' and type='v')
drop view vw_bcpMasterSysobjects
go
CREATE PROCEDURE stat_hot_gprs(@sortField VARCHAR(50),@sortTopCount VARCHAR(50),@outFile VARCHAR(50))
as 
BEGIN
 --���ԭʼ��
 declare @sqlc varchar(8000)
 declare @sqld varchar(8000)
 set @sqlc='create view vw_bcpRedianSysobjects as    
            select top '+@sortTopCount+' *
            from [RedianHedian].[dbo].[gprsredian]
            order by '+@sortField
 exec(@sqlc)
 set @sqld= 'bcp tempdb..vw_bcpRedianSysobjects out c:\'+@outFile+'.csv -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqld
--ͳ�ƹ�����
 declare @sqla varchar(8000)
 declare @sqlb varchar(8000)
 set @sqla='create view vw_bcpMasterSysobjects as      
       select top '+@sortTopCount+'
       [GPRS��������]=floor([GPRS��������]/33212.22)+0.001
      ,[GPRS������]=floor([GPRS������]/36036.8)+0.002
      ,[EDGE��������]=floor([GPRS��������]/44319.86)+0.003
      ,[EDGE������]=floor([EDGE������]/51550.08)+0.004
      ,[PS��������]=floor([PS��������]/20164.68)+0.005
      ,[PS������]=floor([PS������]/21407.48)+0.006
      ,[GPRS��������]=floor([GPRS��������]/0.617902)+0.007
      ,[EDGE��������]=floor([EDGE��������]/3.153366)+0.008
      ,[EDGEÿ�ŵ���������]=floor([EDGEÿ�ŵ���������]/1.105154)+0.009
      ,[GPRSÿ�ŵ���������]=floor([GPRSÿ�ŵ���������]/0.032104)+0.010
      ,[GPRSÿPFC��������]=floor([GPRSÿPFC��������]/0.729792)+0.011
      ,[EDGEÿPFC��������]=floor([EDGEÿPFC��������]/0.697372)+0.012
      ,[����TBF�����ɹ���]=floor([����TBF�����ɹ���]/1.280918)+0.013
      ,[����TBF�����ɹ���]=floor([����TBF�����ɹ���]/1.05192)+0.014
      ,[����TBF�쳣�ͷ�]=floor([����TBF�쳣�ͷ�]/5.98)+0.015
      ,[PS����ָ�ɳɹ���]=floor([PS����ָ�ɳɹ���]/1.255536)+0.016
      ,[����TBF������]=floor([����TBF������]/0.006236)+0.017
      ,[ƽ������PDCH]=floor([ƽ������PDCH]/1.0365)+0.018
      ,[ƽ��ʹ��PDCH]=floor([ƽ��ʹ��PDCH]/1.144444)+0.019
      ,[GPRS���м����ŵ�]=floor([GPRS���м����ŵ�]/1.168722)+0.020
      ,[GPRSÿ�������û�]=floor([GPRSÿ�������û�]/0.192506)+0.021
      ,[EDGE���м����ŵ�]=floor([EDGE���м����ŵ�]/0.504778)+0.022
      ,[EDGEÿ�������û�]=floor([EDGEÿ�������û�]/0.223866)+0.023
      ,[PDCH���ö�]=floor([PDCH���ö�]/0.181192)+0.024
 from tempdb..vw_bcpRedianSysobjects
 order by '+@sortField
 exec(@sqla)
 set @sqlb= 'bcp tempdb..vw_bcpMasterSysobjects out c:\'+@outFile+'.txt -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqlb
End
