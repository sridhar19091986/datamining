四个存储过程

pr__SYS_MakeInsertRecordProc

pr__SYS_MakeUpdateRecordProc

pr__SYS_MakeSelectRecordProc

pr__SYS_MakeDeleteRecordProc

执行方式：在查询分析器中执行

pr__SYS_MakeInsertRecordProc '表名'          --得到插入语句

pr__SYS_MakeInsertRecordProc '表名',1   　    --得到插入语句，并创建Insert存储过程
相关阅读：
(1) Automate the Generation of Stored Procedures for Your Database
(2) Data Access Application Block

完整脚本如下:

----------------------------- Proc_Delete_begin -------------------------------------
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO


Create       PROC pr__SYS_MakeDeleteRecordProc
@sTableName varchar(128),
@bExecute bit = 0
AS

IF dbo.fnTableHasPrimaryKey(@sTableName) = 0
BEGIN
RAISERROR ('Procedure cannot be created on a table with no primary key.', 10, 1)
RETURN
END

DECLARE @sProcText varchar(8000),
@sKeyFields varchar(2000),
@sWhereClause varchar(2000),
@sColumnName varchar(128),
@nColumnID smallint,
@bPrimaryKeyColumn bit,
@nAlternateType int,
@nColumnLength int,
@nColumnPrecision int,
@nColumnScale int,
@IsNullable bit,
@IsIdentity int,
@sTypeName varchar(128),
@sDefaultValue varchar(4000),
@sCRLF char(2),
@sTAB char(1)

SET @sTAB = char(9)
SET   @sCRLF = char(13) + char(10)

SET   @sProcText = ''
SET   @sKeyFields = ''
SET @sWhereClause = ''

SET   @sProcText = @sProcText + 'IF EXISTS(Select * FROM sysobjects Where name = ''prApp_' + @sTableName + '_Delete'')' + @sCRLF
SET   @sProcText = @sProcText + @sTAB + 'Drop PROC prApp_' + @sTableName + '_Delete' + @sCRLF
IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF

SET   @sProcText = @sProcText + @sCRLF

PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)

SET   @sProcText = ''
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + '-- Delete a single record from ' + @sTableName + @sCRLF
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + 'Create PROC prApp_' + @sTableName + '_Delete' + @sCRLF

DECLARE crKeyFields cursor for
Select *
FROM dbo.fnTableColumnInfo(@sTableName)
orDER BY 2

OPEN crKeyFields

FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
@nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
@IsIdentity, @sTypeName, @sDefaultValue
    
WHILE (@@FETCH_STATUS = 0)
BEGIN

IF (@bPrimaryKeyColumn = 1)
   BEGIN
   IF (@sKeyFields <> '')
    SET @sKeyFields = @sKeyFields + ',' + @sCRLF

   SET @sKeyFields = @sKeyFields + @sTAB + '@' + @sColumnName + ' ' + @sTypeName

   IF (@nAlternateType = 2) --decimal, numeric
    SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnPrecision AS varchar(3)) + ', '
      + CAST(@nColumnScale AS varchar(3)) + ')'

   ELSE IF (@nAlternateType = 1) --character and binary
    SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnLength AS varchar(4)) +   ')'

   IF (@sWhereClause = '')
    SET @sWhereClause = @sWhereClause + 'Where '
   ELSE
    SET @sWhereClause = @sWhereClause + ' AND '

   SET @sWhereClause = @sWhereClause + @sTAB + @sColumnName   + ' = @' + @sColumnName + @sCRLF
   END

FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
   @nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
   @IsIdentity, @sTypeName, @sDefaultValue
END

CLOSE crKeyFields
DEALLOCATE crKeyFields

SET   @sProcText = @sProcText + @sKeyFields + @sCRLF
SET   @sProcText = @sProcText + 'AS' + @sCRLF
SET   @sProcText = @sProcText + @sCRLF
SET   @sProcText = @sProcText + 'Delete ' + @sTableName + @sCRLF
SET   @sProcText = @sProcText + @sWhereClause
SET   @sProcText = @sProcText + @sCRLF
IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF


PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

----------------------------- Proc_Delete_end    -------------------------------------


----------------------------- Proc_Insert_begin -------------------------------------
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO


Create        PROC pr__SYS_MakeInsertRecordProc
@sTableName varchar(128),
@bExecute bit = 0
AS

IF dbo.fnTableHasPrimaryKey(@sTableName) = 0
BEGIN
RAISERROR ('Procedure cannot be created on a table with no primary key.', 10, 1)
RETURN
END

DECLARE @sProcText varchar(8000),
@sKeyFields varchar(2000),
@sAllFields varchar(2000),
@sAllParams varchar(2000),
@sWhereClause varchar(2000),
@sColumnName varchar(128),
@nColumnID smallint,
@bPrimaryKeyColumn bit,
@nAlternateType int,
@nColumnLength int,
@nColumnPrecision int,
@nColumnScale int,
@IsNullable bit,
@IsIdentity int,
@HasIdentity int,
@sTypeName varchar(128),
@sDefaultValue varchar(4000),
@sCRLF char(2),
@sTAB char(1)

SET   @HasIdentity = 0
SET @sTAB = char(9)
SET   @sCRLF = char(13) + char(10)
SET   @sProcText = ''
SET   @sKeyFields = ''
SET @sAllFields = ''
SET @sWhereClause = ''
SET @sAllParams   = ''

SET   @sProcText = @sProcText + 'IF EXISTS(Select * FROM sysobjects Where name = ''prApp_' + @sTableName + '_Insert'')' + @sCRLF
SET   @sProcText = @sProcText + @sTAB + 'Drop PROC prApp_' + @sTableName + '_Insert' + @sCRLF
IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF

SET   @sProcText = @sProcText + @sCRLF

PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)

SET   @sProcText = ''
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + '-- Insert a single record into ' + @sTableName + @sCRLF
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + 'Create PROC prApp_' + @sTableName + '_Insert' + @sCRLF

DECLARE crKeyFields cursor for
Select *
FROM dbo.fnTableColumnInfo(@sTableName)
orDER BY 2

OPEN crKeyFields


FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
@nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
@IsIdentity, @sTypeName, @sDefaultValue
    
WHILE (@@FETCH_STATUS = 0)
BEGIN
IF (@IsIdentity = 0)
   BEGIN
   IF (@sKeyFields <> '')
    SET @sKeyFields = @sKeyFields + ',' + @sCRLF

   SET @sKeyFields = @sKeyFields + @sTAB + '@' + @sColumnName + ' ' + @sTypeName

   IF (@sAllFields <> '')
    BEGIN
    SET @sAllParams = @sAllParams + ', '
    SET @sAllFields = @sAllFields + ', '
    END

   IF (@sTypeName = 'timestamp')
    SET @sAllParams = @sAllParams + 'NULL'
   ELSE IF (@sDefaultValue IS NOT NULL)
    SET @sAllParams = @sAllParams + 'COALESCE(@' + @sColumnName + ', ' + @sDefaultValue + ')'
   ELSE
    SET @sAllParams = @sAllParams + '@' + @sColumnName

   SET @sAllFields = @sAllFields + @sColumnName

   END
ELSE
   BEGIN
   SET @HasIdentity = 1
   END

IF (@nAlternateType = 2) --decimal, numeric
   SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnPrecision AS varchar(3)) + ', '
     + CAST(@nColumnScale AS varchar(3)) + ')'

ELSE IF (@nAlternateType = 1) --character and binary
   SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnLength AS varchar(4)) +   ')'

IF (@IsIdentity = 0)
   BEGIN
   IF (@sDefaultValue IS NOT NULL) or (@IsNullable = 1) or (@sTypeName = 'timestamp')
    SET @sKeyFields = @sKeyFields + ' = NULL'
   END

FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
   @nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
   @IsIdentity, @sTypeName, @sDefaultValue
END

CLOSE crKeyFields
DEALLOCATE crKeyFields

SET   @sProcText = @sProcText + @sKeyFields + @sCRLF
SET   @sProcText = @sProcText + 'AS' + @sCRLF
SET   @sProcText = @sProcText + @sCRLF
SET   @sProcText = @sProcText + 'Insert ' + @sTableName + '(' + @sAllFields + ')' + @sCRLF
SET   @sProcText = @sProcText + 'VALUES (' + @sAllParams + ')' + @sCRLF
SET   @sProcText = @sProcText + @sCRLF

IF (@HasIdentity = 1)
BEGIN
SET   @sProcText = @sProcText + 'RETURN SCOPE_IDENTITY()' + @sCRLF
SET   @sProcText = @sProcText + @sCRLF
END

IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF


PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

----------------------------- Proc_Insert_end    -------------------------------------


----------------------------- Proc_Select_begin -------------------------------------
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO


Create      PROC pr__SYS_MakeSelectRecordProc
@sTableName varchar(128),
@bExecute bit = 0
AS

IF dbo.fnTableHasPrimaryKey(@sTableName) = 0
BEGIN
RAISERROR ('Procedure cannot be created on a table with no primary key.', 10, 1)
RETURN
END

DECLARE @sProcText varchar(8000),
@sKeyFields varchar(2000),
@sSelectClause varchar(2000),
@sWhereClause varchar(2000),
@sColumnName varchar(128),
@nColumnID smallint,
@bPrimaryKeyColumn bit,
@nAlternateType int,
@nColumnLength int,
@nColumnPrecision int,
@nColumnScale int,
@IsNullable bit,
@IsIdentity int,
@sTypeName varchar(128),
@sDefaultValue varchar(4000),
@sCRLF char(2),
@sTAB char(1)

SET @sTAB = char(9)
SET   @sCRLF = char(13) + char(10)

SET   @sProcText = ''
SET   @sKeyFields = ''
SET @sSelectClause = ''
SET @sWhereClause = ''

SET   @sProcText = @sProcText + 'IF EXISTS(Select * FROM sysobjects Where name = ''prApp_' + @sTableName + '_Select'')' + @sCRLF
SET   @sProcText = @sProcText + @sTAB + 'Drop PROC prApp_' + @sTableName + '_Select' + @sCRLF
IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF

SET   @sProcText = @sProcText + @sCRLF

PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)

SET   @sProcText = ''
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + '-- Select a single record from ' + @sTableName + @sCRLF
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + 'Create PROC prApp_' + @sTableName + '_Select' + @sCRLF

DECLARE crKeyFields cursor for
Select *
FROM dbo.fnTableColumnInfo(@sTableName)
orDER BY 2

OPEN crKeyFields

FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
@nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
@IsIdentity, @sTypeName, @sDefaultValue
    
WHILE (@@FETCH_STATUS = 0)
BEGIN
IF (@bPrimaryKeyColumn = 1)
   BEGIN
   IF (@sKeyFields <> '')
    SET @sKeyFields = @sKeyFields + ',' + @sCRLF

   SET @sKeyFields = @sKeyFields + @sTAB + '@' + @sColumnName + ' ' + @sTypeName

   IF (@nAlternateType = 2) --decimal, numeric
    SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnPrecision AS varchar(3)) + ', '
      + CAST(@nColumnScale AS varchar(3)) + ')'

   ELSE IF (@nAlternateType = 1) --character and binary
    SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnLength AS varchar(4)) +   ')'

   IF (@sWhereClause = '')
    SET @sWhereClause = @sWhereClause + 'Where '
   ELSE
    SET @sWhereClause = @sWhereClause + ' AND '

   SET @sWhereClause = @sWhereClause + @sTAB + @sColumnName   + ' = @' + @sColumnName + @sCRLF
   END

IF (@sSelectClause = '')
   SET @sSelectClause = @sSelectClause + 'Select'
ELSE
   SET @sSelectClause = @sSelectClause + ',' + @sCRLF

SET @sSelectClause = @sSelectClause + @sTAB + @sColumnName

FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
   @nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
   @IsIdentity, @sTypeName, @sDefaultValue
END

CLOSE crKeyFields
DEALLOCATE crKeyFields

SET   @sSelectClause = @sSelectClause + @sCRLF

SET   @sProcText = @sProcText + @sKeyFields + @sCRLF
SET   @sProcText = @sProcText + 'AS' + @sCRLF
SET   @sProcText = @sProcText + @sCRLF
SET   @sProcText = @sProcText + @sSelectClause
SET   @sProcText = @sProcText + 'FROM ' + @sTableName + @sCRLF
SET   @sProcText = @sProcText + @sWhereClause
SET   @sProcText = @sProcText + @sCRLF
IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF


PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

----------------------------- Proc_Select_end    -------------------------------------


----------------------------- Proc_Update_begin -------------------------------------
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO


Create          PROC pr__SYS_MakeUpdateRecordProc
@sTableName varchar(128),
@bExecute bit = 0
AS

IF dbo.fnTableHasPrimaryKey(@sTableName) = 0
BEGIN
RAISERROR ('Procedure cannot be created on a table with no primary key.', 10, 1)
RETURN
END

DECLARE @sProcText varchar(8000),
@sKeyFields varchar(2000),
@sSetClause varchar(2000),
@sWhereClause varchar(2000),
@sColumnName varchar(128),
@nColumnID smallint,
@bPrimaryKeyColumn bit,
@nAlternateType int,
@nColumnLength int,
@nColumnPrecision int,
@nColumnScale int,
@IsNullable bit,
@IsIdentity int,
@sTypeName varchar(128),
@sDefaultValue varchar(4000),
@sCRLF char(2),
@sTAB char(1)

SET @sTAB = char(9)
SET   @sCRLF = char(13) + char(10)

SET   @sProcText = ''
SET   @sKeyFields = ''
SET @sSetClause = ''
SET @sWhereClause = ''

SET   @sProcText = @sProcText + 'IF EXISTS(Select * FROM sysobjects Where name = ''prApp_' + @sTableName + '_Update'')' + @sCRLF
SET   @sProcText = @sProcText + @sTAB + 'Drop PROC prApp_' + @sTableName + '_Update' + @sCRLF
IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF

SET   @sProcText = @sProcText + @sCRLF

PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)

SET   @sProcText = ''
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + '-- Update a single record in ' + @sTableName + @sCRLF
SET   @sProcText = @sProcText + '----------------------------------------------------------------------------' + @sCRLF
SET   @sProcText = @sProcText + 'Create PROC prApp_' + @sTableName + '_Update' + @sCRLF

DECLARE crKeyFields cursor for
Select *
FROM dbo.fnTableColumnInfo(@sTableName)
orDER BY 2

OPEN crKeyFields


FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
@nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
@IsIdentity, @sTypeName, @sDefaultValue
    
WHILE (@@FETCH_STATUS = 0)
BEGIN
IF (@sKeyFields <> '')
   SET @sKeyFields = @sKeyFields + ',' + @sCRLF

SET @sKeyFields = @sKeyFields + @sTAB + '@' + @sColumnName + ' ' + @sTypeName

IF (@nAlternateType = 2) --decimal, numeric
   SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnPrecision AS varchar(3)) + ', '
     + CAST(@nColumnScale AS varchar(3)) + ')'

ELSE IF (@nAlternateType = 1) --character and binary
   SET @sKeyFields =   @sKeyFields + '(' + CAST(@nColumnLength AS varchar(4)) +   ')'

IF (@bPrimaryKeyColumn = 1)
   BEGIN
   IF (@sWhereClause = '')
    SET @sWhereClause = @sWhereClause + 'Where '
   ELSE
    SET @sWhereClause = @sWhereClause + ' AND '

   SET @sWhereClause = @sWhereClause + @sTAB + @sColumnName   + ' = @' + @sColumnName + @sCRLF
   END
ELSE
   IF (@IsIdentity = 0)
    BEGIN
    IF (@sSetClause = '')
     SET @sSetClause = @sSetClause + 'SET'
    ELSE
     SET @sSetClause = @sSetClause + ',' + @sCRLF
    SET @sSetClause = @sSetClause + @sTAB + @sColumnName   + ' = '
    IF (@sTypeName = 'timestamp')
     SET @sSetClause = @sSetClause + 'NULL'
    ELSE IF (@sDefaultValue IS NOT NULL)
     SET @sSetClause = @sSetClause + 'COALESCE(@' + @sColumnName + ', ' + @sDefaultValue + ')'
    ELSE
     SET @sSetClause = @sSetClause + '@' + @sColumnName
    END

IF (@IsIdentity = 0)
   BEGIN
   IF (@IsNullable = 1) or (@sTypeName = 'timestamp')
    SET @sKeyFields = @sKeyFields + ' = NULL'
   END

FETCH   NEXT
FROM   crKeyFields
INTO   @sColumnName, @nColumnID, @bPrimaryKeyColumn, @nAlternateType,
   @nColumnLength, @nColumnPrecision, @nColumnScale, @IsNullable,
   @IsIdentity, @sTypeName, @sDefaultValue
END

CLOSE crKeyFields
DEALLOCATE crKeyFields

SET   @sSetClause = @sSetClause + @sCRLF

SET   @sProcText = @sProcText + @sKeyFields + @sCRLF
SET   @sProcText = @sProcText + 'AS' + @sCRLF
SET   @sProcText = @sProcText + @sCRLF
SET   @sProcText = @sProcText + 'Update ' + @sTableName + @sCRLF
SET   @sProcText = @sProcText + @sSetClause
SET   @sProcText = @sProcText + @sWhereClause
SET   @sProcText = @sProcText + @sCRLF
IF @bExecute = 0
SET   @sProcText = @sProcText + 'GO' + @sCRLF


PRINT @sProcText

IF @bExecute = 1
EXEC (@sProcText)


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

----------------------------- Proc_Update_end -------------------------------------

----------------------------- Function_begin   -------------------------------------
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

Create FUNCTION dbo.fnCleanDefaultValue(@sDefaultValue varchar(4000))
RETURNS varchar(4000)
AS
BEGIN
RETURN SubString(@sDefaultValue, 2, DataLength(@sDefaultValue)-2)
END


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

Create FUNCTION dbo.fnColumnDefault(@sTableName varchar(128), @sColumnName varchar(128))
RETURNS varchar(4000)
AS
BEGIN
DECLARE @sDefaultValue varchar(4000)

Select @sDefaultValue = dbo.fnCleanDefaultValue(COLUMN_DEFAULT)
FROM INFORMATION_SCHEMA.COLUMNS
Where TABLE_NAME = @sTableName
   AND   COLUMN_NAME = @sColumnName

RETURN   @sDefaultValue

END

GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO


Create    FUNCTION dbo.fnIsColumnPrimaryKey(@sTableName varchar(128), @nColumnName varchar(128))
RETURNS bit
AS
BEGIN
DECLARE @nTableID int,
   @nIndexID int,
   @i int

SET   @nTableID = OBJECT_ID(@sTableName)

Select   @nIndexID = indid
FROM   sysindexes
Where   id = @nTableID
   AND   indid BETWEEN 1 And 254
   AND   (status & 2048) = 2048

IF @nIndexID Is Null
   RETURN 0

IF @nColumnName IN
   (Select sc.[name]
   FROM   sysindexkeys sik
    INNER JOIN syscolumns sc ON sik.id = sc.id AND sik.colid = sc.colid
   Where   sik.id = @nTableID
    AND   sik.indid = @nIndexID)
   BEGIN
   RETURN 1
   END


RETURN 0
END


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO


Create        FUNCTION dbo.fnTableColumnInfo(@sTableName varchar(128))
RETURNS TABLE
AS
RETURN
Select c.name AS sColumnName,
   c.colid AS nColumnID,
   dbo.fnIsColumnPrimaryKey(@sTableName, c.name) AS bPrimaryKeyColumn,
   CASE   WHEN t.name IN ('char', 'varchar', 'binary', 'varbinary', 'nchar', 'nvarchar') THEN 1
    WHEN t.name IN ('decimal', 'numeric') THEN 2
    ELSE 0
   END AS nAlternateType,
   c.length AS nColumnLength,
   c.prec AS nColumnPrecision,
   c.scale AS nColumnScale,
   c.IsNullable,
   SIGN(c.status & 128) AS IsIdentity,
   t.name as sTypeName,
   dbo.fnColumnDefault(@sTableName, c.name) AS sDefaultValue
FROM syscolumns c
   INNER JOIN systypes t ON c.xtype = t.xtype and c.usertype = t.usertype
Where c.id = OBJECT_ID(@sTableName)


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO
SET ANSI_NULLS ON
GO

Create FUNCTION dbo.fnTableHasPrimaryKey(@sTableName varchar(128))
RETURNS bit
AS
BEGIN
DECLARE @nTableID int,
   @nIndexID int

SET   @nTableID = OBJECT_ID(@sTableName)

Select   @nIndexID = indid
FROM   sysindexes
Where   id = @nTableID
   AND   indid BETWEEN 1 And 254
   AND   (status & 2048) = 2048

IF @nIndexID IS NOT Null
   RETURN 1

RETURN 0
END


GO
SET QUOTED_IDENTIFIER OFF
GO
SET ANSI_NULLS ON
GO