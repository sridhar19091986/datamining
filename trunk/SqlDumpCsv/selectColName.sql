use tempdb
go
--select * from  vw_bcpMasterSysobjects
Declare @TabName varchar(100)
Declare @Cols varchar(1000)
Declare @tmpCol varchar(100)

Set @TabName ='vw_bcpMasterSysobjects'
SELECT @Cols = (SELECT COL_NAME(OBJECT_ID(@TabName),1) )
Set @tmpCol = ''

Declare @i int
Set @i=2

WHILE @i > 0
Begin

   SELECT @tmpCol = ISNULL((SELECT COL_NAME(OBJECT_ID(@TabName),@i)),'')

   if (@tmpCol<>'')
      begin
     set @Cols = @Cols +','+ @tmpCol
     set @tmpCol = ''
     set @i = @i + 1
      end
   else
  Set @i = 0 --ÍË³öÑ­»·
End

print @Cols