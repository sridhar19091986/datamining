USE [RedianHedian]
GO

/****** Object:  Table [dbo].[��$]    Script Date: 04/22/2011 11:17:31 ******/
IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[gprs_tousu]') AND type in (N'U'))
DROP TABLE [dbo].[gprs_tousu]
GO

USE [RedianHedian]
GO

/****** Object:  Table [dbo].[��$]    Script Date: 04/22/2011 11:17:31 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[gprs_tousu](
	[������] [nvarchar](255) NULL,
	[����] [nvarchar](255) NULL,
	[����ʱ��] [datetime] NULL,
	[�����������] [nvarchar](255) NULL,
	[������] [nvarchar](255) NULL,
	[����״̬] [nvarchar](255) NULL,
	[�û�����] [nvarchar](255) NULL,
	[�û�Ʒ��] [nvarchar](255) NULL,
	[ҵ������] [nvarchar](4000) NULL,
	[����] [nvarchar](255) NULL,
	[γ��] [nvarchar](255) NULL
) ON [PRIMARY]

GO

BULK INSERT gprs_tousu
    FROM 'C:\3�·�����ҵ��Ͷ����Ϣ.csv' 
    WITH 
    ( 
        FIRSTROW = 2,
        FIELDTERMINATOR = ',', 
        ROWTERMINATOR = '\n' 
    )


