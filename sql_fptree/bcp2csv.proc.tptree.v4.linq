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
	var ttt = from p in Pdchredian1000s
			  select p;
	var m = from p in ttt
			let stepbystep = 0.1 * ttt.Count()
							 select new
	{

		GPRS下行流量 = (int)(p.GPRS下行流量 / ((ttt.Max(e => e.GPRS下行流量) - ttt.Min(e => e.GPRS下行流量)) / stepbystep)) + 0.005,
		GPRS总流量 = (int)(p.GPRS总流量 / ((ttt.Max(e => e.GPRS总流量) - ttt.Min(e => e.GPRS总流量)) / stepbystep)) + 0.006,
		EDGE下行流量 = (int)(p.EDGE下行流量 / ((ttt.Max(e => e.EDGE下行流量) - ttt.Min(e => e.EDGE下行流量)) / stepbystep)) + 0.007,
		EDGE总流量 = (int)(p.EDGE总流量 / ((ttt.Max(e => e.EDGE总流量) - ttt.Min(e => e.EDGE总流量)) / stepbystep)) + 0.008,
		PS下行流量 = (int)(p.PS下行流量 / ((ttt.Max(e => e.PS下行流量) - ttt.Min(e => e.PS下行流量)) / stepbystep)) + 0.009,
		PS总流量 = (int)(p.PS总流量 / ((ttt.Max(e => e.PS总流量) - ttt.Min(e => e.PS总流量)) / stepbystep)) + 0.01,
		GPRS下行速率 = (int)(p.GPRS下行速率 / ((ttt.Max(e => e.GPRS下行速率) - ttt.Min(e => e.GPRS下行速率)) / stepbystep)) + 0.011,
		EDGE下行速率 = (int)(p.EDGE下行速率 / ((ttt.Max(e => e.EDGE下行速率) - ttt.Min(e => e.EDGE下行速率)) / stepbystep)) + 0.012,
		EDGE每信道下行速率 = (int)(p.EDGE每信道下行速率 / ((ttt.Max(e => e.EDGE每信道下行速率) - ttt.Min(e => e.EDGE每信道下行速率)) / stepbystep)) + 0.013,
		GPRS每信道下行速率 = (int)(p.GPRS每信道下行速率 / ((ttt.Max(e => e.GPRS每信道下行速率) - ttt.Min(e => e.GPRS每信道下行速率)) / stepbystep)) + 0.014,
		GPRS每PFC下行流量 = (int)(p.GPRS每PFC下行流量 / ((ttt.Max(e => e.GPRS每PFC下行流量) - ttt.Min(e => e.GPRS每PFC下行流量)) / stepbystep)) + 0.015,
		EDGE每PFC下行流量 = (int)(p.EDGE每PFC下行流量 / ((ttt.Max(e => e.EDGE每PFC下行流量) - ttt.Min(e => e.EDGE每PFC下行流量)) / stepbystep)) + 0.016,
		下行TBF建立成功率 = (int)(p.下行TBF建立成功率 / ((ttt.Max(e => e.下行TBF建立成功率) - ttt.Min(e => e.下行TBF建立成功率)) / stepbystep)) + 0.017,
		上行TBF建立成功率 = (int)(p.上行TBF建立成功率 / ((ttt.Max(e => e.上行TBF建立成功率) - ttt.Min(e => e.上行TBF建立成功率)) / stepbystep)) + 0.018,
		上行TBF异常释放 = (int)(p.上行TBF异常释放 / ((ttt.Max(e => e.上行TBF异常释放) - ttt.Min(e => e.上行TBF异常释放)) / stepbystep)) + 0.019,
		PS立即指派成功率 = (int)(p.PS立即指派成功率 / ((ttt.Max(e => e.PS立即指派成功率) - ttt.Min(e => e.PS立即指派成功率)) / stepbystep)) + 0.02,
		上行TBF掉线率 = (int)(p.上行TBF掉线率 / ((ttt.Max(e => e.上行TBF掉线率) - ttt.Min(e => e.上行TBF掉线率)) / stepbystep)) + 0.021,
		平均分配PDCH = (int)(p.平均分配PDCH / ((ttt.Max(e => e.平均分配PDCH) - ttt.Min(e => e.平均分配PDCH)) / stepbystep)) + 0.022,
		平均使用PDCH = (int)(p.平均使用PDCH / ((ttt.Max(e => e.平均使用PDCH) - ttt.Min(e => e.平均使用PDCH)) / stepbystep)) + 0.023,
		GPRS下行激活信道 = (int)(p.GPRS下行激活信道 / ((ttt.Max(e => e.GPRS下行激活信道) - ttt.Min(e => e.GPRS下行激活信道)) / stepbystep)) + 0.024,
		GPRS每线下行用户 = (int)(p.GPRS每线下行用户 / ((ttt.Max(e => e.GPRS每线下行用户) - ttt.Min(e => e.GPRS每线下行用户)) / stepbystep)) + 0.025,
		EDGE下行激活信道 = (int)(p.EDGE下行激活信道 / ((ttt.Max(e => e.EDGE下行激活信道) - ttt.Min(e => e.EDGE下行激活信道)) / stepbystep)) + 0.026,
		EDGE每线下行用户 = (int)(p.EDGE每线下行用户 / ((ttt.Max(e => e.EDGE每线下行用户) - ttt.Min(e => e.EDGE每线下行用户)) / stepbystep)) + 0.027,
		PDCH复用度 = (int)(p.PDCH复用度 / ((ttt.Max(e => e.PDCH复用度) - ttt.Min(e => e.PDCH复用度)) / stepbystep)) + 0.028
	};
	string strs = null;
	foreach(var p in m)
	{

		strs += p.GPRS下行流量 +",";
		strs += p.GPRS总流量 +",";
		strs += p.EDGE下行流量 +",";
		strs += p.EDGE总流量 +",";
		strs += p.PS下行流量 +",";
		strs += p.PS总流量 +",";
		strs += p.GPRS下行速率 +",";
		strs += p.EDGE下行速率 +",";
		strs += p.EDGE每信道下行速率 +",";
		strs += p.GPRS每信道下行速率 +",";
		strs += p.GPRS每PFC下行流量 +",";
		strs += p.EDGE每PFC下行流量 +",";
		strs += p.下行TBF建立成功率 +",";
		strs += p.上行TBF建立成功率 +",";
		strs += p.上行TBF异常释放 +",";
		strs += p.PS立即指派成功率 +",";
		strs += p.上行TBF掉线率 +",";
		strs += p.平均分配PDCH + ",";
		strs += p.平均使用PDCH + ",";
		strs += p.GPRS下行激活信道 +",";
		strs += p.GPRS每线下行用户 +",";
		strs += p.EDGE下行激活信道 +",";
		strs += p.EDGE每线下行用户 +",";
		strs += p.PDCH复用度 +",";



		strs += "\r\n";
	}
	write_rp(strs, @"C:\fptree.txt");
	m.Dump();
}

// Define other methods and classes here
void write_rp(string line, string filename)
{
	File.Delete(filename);
	//创建一个文件流，用以写入或者创建一个StreamWriter
	FileStream fs = new FileStream ( filename , FileMode.Create, FileAccess.Write ) ;
	StreamWriter m_streamWriter = new StreamWriter ( fs ) ;
	m_streamWriter.Flush ( ) ;
	// 使用StreamWriter来往文件中写入内容
	m_streamWriter.BaseStream.Seek ( 0 , SeekOrigin.Begin ) ;
	// 把richTextBox1中的内容写入文件
	m_streamWriter.WriteLine( line ) ;
	//关闭此文件
	m_streamWriter.Flush ( ) ;
	m_streamWriter.Close ( ) ;
}