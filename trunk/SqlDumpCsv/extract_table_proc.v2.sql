
use RedianHedian
go
if exists(select * from sysobjects where name='ExtractTable' and type='p')
drop procedure ExtractTable
go
CREATE PROCEDURE ExtractTable
(@inputFile varchar(50),@sortField VARCHAR(50),@sortTopCount VARCHAR(50),@outFile VARCHAR(50))
as 
BEGIN
 if exists(select * from sysobjects where id=object_id('RedianHedian..'+@outFile))
 begin 
   declare @sql varchar(8000)
   set @sql = 'drop table RedianHedian..'+@outFile
   exec(@sql)
 end
 declare @sqlc varchar(8000)
 declare @sqld varchar(8000)
 set @sqlc='select top '+@sortTopCount+' *
            into '+@outFile+'
            from '+@inputFile+'
            order by '+@sortField 
 exec(@sqlc)
 set @sqld='c:\'+@outFile+'.csv'
 EXEC proc_generate_excel_with_columns RedianHedian, @outFile,@sqld
 End