<Query Kind="Program">
  <Connection>
    <ID>76932288-1882-4022-a8fb-c01693c0810c</ID>
    <Persist>true</Persist>
    <Driver Assembly="IQDriver" PublicKeyToken="5b59726538a49684">IQDriver.IQDriver</Driver>
    <Provider>Devart.Data.MySql</Provider>
    <CustomCxString>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAACTNuYXkfk66zdd04KiO8wGAAAAAASAAACgAAAAEAAAAIOK7HFTUbC4NvHwmY54AThAAAAAYKS1JdvPz5iWCN+bVClSqdEdGOTZYMPe388yxJbFLsrQLNQremir6pv+EuhDT42GyAqHjKDtruU3cS1x7UlBXBQAAAA9C6jhKoipfqKkln91vqhRHssrBA==</CustomCxString>
    <Server>localhost</Server>
    <UserName>root</UserName>
    <Password>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAADkrYJ5ii5dOlB54Q4g4VNRAAAAAASAAACgAAAAEAAAALRZpGA5TrMbTQf5Q7s/6/IIAAAAo4mX5DXQQiEUAAAArcrXZk87oLcEm3N3crj52U5a5Ak=</Password>
    <EncryptCustomCxString>true</EncryptCustomCxString>
    <Database>test</Database>
    <DriverData>
      <StripUnderscores>false</StripUnderscores>
      <QuietenAllCaps>false</QuietenAllCaps>
      <Port>3306</Port>
    </DriverData>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	var neigh=from p in Ne_table_news
	          select p;
			  
	string abc="";
	Text2Class tc=new Text2Class();
	foreach(var n in neigh)
	{
	if(tc.CellBaList.Where(e=>e.cell==n.Cell).Any())
		if(tc.CellBaList.Where(e=>e.cell==n.Cell).Select(e=>e.ba).First().Count>0)
	{
	    n.Active_balist=tc.CellBaList.Where(e=>e.mode=="ACTIVE").Where(e=>e.cell==n.Cell).Select(e=>e.ba).First()
		.Aggregate((x, y)=> x +","+y).ToString();
		
		n.Idle_balist=tc.CellBaList.Where(e=>e.mode!="ACTIVE").Where(e=>e.cell==n.Cell).Select(e=>e.ba).First()
		.Aggregate((x, y)=> x +","+y).ToString();
		
		 n.Active_balist.Dump();
		 n.Idle_balist.Dump();
	}
		//.Aggregate ("g", (seed, n) => seed + n).Dump();
		//Console.WriteLine(n.Active_balist);
	}
	this.SubmitChanges();
		
}

// Define other methods and classes here
	public class Text2Class
	{
		public List<CellBA> CellBaList;
		public Text2Class()
		{
			CellBaList = new List<CellBA>();
			StreamRead2Class();
		}

		private string linestring;
		private string line;

		private void StreamRead2Class()
		{
			StreamReader sr = new StreamReader( @"G:\seemapcell\Linq2SqlGeography\abisLocating\Linq2SqlGeography\bin\Debug\MCOM_20111027\BA表.txt");
			//string line;
			while ((line = sr.ReadLine()) != null)
			{
				if (!line.StartsWith("CELL"))
				{
					Regex r = new Regex(@"\s+");
					linestring = r.Replace(line, @" ");
					var ln = linestring.Split(new Char[] { ' ' });
					CellBA cb = new CellBA();
					cb.cell = ln[0];
					cb.mode = ln[1];
					for (int i = 2; i < ln.Length; i++)
						if (ln[i].Trim().Length > 0)
							cb.ba.Add(ln[i].Trim());
					//Console.WriteLine(cb.cell);
					CellBaList.Add(cb);
				}
			}
		}
	}
	public class CellBA
	{
		public string cell;
		public string mode;
		public List<string> ba =new List<string>();
	}