--check the DB exists procedure or not, if yes Delete it
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[proc_generate_excel_with_columns]') and OBJECTPROPERTY(id, N'IsProcedure')=1)  
begin
 drop procedure [dbo].[proc_generate_excel_with_columns]
end


/*
��ͷ���� "[]", ������Ҫ,���޸Ĳ�ѯ���,��������������û�пո�,�����EXCELʱ,show warning,����Ӱ��

EXEC proc_generate_excel_with_columns 'DBName', 'TableName','D:\OutPutExcel.xls'

EXEC proc_generate_excel_with_columns 'DBName', 'User','D:\OutPutExcel.xls' --User is a Key Work but Not need add '[]'

--�����ݺ�������,�뵼�������İ�EXCEL,��TXT�ĵ���, ��������....
EXEC proc_generate_excel_with_columns 'DBName', 'TableName','D:\OutPutTTxt.txt'

--��TXT ��Ӣ�İ�EXCEL,Ҳ����, bcp  �����ı����Ǵ��ı��ĵ�


--����ʱ,Ҫ���޸ĺ����ݿ����,��½��������,
--���޸Ĵ���ı���
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
 @columns=coalesce(@columns+',','')+column_name+' as ['+column_name +']' --��ͷ����"[]"
 --@columns=coalesce(@columns+',','')+'['+column_name+'] as ['+column_name +']' --��ͷ��"[]"
from
 information_schema.columns
where
 table_name=''+@table_name+''
select @columns=''''''+replace(replace(@columns,' as ',''''' as '),',',',''''')

--Create a dummy file to have actual data
select @data_file=substring(@file_name,1,len(@file_name)-charindex('\',reverse(@file_name)))+'\data_file.xls'

--Generate column names in the passed EXCEL file
set @sql='exec [master].[dbo].[xp_cmdshell] ''bcp "select * from (select '+@columns+') as t" queryout "'+@file_name+'" -c  -t, -T -S"localhost" -U"sa" -P"adminadmin"'''--���޸ĵ�½����
--set @sql='exec master..xp_cmdshell ''bcp "select * from (select '+@columns+') as t" queryout "'+@file_name+'" -c -S"'+@DBServer+'" -U"'+@DBAdminName+'" -P"'+@DBPwd+'"'''--����ʱ,��Ҫ�����������ݿ���Ϣ
exec(@sql)

--Generate data in the dummy file
set @sql='exec [master].[dbo].[xp_cmdshell] ''bcp "select * from ['+@db_name+'].dbo.['+@table_name+']" queryout "'+@data_file+'" -c  -t, -T -S"localhost" -U"sa" -P"adminadmin"''' --���޸ĵ�½����
--set @sql='exec master..xp_cmdshell ''bcp "select * from ['+@db_name+'].dbo.['+@table_name+']" queryout "'+@data_file+'" -c -S"'+@DBServer+'" -U"'+@DBAdminName+'" -P"'+@DBPwd+'"''' --����ʱ,��Ҫ�����������ݿ���Ϣ
exec(@sql)

--Copy dummy file to passed EXCEL file
set @sql= 'exec [master].[dbo].[xp_cmdshell] ''type "'+@data_file+'" >> "'+@file_name+'"'''
exec(@sql)

--Delete dummy file
set @sql= 'exec [master].[dbo].[xp_cmdshell] ''del '+@data_file+''''
exec(@sql)
