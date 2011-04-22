USE [RedianHedian]
GO

/****** Object:  Table [dbo].[´ð¸´$]    Script Date: 04/22/2011 11:17:31 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[gprs_cell_rp]') AND type in (N'U'))
DROP TABLE [dbo].[gprs_cell_rp]
GO

USE [RedianHedian]
GO

/****** Object:  Table [dbo].[´ð¸´$]    Script Date: 04/22/2011 11:17:31 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO



CREATE TABLE [dbo].[gprs_cell_rp](
	[CELL] [nvarchar](255) NULL,
	[NOPDCH] [nvarchar](255) NULL,
	[RP] [nvarchar](255) NULL,
) ON [PRIMARY]

GO

BULK INSERT gprs_cell_rp
    FROM 'C:\20110421RPP2.csv' 
    WITH 
    ( 
        FIRSTROW = 2,
        FIELDTERMINATOR = ',', 
        ROWTERMINATOR = '\n' 
    )


