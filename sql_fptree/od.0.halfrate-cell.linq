<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

	void Main()
	{
		var hf = from p in Redian_hedians
				 where p.时间.Value.Hour == 21
				 group p by p.小区名 into ttt
				 select new
				 {
					 Cell = ttt.Key,
					 Tedge= ttt.Max(e => e.EDGE下行激活信道),
					 Tchal = ttt.Max(e => e.T可用信道) ,
					 Ttraffic = ttt.Average(e => e.T话务量)
				 };
		//hf.Dump();
		var hft = from p in hf.ToList()
				  where p.Ttraffic > 0 && p.Tchal > 0
				  select new
				  {
					  小区名 = p.Cell,
					  T可用信道 = p.Tchal,
					  Edge信道=p.Tedge,
					  T话务量21点一周平均 = p.Ttraffic,
					  半速率百分之二十折算T话务量 = 0.9 * p.Ttraffic,
					  GOS=0.02,
					  由GOS计算半速率百分之二十折算T话务量的信道数 = erlangbinv(0.02, 0.9 * p.Ttraffic),
                      GPRS的OD为0时的半速率百分之二十需求的T信道数=erlangbinv(0.02, 0.9 * p.Ttraffic)-(p.Tchal-p.Tedge),
					  忽略GOS在GPRS的OD为0时的半速率 = (1-(p.Tchal-p.Tedge) / p.Ttraffic) * 2.0
				  };
		//hft.Where(e=>e.OD为0时的半速率>0.2).Count();
		//hft.OrderByDescending(e => e.GPRS的OD为0时的半速率百分之二十需求的T信道数).Dump();
		
		var dump=hft.OrderByDescending(e => e.GPRS的OD为0时的半速率百分之二十需求的T信道数).ToList();
		
		dump.Dump();
		dump.Skip(1000).Dump();
		dump.Skip(2000).Dump();
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