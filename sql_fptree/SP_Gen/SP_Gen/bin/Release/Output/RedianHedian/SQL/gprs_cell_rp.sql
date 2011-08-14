--
-- Dropping stored procedure sp_gprs_cell_rp_SelectAll : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_gprs_cell_rp_SelectAll]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_gprs_cell_rp_SelectAll]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_gprs_cell_rp_SelectAll
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 0:04:48
-- Description:	This stored procedure is intended for selecting all rows from gprs_cell_rp table
-- ==========================================================================================
Create Procedure sp_gprs_cell_rp_SelectAll
As
Begin
	Select 
		[CELL],
		[NOPDCH],
		[RP]
	From gprs_cell_rp
End

GO


--
-- Dropping stored procedure sp_gprs_cell_rp_SelectRow : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_gprs_cell_rp_SelectRow]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_gprs_cell_rp_SelectRow]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_gprs_cell_rp_SelectRow
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 0:04:48
-- Description:	This stored procedure is intended for selecting a specific row from gprs_cell_rp table
-- ==========================================================================================
Create Procedure sp_gprs_cell_rp_SelectRow

As
Begin
	Select 
		[CELL],
		[NOPDCH],
		[RP]
	From gprs_cell_rp
	Where

End

GO


--
-- Dropping stored procedure sp_gprs_cell_rp_Insert : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_gprs_cell_rp_Insert]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_gprs_cell_rp_Insert]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_gprs_cell_rp_Insert
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 0:04:48
-- Description:	This stored procedure is intended for inserting values to gprs_cell_rp table
-- ==========================================================================================
Create Procedure sp_gprs_cell_rp_Insert
	@CELL nvarchar(255),
	@NOPDCH nvarchar(255),
	@RP nvarchar(255)
As
Begin
	Insert Into gprs_cell_rp
		([CELL],[NOPDCH],[RP])
	Values
		(@CELL,@NOPDCH,@RP)

End

GO


--
-- Dropping stored procedure sp_gprs_cell_rp_Update : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_gprs_cell_rp_Update]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_gprs_cell_rp_Update]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_gprs_cell_rp_Update
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 0:04:48
-- Description:	This stored procedure is intended for updating gprs_cell_rp table
-- ==========================================================================================
Create Procedure sp_gprs_cell_rp_Update
	@CELL nvarchar(255),
	@NOPDCH nvarchar(255),
	@RP nvarchar(255)
As
Begin
	Update gprs_cell_rp
	Set
		[CELL] = @CELL,
		[NOPDCH] = @NOPDCH,
		[RP] = @RP
	Where		

End

GO


--
-- Dropping stored procedure sp_gprs_cell_rp_DeleteRow : 
--

IF EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[sp_gprs_cell_rp_DeleteRow]') AND OBJECTPROPERTY(id, N'IsProcedure') = 1)
  DROP PROCEDURE [dbo].[sp_gprs_cell_rp_DeleteRow]
  
GO

-- ==========================================================================================
-- Entity Name:	sp_gprs_cell_rp_DeleteRow
-- Author:	Mehdi Keramati
-- Create date:	2011-05-02 0:04:48
-- Description:	This stored procedure is intended for deleting a specific row from gprs_cell_rp table
-- ==========================================================================================
Create Procedure sp_gprs_cell_rp_DeleteRow

As
Begin
	Delete gprs_cell_rp
	Where


End

GO
