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
		var busyhour = from p in SZ24B_hedians
					   group p by p.时间 into ttt
					   select new
					   {
						   时间 = ttt.Key,
						   T话务量 = ttt.Sum(e => e.T话务量)
					   };

		var bh = busyhour.OrderByDescending(e => e.T话务量).FirstOrDefault().时间;

		var tbf = from p in SZ24B_hedians
				  where p.时间.Value.Hour == bh.Value.Hour
				  group p by p.小区号 into ttt
				  select new
				  {
					  CellNumber = ttt.Key,
					  Ttime = bh.Value.Hour,
					  Tchan = ttt.Max(e => e.T可用信道),
					  Terl = ttt.Average(e => e.T话务量),
					  EdgeSubPerChan = ttt.Average(e => e.EDGE每线下行用户),
					  EdgeChan = ttt.Average(e => e.EDGE下行激活信道),
					  GprsSubPerChan = ttt.Average(e => e.GPRS每线下行用户),
					  GprsChan = ttt.Average(e => e.GPRS下行激活信道)
				  };

		var tbflimit = from p in tbf
					   select new
					   {
						   小区号 = p.CellNumber,
						   时间 = p.Ttime,
						   T可用信道 = p.Tchan,
						   T话务量 = p.Terl,
						   EDGE每线下行用户 = p.EdgeSubPerChan,
						   EDGE下行激活信道 = p.EdgeChan,
						   GPRS每线下行用户 = p.GprsSubPerChan,
						   GPRS下行激活信道 = p.GprsChan,
						   TBFLIMIT = ((p.EdgeSubPerChan * p.EdgeChan + p.GprsSubPerChan * p.GprsChan) / 4) / ((p.Tchan - p.Terl / 0.9) / 8)
					   };
		tbflimit.Dump();

	}