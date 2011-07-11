<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
	{
		var busyhour = from p in BSCGPRS2_20110707s
		              // let ttime=DateTime.Parse(p.时间)  //方法“System.DateTime Parse(System.String)”不支持转换为 SQL。
		               let ttime=Convert.ToDateTime(p.时间)
					   group p by ttime into ttt
					   select new
					   {
						   时间 = ttt.Key,
						   T话务量 = ttt.Sum(e => e.T话务量)
					   };
        var bh1 = busyhour.OrderByDescending(e => e.T话务量).ToList();
		var bh = bh1.FirstOrDefault().时间;

		var tbf = from p in  BSCGPRS2_20110707s
		          let ttime=Convert.ToDateTime(p.时间)
				  where ttime.Hour == 21
				  group p by p.BSC into ttt
				  select new
				  {
					  BSC = ttt.Key,
					  Ttime =21,
					  Tchan = ttt.Average(e => e.T可用信道),
					  Terl = ttt.Average(e => e.T话务量),
					  Thalf=ttt.Average(e=>e.H话务比),
	                  Tfault=ttt.Average(e=>e.T损坏率),
					  Tpdch=ttt.Average(e=>e.平均分配PDCH总数),
					  下行TBF建立成功率=ttt.Average(e=>e.下行TBF建立成功率),
					  EDGE终端使用EDGE比例=ttt.Average(e=>e.EDGE终端使用EDGE比例),
					  TusePDCH=ttt.Average(e=>e.PDCH复用度),
					  //Tt=ttt.Average(e=>e.
					  
					  //Tedge=ttt.Average(e=>e.EDGE下行激活信道),
					  //EdgeSubPerChan = ttt.Average(e => e.EDGE每线下行用户),
					  //EdgeChan = ttt.Average(e => e.EDGE下行激活信道),
					  //GprsSubPerChan = ttt.Average(e => e.GPRS每线下行用户),
					  //GprsChan = ttt.Average(e => e.GPRS下行激活信道)
				  };

		var tbflimit = from p in tbf.ToList()
		               let tch=erlangbinv(0.02,p.Terl)
					   select new
					   {
						   小区号 = p.BSC,
						   时间 = p.Ttime,
						   T可用信道 = p.Tchan,
						   T话务量 = p.Terl,
						   Tvoice=tch,
						   Tpdch=p.Tpdch,
						   Tuse=tch+p.Tpdch,
						   Ruse=(tch+p.Tpdch)/p.Tchan,
						   下行TBF建立成功率=p.下行TBF建立成功率,
						   p.Thalf,
						   p.Tfault,
						   p.EDGE终端使用EDGE比例,
						   p.TusePDCH
//						   EDGE每线下行用户 = p.EdgeSubPerChan,
//						   EDGE下行激活信道 = p.EdgeChan,
						   //GPRS每线下行用户 = p.GprsSubPerChan,
						  // GPRS下行激活信道 = p.GprsChan,
						   //TCHchan= tch,
						   //EDGEchan=p.Tedge,
						   //GPRSchan=p.Tchan - tch-p.Tedge,
						   //TBFLIMIT =10* ((p.GprsSubPerChan * p.GprsChan) / 4) / ((p.Tchan - tch-p.Tedge) / 8)
					   };
		tbflimit.OrderBy(e=>e.EDGE终端使用EDGE比例).Dump();

	}
	
		// Define other methods and classes here
	private int erlangbinv(double p, double? erlang)
	{
		double rho = (double)erlang;
		double B = 1;
		int n = 1;
		while (1 == 1)
		{
			B = ((rho * B) / n) / (1 + rho * B / n);
			if (B <= p)
				return n;
			n = n + 1;
		}
		return n;
	}