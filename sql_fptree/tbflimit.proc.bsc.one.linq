<Query Kind="Program">
  <Connection>
    <ID>92afe844-6bee-429a-8d93-c850e50afd51</ID>
    <Persist>true</Persist>
    <Server>localhost</Server>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
	{
//		var busyhour = from p in Redian_hedians
//					   group p by p.时间 into ttt
//					   select new
//					   {
//						   时间 = ttt.Key,
//						   T话务量 = ttt.Sum(e => e.T话务量)
//					   };
//
//		var bh = busyhour.OrderByDescending(e => e.T话务量).FirstOrDefault().时间;

		var tbf = from p in Redian_hedians
		          where p.BSC=="SZ46A"
				  where p.时间.Value.Day ==7
				  group p by p.时间 into ttt
				  select new
				  {
					  BSCnumber =ttt.Select(e=>e.BSC).FirstOrDefault(),
					  Ttime =  ttt.Key,
					  Tchan = ttt.Average(e => e.T可用信道),
					  Tferl = ttt.Average(e => e.F话务量),
					  Therl=ttt.Average(e=>e.H话务量),
					  Tedge=ttt.Average(e=>e.EDGE下行激活信道),
					  EdgeSubPerChan = ttt.Average(e => e.EDGE每线下行用户),
					  EdgeChan = ttt.Average(e => e.EDGE下行激活信道),
					  GprsSubPerChan = ttt.Average(e => e.GPRS每线下行用户),
					  GprsChan = ttt.Average(e => e.GPRS下行激活信道)
				  };

		var tbflimit = from p in tbf.ToList()
		               let tch=erlangbinv(0.02,p.Tferl+p.Therl/2)
					   select new
					   {
						   BSC = p.BSCnumber,
						   时间 = p.Ttime,
						   T可用信道 = p.Tchan,
						   T话务量 = p.Tferl+p.Therl,
						   EDGE每线下行用户 = p.EdgeSubPerChan,
						   EDGE下行激活信道 = p.EdgeChan,
						   GPRS每线下行用户 = p.GprsSubPerChan,
						   GPRS下行激活信道 = p.GprsChan,
						   TCHchan= tch,
						   //EDGEchan=p.Tedge,
						   //GPRSchan=p.Tchan - tch-p.Tedge,
						   TBFLIMIT =10* ((p.GprsSubPerChan * p.GprsChan+p.EdgeSubPerChan*p.EdgeChan) / 4) / ((p.Tchan - tch) / 8)
					   };
		tbflimit.OrderBy(e=>e.时间).Dump();

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