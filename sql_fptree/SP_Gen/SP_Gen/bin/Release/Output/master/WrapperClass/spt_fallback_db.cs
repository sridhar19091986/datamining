using System;
using System.Data;
using System.Data.SqlClient;
using System.ComponentModel;
using NovinMedia.Data;

namespace NovinMedia.Data.Layer
{
    [DataObject(true)]
    public class spt_fallback_db
    {
		[DataObjectMethod(DataObjectMethodType.Fill)]
		public DataSet SelectAll()
        {
            DbObject dbo = new DbObject();
            SqlParameter[] parameters = new SqlParameter[]
                {
 
                };
            return dbo.RunProcedure("sp_spt_fallback_db_SelectAll", parameters, "spt_fallback_db");
        }

		[DataObjectMethod(DataObjectMethodType.Fill)]
		public DataSet SelectRow()
        {
            DbObject dbo = new DbObject();
            SqlParameter[] parameters = new SqlParameter[]
                {
 
                };
            return dbo.RunProcedure("sp_spt_fallback_db_SelectRow", parameters, "spt_fallback_db");
        }

		[DataObjectMethod(DataObjectMethodType.Insert)]
		public int InsertRow(string xserver_name,DateTime xdttm_ins,DateTime xdttm_last_ins_upd,short xfallback_dbid,string name,short dbid,short status,short version)
		{
			int RowsAffected = 0;
			int Result = 0;
			DbObject dbo = new DbObject();
			SqlParameter[] parameters = new SqlParameter[]
				{
					new SqlParameter("xserver_name",xserver_name),
					new SqlParameter("xdttm_ins",xdttm_ins),
					new SqlParameter("xdttm_last_ins_upd",xdttm_last_ins_upd),
					new SqlParameter("xfallback_dbid",xfallback_dbid),
					new SqlParameter("name",name),
					new SqlParameter("dbid",dbid),
					new SqlParameter("status",status),
					new SqlParameter("version",version) 
				};
			Result = dbo.RunProcedure("sp_spt_fallback_db_Insert", parameters, out RowsAffected);
			return Result;
        }

		[DataObjectMethod(DataObjectMethodType.Update)]
		public int UpdateRow(string xserver_name,DateTime xdttm_ins,DateTime xdttm_last_ins_upd,short xfallback_dbid,string name,short dbid,short status,short version)
		{
			int RowsAffected = 0;
			int Result = 0;
			DbObject dbo = new DbObject();
			SqlParameter[] parameters = new SqlParameter[]
				{
					new SqlParameter("xserver_name",xserver_name),
					new SqlParameter("xdttm_ins",xdttm_ins),
					new SqlParameter("xdttm_last_ins_upd",xdttm_last_ins_upd),
					new SqlParameter("xfallback_dbid",xfallback_dbid),
					new SqlParameter("name",name),
					new SqlParameter("dbid",dbid),
					new SqlParameter("status",status),
					new SqlParameter("version",version) 
				};
			Result = dbo.RunProcedure("sp_spt_fallback_db_Update", parameters, out RowsAffected);
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
			Result = dbo.RunProcedure("sp_spt_fallback_db_DeleteRow", parameters, out RowsAffected);
			return Result;
        }
    }
}