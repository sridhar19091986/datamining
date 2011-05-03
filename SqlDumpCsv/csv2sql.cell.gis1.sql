USE [RedianHedian]
GO

/****** Object:  Table [dbo].[´ð¸´$]    Script Date: 04/22/2011 11:17:31 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[gprs_cell_gis]') AND type in (N'U'))
DROP TABLE [dbo].[gprs_cell_gis]
GO

USE [RedianHedian]
GO

/****** Object:  Table [dbo].[´ð¸´$]    Script Date: 04/22/2011 11:17:31 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[gprs_cell_gis](
    [bsc] [nvarchar](255) NULL,
	[cell_name] [nvarchar](255) NULL,
	[cell_no] [nvarchar](255) NULL,
	[latitude] [float] NULL,
	[longitude] [float] NULL,
	[»ùÕ¾´úºÅ] [nvarchar](255) NULL,
	[»ùÕ¾Ãû] [nvarchar](255) NULL
) ON [PRIMARY]

GO

BULK INSERT gprs_cell_gis
    FROM 'C:\Gprs_cissss.txt' 
    WITH 
    ( 
        FIRSTROW = 2,
        FIELDTERMINATOR = ',', 
        ROWTERMINATOR = '\n' 
    )


