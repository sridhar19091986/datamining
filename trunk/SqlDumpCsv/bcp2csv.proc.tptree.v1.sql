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
            from [RedianHedian].[dbo].[gprsredian_ps]
            order by '+@sortField 
 exec(@sqlc)
 set @sqld= 'bcp RedianHedian..bcpRedianSysobjects out c:\'+@outFile+'.csv -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqld
--ͳ�ƹ�����
 declare @sqla varchar(8000)
 declare @sqlb varchar(8000)
 set @sqla='select 
       floor([GPRS��������]/33470.24)+0.001         as   [GPRS��������]
      ,floor([GPRS������]/36357.56)+0.002            as   [GPRS������]
      ,floor([GPRS��������]/30452.18)+0.003         as   [EDGE��������]
      ,floor([EDGE������]/33151.78)+0.004           as   [EDGE������]
      ,floor([PS��������]/27243.38)+0.005           as   [PS��������]
      ,floor([PS������]/24628.86)+0.006             as   [PS������]
      ,floor([GPRS��������]/1.054742)+0.007         as   [GPRS��������]
      ,floor([EDGE��������]/4.454972)+0.008         as   [EDGE��������]
      ,floor([EDGEÿ�ŵ���������]/1.179246)+0.009   as   [EDGEÿ�ŵ���������]
      ,floor([GPRSÿ�ŵ���������]/0.09081)+0.010   as   [GPRSÿ�ŵ���������]
      ,floor([GPRSÿPFC��������]/3.286316)+0.011    as   [GPRSÿPFC��������]
      ,floor([EDGEÿPFC��������]/119.23203)+0.012    as   [EDGEÿPFC��������]
      ,floor([����TBF�����ɹ���]/0.769016)+0.013    as   [����TBF�����ɹ���]
      ,floor([����TBF�����ɹ���]/0.363938)+0.014     as   [����TBF�����ɹ���]
      ,floor([����TBF�쳣�ͷ�]/6.12)+0.015          as   [����TBF�쳣�ͷ�]
      ,floor([PS����ָ�ɳɹ���]/0.626598)+0.016     as   [PS����ָ�ɳɹ���]
      ,floor([����TBF������]/0.065088)+0.017        as   [����TBF������]
      ,floor([ƽ������PDCH]/1.439964)+0.018           as   [ƽ������PDCH]
      ,floor([ƽ��ʹ��PDCH]/1.397444)+0.019         as   [ƽ��ʹ��PDCH]
      ,floor([GPRS���м����ŵ�]/1.137056)+0.020     as   [GPRS���м����ŵ�]
      ,floor([GPRSÿ�������û�]/0.23174)+0.021     as   [GPRSÿ�������û�]
      ,floor([EDGE���м����ŵ�]/0.639944)+0.022     as   [EDGE���м����ŵ�]
      ,floor([EDGEÿ�������û�]/0.203624)+0.023     as   [EDGEÿ�������û�]
      ,floor([PDCH���ö�]/0.18587)+0.024           as  [PDCH���ö�]
      ,floor([PPSIMMASS]/1331.04)+0.025           as  [PPSIMMASS]
      ,floor([REJPSIMMASS]/4.44)+0.026           as  [REJPSIMMASS]
      ,floor([PSCHREQ]/3956.58)+0.027           as  [PSCHREQ]
      ,floor([PREJTFI]/1.36)+0.028           as  [PREJTFI]
      ,floor([PREJOTH]/1.36)+0.029           as  [PREJOTH]
      into bcpMasterSysobjects
      from bcpRedianSysobjects'
 exec(@sqla)
 set @sqlb= 'bcp RedianHedian..bcpMasterSysobjects out c:\'+@outFile+'.txt -c -t, -T -S' + @@servername
 exec master..xp_cmdshell @sqlb
End

