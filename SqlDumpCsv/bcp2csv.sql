use tempdb
go
drop view vw_bcpMasterSysobjects
go
create view vw_bcpMasterSysobjects
as
 select top 100 percent
 name = 'name=' + name,
 crdate = 'crdate=' + convert(varchar(8), crdate, 112),
 crtime = 'crtime=' + convert(varchar(8), crdate, 108)
 from master..sysobjects
order by crdate desc
go
declare @sql varchar(8000)
select @sql = 'bcp tempdb..vw_bcpMasterSysobjects out c:\sysobjects.txt -c -t, -T -S' + @@servername
exec master..xp_cmdshell @sql