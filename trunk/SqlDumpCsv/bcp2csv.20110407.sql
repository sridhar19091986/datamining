use tempdb
go
drop view vw_bcpMasterSysobjects
go
create view vw_bcpMasterSysobjects
as select 
       [GPRS��������]='GPRS��������'+cast(floor([GPRS��������]/33212.22) as nvarchar)
      ,[GPRS������]='GPRS������'+cast(floor([GPRS������]/36036.8) as nvarchar)
      ,[EDGE��������]='EDGE��������'+cast(floor([GPRS��������]/44319.86) as nvarchar)
      ,[EDGE������]='EDGE������'+cast(floor([EDGE������]/51550.08) as nvarchar)
      ,[PS��������]='PS��������'+cast(floor([PS��������]/20164.68) as nvarchar)
      ,[PS������]='PS������'+cast(floor([PS������]/21407.48) as nvarchar)
      ,[GPRS��������]='GPRS��������]'+cast(floor([GPRS��������]/0.617902) as nvarchar)
      ,[EDGE��������]='EDGE��������'+cast(floor([EDGE��������]/3.153366) as nvarchar)
      ,[EDGEÿ�ŵ���������]='EDGEÿ�ŵ���������'+cast(floor([EDGEÿ�ŵ���������]/1.105154) as nvarchar)
      ,[GPRSÿ�ŵ���������]='GPRSÿ�ŵ���������'+cast(floor([GPRSÿ�ŵ���������]/0.032104) as nvarchar)
      ,[GPRSÿPFC��������]='GPRSÿPFC��������'+cast(floor([GPRSÿPFC��������]/0.729792) as nvarchar)
      ,[EDGEÿPFC��������]='EDGEÿPFC��������'+cast(floor([EDGEÿPFC��������]/0.697372) as nvarchar)
      ,[����TBF�����ɹ���]='����TBF�����ɹ���'+cast(floor([����TBF�����ɹ���]/1.280918) as nvarchar)
      ,[����TBF�����ɹ���]='����TBF�����ɹ���'+cast(floor([����TBF�����ɹ���]/1.05192) as nvarchar)
      ,[����TBF�쳣�ͷ�]='����TBF�쳣�ͷ�'+cast(floor([����TBF�쳣�ͷ�]/5.98) as nvarchar)
      ,[PS����ָ�ɳɹ���]='PS����ָ�ɳɹ���'+cast(floor([PS����ָ�ɳɹ���]/1.255536) as nvarchar)
      ,[����TBF������]='����TBF������'+cast(floor([����TBF������]/0.006236) as nvarchar)
      ,[ƽ������PDCH]='ƽ������PDCH'+cast(floor([ƽ������PDCH]/1.0365) as nvarchar)
      ,[ƽ��ʹ��PDCH]='ƽ��ʹ��PDCH'+cast(floor([ƽ��ʹ��PDCH]/1.144444) as nvarchar)
      ,[GPRS���м����ŵ�]='GPRS���м����ŵ�'+cast(floor([GPRS���м����ŵ�]/1.168722) as nvarchar)
      ,[GPRSÿ�������û�]='GPRSÿ�������û�'+cast(floor([GPRSÿ�������û�]/0.192506) as nvarchar)
      ,[EDGE���м����ŵ�]='EDGE���м����ŵ�'+cast(floor([EDGE���м����ŵ�]/0.504778) as nvarchar)
      ,[EDGEÿ�������û�]='EDGEÿ�������û�'+cast(floor([EDGEÿ�������û�]/0.223866) as nvarchar)
      ,[PDCH���ö�]='PDCH���ö�'+cast(floor([PDCH���ö�]/0.181192) as nvarchar)
 from [RedianHedian].[dbo].[psredian1000]
go
declare @sql varchar(8000)
select @sql = 'bcp tempdb..vw_bcpMasterSysobjects out c:\sysobjects.txt -c -t, -T -S' + @@servername
exec master..xp_cmdshell @sql

