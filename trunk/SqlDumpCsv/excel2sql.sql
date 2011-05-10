

--[code=SQL][/code]SELECT * 
--FROM OpenDataSource( 'Microsoft.Jet.OLEDB.4.0',
--'Data Source="d:\临时文件\05.xls";User ID=Admin;Password=;Extended properties=Excel 5.0')...[Sheet1$]

--Provider=Microsoft.ACE.OLEDB.12.0;
--Data Source=c:\myFolder\myExcel2007file.xlsx;
--Extended Properties="Excel 12.0;HDR=YES";
--"HDR=Yes;"  

--select * from opendatasource('Microsoft.ACE.OLEDB.12.0',
--'Data Source="c:\PS总流量10000.xlsx";
--Extended Properties="Excel 12.0;HDR=YES";
--')...[EDGE下行速率透视$]

use RedianHedian
go
insert into gsmredian 
select *
from opendatasource('Microsoft.ACE.OLEDB.12.0',
'Data Source="D:\小区小时SMS2011-05-07-1\小区小时SMS2011-05-07-1\SZ01-10小区小时SMS2011-05-07.xlsx";
Extended Properties="Excel 12.0;HDR=YES";
')...[Sheet1$]
go