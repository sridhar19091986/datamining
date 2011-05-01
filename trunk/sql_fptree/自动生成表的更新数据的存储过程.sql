/*

���ܣ�����ĳһ��������ֶΡ��洢���̡������Ĳ�����Ϣ

��ƣ�OK_008

ʱ�䣺2006-05

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

                     ��Ҫ�Ǵ�ϵͳ������ȡ��(����)�ĸ��ֶ���Ϣ��

                     sysobjects: �����ݿ��ڴ�����ÿ������Լ����Ĭ��ֵ����־�����򡢴洢���̵ȣ��ڱ���ռһ��

                     syscolumns:ÿ�������ͼ�е�ÿ���ڱ���ռһ�У��洢�����е�ÿ�������ڱ���Ҳռһ��

                     systypes: �����������ͺ��û�������������

                     */

                     select b.name as �ֶ���,c.name as �ֶ�����,b.length/2 as �ֶγ���,b.colstat as �Ƿ��Զ�����

                     from sysobjects a

                     inner join syscolumns b on a.id=b.id

                     inner join systypes c on c.xusertype=b.xtype

                     where a.name =@ObjName

                     order by B.ColID

           RETURN

END

GO

�洢����SP_CreateProcdure��

CREATE PROCEDURE SP_CreateProcdure

@TableName nvarchar(50)

AS

/*

���ܣ� �Զ����ɱ�ĸ������ݵĴ洢����

     �磺��������MyTable��ִ��SP_CreateProcdure �����ɱ�MyTable�����ݸ�

                   �µĴ洢����UP_MyTable

��ƣ� OK_008

ʱ�䣺 2006-05

��ע��

           1�����ڲ�ѯ��������ִ�У�EXEC SP_CreateProcdure TableName

           2���������ɵ��ַ������Ⱥϼƺܶ�ʱ�����>4000���ϣ�����ֻʹ��Print�����

                 ��Copy���ɡ�

           3���÷���������һ���ĸ������ݵĴ洢���̣����и��¸�ʽ���Ը���ʵ��

                 ����޸ġ�

��Ʒ�����

           1����ȡ��ĸ����ֶ���Ϣ

           2�� �����ԩ� ����������ݹ���

                   ���� ����洢���̲�������

                ���� �����������ݲ���

             ���� ����������ݲ���

                   ���� ����ɾ�����ݲ���

           3���ֶ�PRINT

           4����������Ľ�����Ƶ��½����洢���̽����м���ʹ�á�

*/

DECLARE @strParameter nvarchar(3000)

DECLARE @strInsert nvarchar(3000)

DECLARE @strUpdate nvarchar(3000)

DECLARE @strDelete nvarchar(500)

DECLARE @strWhere nvarchar(100)

DECLARE @strNewID nvarchar(100)

DECLARE @SQL_CreateProc nvarchar(4000)

SET @SQL_CreateProc='CREATE PROCEDURE UP_'+@TableName +char(13)+'@INTUpdateID int,' +' /* -1 ɾ�� 0 �޸� 1���� */'

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

           --����洢���̲�������

           SET @strParameter=@strParameter +CHAR(13)+'@'+ @TName + ' ' +@TypeName+

                                           (CASE

                                           WHEN @TypeName='nvarchar' THEN '('+@TypeLength+')'

                                           ELSE ''

                                           END)+','

           --�����������ݲ���

           IF @Colstat=0 SET @strInsert=@strInsert + '@'+ @TName +','

           --����������ݲ���

           IF (@strWhere='')

              BEGIN

                     IF @Colstat=0 SET @strNewID='SET @'+@TName+'=(Select ISNULL(MAX('+@TName+'),0) From '+@TableName+')+1 --ȡ�µ�ID'

                     SET @strWhere=' WHERE '+@TName+'='+'@'+@TName

              END

           ELSE

                     SET @strUpdate=@strUpdate+@TName+'='+'@'+@TName +','

           --����ɾ�����ݲ���

           FETCH NEXT FROM Obj_Cursor INTO @TName,@TypeName,@TypeLength,@Colstat

END

CLOSE Obj_Cursor

DEALLOCATE Obj_Cursor

SET @strParameter=LEFT(@strParameter,LEN(@strParameter)-1) --ȥ�����ұߵĶ���

SET @strUpdate=LEFT(@strUpdate,LEN(@strUpdate)-1)

SET @strInsert=LEFT(@strInsert,LEN(@strInsert)-1)

--�洢������������

PRINT @SQL_CreateProc+@strParameter +CHAR(13)+'AS'

--�޸�

PRINT 'IF (@INTUpdateID=0)'

PRINT' BEGIN'+CHAR(13)

PRINT CHAR(9)+'UPDATE '+@TableName+' SET '+@strUpdate+CHAR(13)+CHAR(9)+@strWhere

PRINT ' END'

--����

PRINT 'ELSE IF (@INTUpdateID=1)'

PRINT ' BEGIN'

PRINT CHAR(9)+@strNewID

PRINT CHAR(9)+'INSERT INTO '+@TableName+' ('+REPLACE(@strInsert,'@','') +') VALUES ( '+@strInsert +')'

PRINT ' END'

--ɾ��

PRINT 'ELSE'

PRINT ' BEGIN'

PRINT CHAR(9)+'DELETE FROM '+@TableName +@strWhere

PRINT ' END'

PRINT 'GO'

GO