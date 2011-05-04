<Query Kind="Program">
  <Connection>
    <ID>337b631f-296e-4a3b-aa36-09f23a9a9a38</ID>
    <Server>.\SQLEXPRESS</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
    GPRSossCollection oss=new GPRSossCollection();
    Dictionary<Gprs_tousu, double> d=new Dictionary<Gprs_tousu, double>();
		  
	foreach(var m in  Gprs_tousus)
	{
	   GPRStuple g=new GPRStuple(m,oss.oss);
	   d.Add(m,g.Get_RPPLOAD_Maybe());
	}
	    
}

	// Define other methods and classes here
	public class GPRStuple
	{
//		public Gprs_tousu tousu=new Gprs_tousu();
//		public List<GPRSoss> oss=new List<GPRSoss>();
		private double distance;
		private double lat1;
		private double lon1;
		private double lat2;
		private double lon2;
		private Dictionary<GPRSoss, double> tousu_oss_distance=new Dictionary<GPRSoss, double>();
		public  GPRStuple(Gprs_tousu tousu, List<GPRSoss> oss)
		{
			foreach(var o in oss)
			{
			   lat1= L2Lon(o.Latitude);
			   lat2=T2Lat(tousu.经度) ;
			   lon1=L2Lon(o.Longitude);
			   lon2=T2Lon(tousu.纬度);
			   distance =(lat1-lat2)*(lat1-lat2)+(lon1-lon2)*(lon1-lon2);
			   tousu_oss_distance.Add(o,distance);
			}
		}
		
		private double Min_Distance()
		{
		   return tousu_oss_distance.Values.Min();
		}
		public double Get_RPPLOAD_Maybe()	
		{
		   double minv=Min_Distance();
		   if (minv==0) return 0;
		   return tousu_oss_distance.Where(e=>e.Value==minv).Select(e=>e.Key).Sum(e=>int.Parse(e.RPPLOAD));
		}
		
		
private double L2Lat (string input)
{
string  ttt;
	if(input.Length<2) return 0;
	if(input.IndexOf("tude") !=-1 || input.IndexOf("#") !=-1) return 0;
	string l=input.Replace("N","").Replace("-","").Replace(".","");
	string a=l.Substring(0,2);
	string b=l.Substring(2,l.Length-2);
	ttt=a+"."+b;
	return double.Parse(ttt);
}
// Define other methods and classes here

private double L2Lon (string input)
{
string  nnn;
	if(input.Length<2) return 0;
	if(input.IndexOf("tude") !=-1 || input.IndexOf("#") !=-1) return 0;
	string l=input.Replace("E","").Replace("-","").Replace(".","");
	string a=l.Substring(0,3);
	string b=l.Substring(3,l.Length-3);
	nnn=a+"."+b;
	return double.Parse(nnn);
}
private double T2Lat(string input)
{
 if(input.IndexOf("无") !=-1) return 0;
  return double.Parse(input);
}
private double T2Lon(string input)
{
 if(input.IndexOf("无") !=-1) return 0;
 return double.Parse(input);
}
	}
	
	public class GPRSoss
	{
		public string BSC;
		public string RPPID;
		public string RPPLOAD;
		public string Cell_name;
		public string Cell_no;
		public string Latitude;
		public string Longitude;
		public string 基站代号;
		public string 基站名;
		public string CELL;
		public string NOPDCH;
		public string RP;
	}
	public class GPRSossCollection
	{
		public List<GPRSoss> oss=new List<GPRSoss>();
		
		public GPRSossCollection()
		{
		   oss=get().ToList();
		}
			public IEnumerable<GPRSoss> get()
		{
			var m = from p in Gprs_cis.ToList()//Gprs_cell_gis
					from t in Gprs_cell_rps
					where p.Cell_no == t.CELL
					select new { p, t };
			// m.Dump();
			var n = from q in Gprs_rpps
					group q by new { q.BSC, q.RPPID } into ttt
					select new { BSC = ttt.Key.BSC, RPPID = ttt.Key.RPPID, RPPLOAD = ttt.Sum(e => e.RPPLOAD) };

			var k = from q in n
					from t in m
					where q.BSC == t.p.Bsc && q.RPPID.ToString() == t.t.RP
					select new
					{
						q.BSC,
						q.RPPID,
						q.RPPLOAD,
						t.p.Bsc,
						t.p.Cell_name,
						t.p.Cell_no,
						t.p.Latitude,
						t.p.Longitude,
						t.p.基站代号,
						t.p.基站名,
						t.t.CELL,
						t.t.NOPDCH,
						t.t.RP
					};


			foreach (var l in k)
			{
				GPRSoss oss = new GPRSoss();
				oss.BSC = l.BSC;
				oss.CELL = l.CELL;
				oss.Cell_name = l.Cell_name;
				oss.Cell_no = l.Cell_no;
				oss.Latitude = l.Latitude;
				oss.Longitude = l.Longitude;
				oss.NOPDCH = l.NOPDCH;
				oss.RP = l.RP;
				oss.RPPID = l.RPPID;
				oss.RPPLOAD = l.RPPLOAD;
				oss.基站代号 = l.基站代号;
				oss.基站名 = l.基站名;
				yield return oss;
			}

		}
	}