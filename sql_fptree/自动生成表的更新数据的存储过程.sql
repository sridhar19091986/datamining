/*

功能：返回某一表的所有字段、存储过程、函数的参数信息

设计：OK_008

时间：2006-05

*/

CREATE FUNCTION FN_GetObjColInfo

(@ObjName varchar(50))

RETURNS @Return_Table TABLE(

                     TName nvarchar(50),

                     TypeName nvarchar(50),

                     TypeLength nvarchar(50),

                     Colstat          Bit

                     )

AS

BEGIN

           INSERT @Return_Table

                     /*

                     主要是从系统表中提取表(对象)的各字段信息。

                     sysobjects: 在数据库内创建的每个对象（约束、默认值、日志、规则、存储过程等）在表中占一行

                     syscolumns:每个表和视图中的每列在表中占一行，存储过程中的每个参数在表中也占一行

                     systypes: 保存数据类型和用户定义数据类型

                     */

                     select b.name as 字段名,c.name as 字段类型,b.length/2 as 字段长度,b.colstat as 是否自动增长

                     from sysobjects a

                     inner join syscolumns b on a.id=b.id

                     inner join systypes c on c.xusertype=b.xtype

                     where a.name =@ObjName

                     order by B.ColID

           RETURN

END

GO

存储过程SP_CreateProcdure：

CREATE PROCEDURE SP_CreateProcdure

@TableName nvarchar(50)

AS

/*

功能： 自动生成表的更新数据的存储过程

     如：当建立表MyTable后，执行SP_CreateProcdure ，生成表MyTable的数据更

                   新的存储过程UP_MyTable

设计： OK_008

时间： 2006-05

备注：

           1、请在查询分析器上执行：EXEC SP_CreateProcdure TableName

           2、由于生成的字符串长度合计很多时候存在>4000以上，所有只使用Print输出，

                 再Copy即可。

           3、该方法能生成一般表的更新数据的存储过程，其中更新格式可以根据实际

                 情况修改。

设计方法：

           1、提取表的各个字段信息

           2、 ──┰─ 构造更新数据过程

                   ├─ 构造存储过程参数部分

                ├─ 构造新增数据部分

             ├─ 构造更新数据部分

                   ├─ 构造删除数据部分

           3、分段PRINT

           4、把输出来的结果复制到新建立存储过程界面中即可使用。

*/

DECLARE @strParameter nvarchar(3000)

DECLARE @strInsert nvarchar(3000)

DECLARE @strUpdate nvarchar(3000)

DECLARE @strDelete nvarchar(500)

DECLARE @strWhere nvarchar(100)

DECLARE @strNewID nvarchar(100)

DECLARE @SQL_CreateProc nvarchar(4000)

SET @SQL_CreateProc='CREATE PROCEDURE UP_'+@TableName +char(13)+'@INTUpdateID int,' +' /* -1 删除 0 修改 1新增 */'

SET @strParameter=''

SET @strInsert=''

SET @strUpdate=''

SET @strWhere=''

DECLARE @TName nvarchar(50),@TypeName nvarchar(50),@TypeLength nvarchar(50),@Colstat bit

DECLARE Obj_Cursor CURSOR FOR

SELECT * FROM FN_GetObjColInfo(@TableName)

OPEN Obj_Cursor

FETCH NEXT FROM Obj_Cursor INTO @TName,@TypeName,@TypeLength,@Colstat

WHILE @@FETCH_STATUS=0

BEGIN

           --构造存储过程参数部分

           SET @strParameter=@strParameter +CHAR(13)+'@'+ @TName + ' ' +@TypeName+

                                           (CASE

                                           WHEN @TypeName='nvarchar' THEN '('+@TypeLength+')'

                                           ELSE ''

                                           END)+','

           --构造新增数据部分

           IF @Colstat=0 SET @strInsert=@strInsert + '@'+ @TName +','

           --构造更新数据部分

           IF (@strWhere='')

              BEGIN

                     IF @Colstat=0 SET @strNewID='SET @'+@TName+'=(Select ISNULL(MAX('+@TName+'),0) From '+@TableName+')+1 --取新的ID'

                     SET @strWhere=' WHERE '+@TName+'='+'@'+@TName

              END

           ELSE

                     SET @strUpdate=@strUpdate+@TName+'='+'@'+@TName +','

           --构造删除数据部分

           FETCH NEXT FROM Obj_Cursor INTO @TName,@TypeName,@TypeLength,@Colstat

END

CLOSE Obj_Cursor

DEALLOCATE Obj_Cursor

SET @strParameter=LEFT(@strParameter,LEN(@strParameter)-1) --去掉最右边的逗号

SET @strUpdate=LEFT(@strUpdate,LEN(@strUpdate)-1)

SET @strInsert=LEFT(@strInsert,LEN(@strInsert)-1)

--存储过程名、参数

PRINT @SQL_CreateProc+@strParameter +CHAR(13)+'AS'

--修改

PRINT 'IF (@INTUpdateID=0)'

PRINT' BEGIN'+CHAR(13)

PRINT CHAR(9)+'UPDATE '+@TableName+' SET '+@strUpdate+CHAR(13)+CHAR(9)+@strWhere

PRINT ' END'

--增加

PRINT 'ELSE IF (@INTUpdateID=1)'

PRINT ' BEGIN'

PRINT CHAR(9)+@strNewID

PRINT CHAR(9)+'INSERT INTO '+@TableName+' ('+REPLACE(@strInsert,'@','') +') VALUES ( '+@strInsert +')'

PRINT ' END'

--删除

PRINT 'ELSE'

PRINT ' BEGIN'

PRINT CHAR(9)+'DELETE FROM '+@TableName +@strWhere

PRINT ' END'

PRINT 'GO'

GO