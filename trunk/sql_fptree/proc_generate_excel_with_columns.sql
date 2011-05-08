--check the DB exists procedure or not, if yes Delete it
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[proc_generate_excel_with_columns]') and OBJECTPROPERTY(id, N'IsProcedure')=1)  
begin
 drop procedure [dbo].[proc_generate_excel_with_columns]
end


/*
表头带有 "[]", 若不需要,可修改查询语句,但表内列名必须没有空格,否则打开EXCEL时,show warning,但不影响

EXEC proc_generate_excel_with_columns 'DBName', 'TableName','D:\OutPutExcel.xls'

EXEC proc_generate_excel_with_columns 'DBName', 'User','D:\OutPutExcel.xls' --User is a Key Work but Not need add '[]'

--若数据含有中文,请导出到中文版EXCEL,或TXT文档等, 否则乱码....
EXEC proc_generate_excel_with_columns 'DBName', 'TableName','D:\OutPutTTxt.txt'

--用TXT 打开英文版EXCEL,也可以, bcp  导出的本质是纯文本文档


--创建时,要先修改好数据库服务,登陆名和密码,
--或修改传入的变量
EXEC proc_generate_excel_with_columns '.\SQLExpress','sa','password','DBName', 'TableName','D:\OutPutExcel.xls'

*/
go
--defind the Values
create procedure proc_generate_excel_with_columns
(
--    @DBServer varchar(100),
--    @DBAdminName varchar(100),
--    @DBPwd varchar(100),

 @db_name varchar(100),
 @table_name varchar(100), 
 @file_name varchar(100)
)
as

--Generate column names as a recordset
declare @columns varchar(8000), @sql varchar(8000), @data_file varchar(100)
select
 @columns=coalesce(@columns+',','')+column_name+' as ['+column_name +']' --表头不带"[]"
 --@columns=coalesce(@columns+',','')+'['+column_name+'] as ['+column_name +']' --表头带"[]"
from
 information_schema.columns
where
 table_name=''+@table_name+''
select @columns=''''''+replace(replace(@columns,' as ',''''' as '),',',',''''')

--Create a dummy file to have actual data
select @data_file=substring(@file_name,1,len(@file_name)-charindex('\',reverse(@file_name)))+'\data_file.xls'

--Generate column names in the passed EXCEL file
set @sql='exec [master].[dbo].[xp_cmdshell] ''bcp "select * from (select '+@columns+') as t" queryout "'+@file_name+'" -c  -t, -T -S"localhost" -U"sa" -P"adminadmin"'''--请修改登陆密码
--set @sql='exec master..xp_cmdshell ''bcp "select * from (select '+@columns+') as t" queryout "'+@file_name+'" -c -S"'+@DBServer+'" -U"'+@DBAdminName+'" -P"'+@DBPwd+'"'''--调用时,需要传递连接数据库信息
exec(@sql)

--Generate data in the dummy file
set @sql='exec [master].[dbo].[xp_cmdshell] ''bcp "select * from ['+@db_name+'].dbo.['+@table_name+']" queryout "'+@data_file+'" -c  -t, -T -S"localhost" -U"sa" -P"adminadmin"''' --请修改登陆密码
--set @sql='exec master..xp_cmdshell ''bcp "select * from ['+@db_name+'].dbo.['+@table_name+']" queryout "'+@data_file+'" -c -S"'+@DBServer+'" -U"'+@DBAdminName+'" -P"'+@DBPwd+'"''' --调用时,需要传递连接数据库信息
exec(@sql)

--Copy dummy file to passed EXCEL file
set @sql= 'exec [master].[dbo].[xp_cmdshell] ''type "'+@data_file+'" >> "'+@file_name+'"'''
exec(@sql)

--Delete dummy file
set @sql= 'exec [master].[dbo].[xp_cmdshell] ''del '+@data_file+''''
exec(@sql)
