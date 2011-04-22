USE [RedianHedian]
GO

/****** Object:  Table [dbo].[答复$]    Script Date: 04/22/2011 11:17:31 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[gprs_tousu]') AND type in (N'U'))
DROP TABLE [dbo].[gprs_tousu]
GO

USE [RedianHedian]
GO

/****** Object:  Table [dbo].[答复$]    Script Date: 04/22/2011 11:17:31 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[gprs_tousu](
	[请求编号] [nvarchar](255) NULL,
	[日期] [nvarchar](255) NULL,
	[受理时间] [datetime] NULL,
	[服务请求类别] [nvarchar](255) NULL,
	[受理工号] [nvarchar](255) NULL,
	[操作状态] [nvarchar](255) NULL,
	[用户级别] [nvarchar](255) NULL,
	[用户品牌] [nvarchar](255) NULL,
	[业务内容] [nvarchar](4000) NULL,
	[经度] [nvarchar](255) NULL,
	[纬度] [nvarchar](255) NULL
) ON [PRIMARY]

GO

BULK INSERT gprs_tousu
    FROM 'C:\3月份数据业务投诉信息.csv' 
    WITH 
    ( 
        FIRSTROW = 2,
        FIELDTERMINATOR = ',', 
        ROWTERMINATOR = '\n' 
    )


