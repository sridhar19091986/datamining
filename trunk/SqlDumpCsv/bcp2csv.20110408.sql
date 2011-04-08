use tempdb
go
drop view vw_bcpMasterSysobjects
go
create view vw_bcpMasterSysobjects
as select 
       [GPRS��������]='a'+cast(floor([GPRS��������]/33212.22) as nvarchar)
      ,[GPRS������]='b'+cast(floor([GPRS������]/36036.8) as nvarchar)
      ,[EDGE��������]='c'+cast(floor([GPRS��������]/44319.86) as nvarchar)
      ,[EDGE������]='d'+cast(floor([EDGE������]/51550.08) as nvarchar)
      ,[PS��������]='e'+cast(floor([PS��������]/20164.68) as nvarchar)
      ,[PS������]='f'+cast(floor([PS������]/21407.48) as nvarchar)
      ,[GPRS��������]='g'+cast(floor([GPRS��������]/0.617902) as nvarchar)
      ,[EDGE��������]='h'+cast(floor([EDGE��������]/3.153366) as nvarchar)
      ,[EDGEÿ�ŵ���������]='i'+cast(floor([EDGEÿ�ŵ���������]/1.105154) as nvarchar)
      ,[GPRSÿ�ŵ���������]='j'+cast(floor([GPRSÿ�ŵ���������]/0.032104) as nvarchar)
      ,[GPRSÿPFC��������]='k'+cast(floor([GPRSÿPFC��������]/0.729792) as nvarchar)
      ,[EDGEÿPFC��������]='l'+cast(floor([EDGEÿPFC��������]/0.697372) as nvarchar)
      ,[����TBF�����ɹ���]='m'+cast(floor([����TBF�����ɹ���]/1.280918) as nvarchar)
      ,[����TBF�����ɹ���]='n'+cast(floor([����TBF�����ɹ���]/1.05192) as nvarchar)
      ,[����TBF�쳣�ͷ�]='o'+cast(floor([����TBF�쳣�ͷ�]/5.98) as nvarchar)
      ,[PS����ָ�ɳɹ���]='p'+cast(floor([PS����ָ�ɳɹ���]/1.255536) as nvarchar)
      ,[����TBF������]='q'+cast(floor([����TBF������]/0.006236) as nvarchar)
      ,[ƽ������PDCH]='r'+cast(floor([ƽ������PDCH]/1.0365) as nvarchar)
      ,[ƽ��ʹ��PDCH]='s'+cast(floor([ƽ��ʹ��PDCH]/1.144444) as nvarchar)
      ,[GPRS���м����ŵ�]='t'+cast(floor([GPRS���м����ŵ�]/1.168722) as nvarchar)
      ,[GPRSÿ�������û�]='u'+cast(floor([GPRSÿ�������û�]/0.192506) as nvarchar)
      ,[EDGE���м����ŵ�]='v'+cast(floor([EDGE���м����ŵ�]/0.504778) as nvarchar)
      ,[EDGEÿ�������û�]='w'+cast(floor([EDGEÿ�������û�]/0.223866) as nvarchar)
      ,[PDCH���ö�]='x'+cast(floor([PDCH���ö�]/0.181192) as nvarchar)
 from [RedianHedian].[dbo].[psredian1000]
go
declare @sql varchar(8000)
select @sql = 'bcp tempdb..vw_bcpMasterSysobjects out c:\sysobjects.txt -c -t, -T -S' + @@servername
exec master..xp_cmdshell @sql

