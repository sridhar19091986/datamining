use tempdb
go
drop  view vcols
go
create view vcols 
as 
select name from syscolumns where id=
(select max(id) from sysobjects where xtype='u' and name='wAMR')
go
declare @colname varchar(1000)
declare @Cols varchar(1000)
declare tbl_cursor CURSOR for select name from tempdb..vcols order by name --����
open tbl_cursor
fetch next from tbl_cursor into @colname
while @@FETCH_STATUS = 0
begin
   set @Cols = @Cols +','+ @colname   --��̬ƴ�ӣ�
   FETCH NEXT FROM col_cursor INTO @colname
end
set @Cols = 'select '+@Cols +'from '--��̬ƴ�ӣ�
print @Cols
close tbl_cursor
deallocate tbl_cursor
go
