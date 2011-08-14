using System;
using System.Data;
using System.Data.SqlClient;
using System.ComponentModel;
using NovinMedia.Data;

namespace NovinMedia.Data.Layer
{
    [DataObject(true)]
    public class gprs_cell_rp
    {
		[DataObjectMethod(DataObjectMethodType.Fill)]
		public DataSet SelectAll()
        {
            DbObject dbo = new DbObject();
            SqlParameter[] parameters = new SqlParameter[]
                {
 
                };
            return dbo.RunProcedure("sp_gprs_cell_rp_SelectAll", parameters, "gprs_cell_rp");
        }

		[DataObjectMethod(DataObjectMethodType.Fill)]
		public DataSet SelectRow()
        {
            DbObject dbo = new DbObject();
            SqlParameter[] parameters = new SqlParameter[]
                {
 
                };
            return dbo.RunProcedure("sp_gprs_cell_rp_SelectRow", parameters, "gprs_cell_rp");
        }

		[DataObjectMethod(DataObjectMethodType.Insert)]
		public int InsertRow(string CELL,string NOPDCH,string RP)
		{
			int RowsAffected = 0;
			int Result = 0;
			DbObject dbo = new DbObject();
			SqlParameter[] parameters = new SqlParameter[]
				{
					new SqlParameter("CELL",CELL),
					new SqlParameter("NOPDCH",NOPDCH),
					new SqlParameter("RP",RP) 
				};
			Result = dbo.RunProcedure("sp_gprs_cell_rp_Insert", parameters, out RowsAffected);
			return Result;
        }

		[DataObjectMethod(DataObjectMethodType.Update)]
		public int UpdateRow(string CELL,string NOPDCH,string RP)
		{
			int RowsAffected = 0;
			int Result = 0;
			DbObject dbo = new DbObject();
			SqlParameter[] parameters = new SqlParameter[]
				{
					new SqlParameter("CELL",CELL),
					new SqlParameter("NOPDCH",NOPDCH),
					new SqlParameter("RP",RP) 
				};
			Result = dbo.RunProcedure("sp_gprs_cell_rp_Update", parameters, out RowsAffected);
			return Result;
        }

		[DataObjectMethod(DataObjectMethodType.Delete)]
		public int DeleteRow()
		{
			int RowsAffected = 0;
			int Result = 0;
			DbObject dbo = new DbObject();
			SqlParameter[] parameters = new SqlParameter[]
				{
 
				};
			Result = dbo.RunProcedure("sp_gprs_cell_rp_DeleteRow", parameters, out RowsAffected);
			return Result;
        }
    }
}