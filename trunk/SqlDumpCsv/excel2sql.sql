

--[code=SQL][/code]SELECT * 
--FROM OpenDataSource( 'Microsoft.Jet.OLEDB.4.0',
--'Data Source="d:\��ʱ�ļ�\05.xls";User ID=Admin;Password=;Extended properties=Excel 5.0')...[Sheet1$]

--Provider=Microsoft.ACE.OLEDB.12.0;
--Data Source=c:\myFolder\myExcel2007file.xlsx;
--Extended Properties="Excel 12.0;HDR=YES";
--"HDR=Yes;"  

--select * from opendatasource('Microsoft.ACE.OLEDB.12.0',
--'Data Source="c:\PS������10000.xlsx";
--Extended Properties="Excel 12.0;HDR=YES";
--')...[EDGE��������͸��$]

use RedianHedian
go
insert into gsmredian 
select *
from opendatasource('Microsoft.ACE.OLEDB.12.0',
'Data Source="D:\С��СʱSMS2011-05-07-1\С��СʱSMS2011-05-07-1\SZ01-10С��СʱSMS2011-05-07.xlsx";
Extended Properties="Excel 12.0;HDR=YES";
')...[Sheet1$]
go