use RedianHedian
go
if exists(select * from sysobjects where name='stat_hot_gprs' and type='p')
drop procedure stat_hot_gprs
go
CREATE PROCEDURE stat_hot_gprs(@sortField VARCHAR(50),@sortTopCount VARCHAR(50),@outFile VARCHAR(50))
as 
BEGIN
 --��ʱ��
if exists(select * from sysobjects where id=object_id('RedianHedian..bcpRedianSysobjects'))
drop table bcpRedianSysobjects
if exists(select * from sysobjects where id=object_id('RedianHedian..bcpMasterSysobjects'))
drop table bcpMasterSysobjects
 --���ԭʼ��
 declare @sqlc varchar(8000)
 declare @sqld varchar(8000)
 set @sqlc='select top '+@sortTopCount+' *
            into bcpRedianSysobjects
            from [RedianHedian].[dbo].[gprsredian]
            order by '+@sortField 
 exec(@sqlc)
 set @sqld= 'bcp RedianHedian..bcpRedianSysobjects out c:\'+@outFile+'.csv -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqld
--ͳ�ƹ�����
 declare @sqla varchar(8000)
 declare @sqlb varchar(8000)
 set @sqla='select 
       floor([GPRS��������]/33212.22)+0.001         as   [GPRS��������]
      ,floor([GPRS������]/36036.8)+0.002            as   [GPRS������]
      ,floor([GPRS��������]/44319.86)+0.003         as   [EDGE��������]
      ,floor([EDGE������]/51550.08)+0.004           as   [EDGE������]
      ,floor([PS��������]/20164.68)+0.005           as   [PS��������]
      ,floor([PS������]/21407.48)+0.006             as   [PS������]
      ,floor([GPRS��������]/0.617902)+0.007         as   [GPRS��������]
      ,floor([EDGE��������]/3.153366)+0.008         as   [EDGE��������]
      ,floor([EDGEÿ�ŵ���������]/1.105154)+0.009   as   [EDGEÿ�ŵ���������]
      ,floor([GPRSÿ�ŵ���������]/0.032104)+0.010   as   [GPRSÿ�ŵ���������]
      ,floor([GPRSÿPFC��������]/0.729792)+0.011    as   [GPRSÿPFC��������]
      ,floor([EDGEÿPFC��������]/0.697372)+0.012    as   [EDGEÿPFC��������]
      ,floor([����TBF�����ɹ���]/1.280918)+0.013    as   [����TBF�����ɹ���]
      ,floor([����TBF�����ɹ���]/1.05192)+0.014     as   [����TBF�����ɹ���]
      ,floor([����TBF�쳣�ͷ�]/5.98)+0.015          as   [����TBF�쳣�ͷ�]
      ,floor([PS����ָ�ɳɹ���]/1.255536)+0.016     as   [PS����ָ�ɳɹ���]
      ,floor([����TBF������]/0.006236)+0.017        as   [����TBF������]
      ,floor([ƽ������PDCH]/1.0365)+0.018           as   [ƽ������PDCH]
      ,floor([ƽ��ʹ��PDCH]/1.144444)+0.019         as   [ƽ��ʹ��PDCH]
      ,floor([GPRS���м����ŵ�]/1.168722)+0.020     as   [GPRS���м����ŵ�]
      ,floor([GPRSÿ�������û�]/0.192506)+0.021     as   [GPRSÿ�������û�]
      ,floor([EDGE���м����ŵ�]/0.504778)+0.022     as   [EDGE���м����ŵ�]
      ,floor([EDGEÿ�������û�]/0.223866)+0.023     as   [EDGEÿ�������û�]
      ,floor([PDCH���ö�]/0.181192)+0.024           as  [PDCH���ö�]
      into bcpMasterSysobjects
      from bcpRedianSysobjects'
 exec(@sqla)
 set @sqlb= 'bcp RedianHedian..bcpMasterSysobjects out c:\'+@outFile+'.txt -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqlb
End

