--
-- Dropping stored procedure sp_spt_fallback_db_SelectAll : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_spt_fallback_db_SelectAll]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_spt_fallback_db_SelectAll]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_spt_fallback_db_SelectAll
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 12:59:10
-- Description:	This stored procedure is intended for selecting all rows from spt_fallback_db table
-- ==========================================================================================
Create Procedure sp_spt_fallback_db_SelectAll
As
Begin
	Select 
		[xserver_name],
		[xdttm_ins],
		[xdttm_last_ins_upd],
		[xfallback_dbid],
		[name],
		[dbid],
		[status],
		[version]
	From spt_fallback_db
End

GO


--
-- Dropping stored procedure sp_spt_fallback_db_SelectRow : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_spt_fallback_db_SelectRow]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_spt_fallback_db_SelectRow]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_spt_fallback_db_SelectRow
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 12:59:10
-- Description:	This stored procedure is intended for selecting a specific row from spt_fallback_db table
-- ==========================================================================================
Create Procedure sp_spt_fallback_db_SelectRow

As
Begin
	Select 
		[xserver_name],
		[xdttm_ins],
		[xdttm_last_ins_upd],
		[xfallback_dbid],
		[name],
		[dbid],
		[status],
		[version]
	From spt_fallback_db
	Where

End

GO


--
-- Dropping stored procedure sp_spt_fallback_db_Insert : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_spt_fallback_db_Insert]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_spt_fallback_db_Insert]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_spt_fallback_db_Insert
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 12:59:10
-- Description:	This stored procedure is intended for inserting values to spt_fallback_db table
-- ==========================================================================================
Create Procedure sp_spt_fallback_db_Insert
	@xserver_name varchar(30),
	@xdttm_ins datetime,
	@xdttm_last_ins_upd datetime,
	@xfallback_dbid smallint,
	@name varchar(30),
	@dbid smallint,
	@status smallint,
	@version smallint
As
Begin
	Insert Into spt_fallback_db
		([xserver_name],[xdttm_ins],[xdttm_last_ins_upd],[xfallback_dbid],[name],[dbid],[status],[version])
	Values
		(@xserver_name,@xdttm_ins,@xdttm_last_ins_upd,@xfallback_dbid,@name,@dbid,@status,@version)

End

GO


--
-- Dropping stored procedure sp_spt_fallback_db_Update : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_spt_fallback_db_Update]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_spt_fallback_db_Update]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_spt_fallback_db_Update
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 12:59:10
-- Description:	This stored procedure is intended for updating spt_fallback_db table
-- ==========================================================================================
Create Procedure sp_spt_fallback_db_Update
	@xserver_name varchar(30),
	@xdttm_ins datetime,
	@xdttm_last_ins_upd datetime,
	@xfallback_dbid smallint,
	@name varchar(30),
	@dbid smallint,
	@status smallint,
	@version smallint
As
Begin
	Update spt_fallback_db
	Set
		[xserver_name] = @xserver_name,
		[xdttm_ins] = @xdttm_ins,
		[xdttm_last_ins_upd] = @xdttm_last_ins_upd,
		[xfallback_dbid] = @xfallback_dbid,
		[name] = @name,
		[dbid] = @dbid,
		[status] = @status,
		[version] = @version
	Where		

End

GO


--
-- Dropping stored procedure sp_spt_fallback_db_DeleteRow : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_spt_fallback_db_DeleteRow]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_spt_fallback_db_DeleteRow]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_spt_fallback_db_DeleteRow
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 12:59:10
-- Description:	This stored procedure is intended for deleting a specific row from spt_fallback_db table
-- ==========================================================================================
Create Procedure sp_spt_fallback_db_DeleteRow

As
Begin
	Delete spt_fallback_db
	Where


End

GO
