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

	string filename = @"C:\fptree.csv";
	File.Delete(filename);
	//创建一个文件流，用以写入或者创建一个StreamWriter
	FileStream fs = new FileStream ( filename , FileMode.Create, FileAccess.Write ) ;
	StreamWriter m_streamWriter = new StreamWriter ( fs ,Encoding.Default) ;
	m_streamWriter.Flush ( ) ;
	// 使用StreamWriter来往文件中写入内容
	m_streamWriter.BaseStream.Seek ( 0 , SeekOrigin.Begin ) ;

	var ttt = from p in 平均分配PDCH10000s
			  select p;
	var m = from p in ttt
			let stepbystep = 0.1 * ttt.Count()
							 select new
	{

GPRS下行流量 = (int)(p.GPRS下行流量/ ((ttt.Max(e => e.GPRS下行流量) - ttt.Min(e => e.GPRS下行流量)) / stepbystep)) + 0.001,
GPRS总流量 = (int)(p.GPRS总流量/ ((ttt.Max(e => e.GPRS总流量) - ttt.Min(e => e.GPRS总流量)) / stepbystep)) + 0.002,
EDGE下行流量 = (int)(p.EDGE下行流量/ ((ttt.Max(e => e.EDGE下行流量) - ttt.Min(e => e.EDGE下行流量)) / stepbystep)) + 0.003,
EDGE总流量 = (int)(p.EDGE总流量/ ((ttt.Max(e => e.EDGE总流量) - ttt.Min(e => e.EDGE总流量)) / stepbystep)) + 0.004,
PS下行流量 = (int)(p.PS下行流量/ ((ttt.Max(e => e.PS下行流量) - ttt.Min(e => e.PS下行流量)) / stepbystep)) + 0.005,
PS总流量 = (int)(p.PS总流量/ ((ttt.Max(e => e.PS总流量) - ttt.Min(e => e.PS总流量)) / stepbystep)) + 0.006,
GPRS下行速率 = (int)(p.GPRS下行速率/ ((ttt.Max(e => e.GPRS下行速率) - ttt.Min(e => e.GPRS下行速率)) / stepbystep)) + 0.007,
EDGE下行速率 = (int)(p.EDGE下行速率/ ((ttt.Max(e => e.EDGE下行速率) - ttt.Min(e => e.EDGE下行速率)) / stepbystep)) + 0.008,
EDGE每信道下行速率 = (int)(p.EDGE每信道下行速率/ ((ttt.Max(e => e.EDGE每信道下行速率) - ttt.Min(e => e.EDGE每信道下行速率)) / stepbystep)) + 0.009,
GPRS每信道下行速率 = (int)(p.GPRS每信道下行速率/ ((ttt.Max(e => e.GPRS每信道下行速率) - ttt.Min(e => e.GPRS每信道下行速率)) / stepbystep)) + 0.01,
GPRS每PFC下行流量 = (int)(p.GPRS每PFC下行流量/ ((ttt.Max(e => e.GPRS每PFC下行流量) - ttt.Min(e => e.GPRS每PFC下行流量)) / stepbystep)) + 0.011,
EDGE每PFC下行流量 = (int)(p.EDGE每PFC下行流量/ ((ttt.Max(e => e.EDGE每PFC下行流量) - ttt.Min(e => e.EDGE每PFC下行流量)) / stepbystep)) + 0.012,
下行TBF建立成功率 = (int)(p.下行TBF建立成功率/ ((ttt.Max(e => e.下行TBF建立成功率) - ttt.Min(e => e.下行TBF建立成功率)) / stepbystep)) + 0.013,
上行TBF建立成功率 = (int)(p.上行TBF建立成功率/ ((ttt.Max(e => e.上行TBF建立成功率) - ttt.Min(e => e.上行TBF建立成功率)) / stepbystep)) + 0.014,
上行TBF异常释放 = (int)(p.上行TBF异常释放/ ((ttt.Max(e => e.上行TBF异常释放) - ttt.Min(e => e.上行TBF异常释放)) / stepbystep)) + 0.015,
PS立即指派成功率 = (int)(p.PS立即指派成功率/ ((ttt.Max(e => e.PS立即指派成功率) - ttt.Min(e => e.PS立即指派成功率)) / stepbystep)) + 0.016,
上行TBF掉线率 = (int)(p.上行TBF掉线率/ ((ttt.Max(e => e.上行TBF掉线率) - ttt.Min(e => e.上行TBF掉线率)) / stepbystep)) + 0.017,
平均分配PDCH = (int)(p.平均分配PDCH/ ((ttt.Max(e => e.平均分配PDCH) - ttt.Min(e => e.平均分配PDCH)) / stepbystep)) + 0.018,
平均使用PDCH = (int)(p.平均使用PDCH/ ((ttt.Max(e => e.平均使用PDCH) - ttt.Min(e => e.平均使用PDCH)) / stepbystep)) + 0.019,
GPRS下行激活信道 = (int)(p.GPRS下行激活信道/ ((ttt.Max(e => e.GPRS下行激活信道) - ttt.Min(e => e.GPRS下行激活信道)) / stepbystep)) + 0.02,
GPRS每线下行用户 = (int)(p.GPRS每线下行用户/ ((ttt.Max(e => e.GPRS每线下行用户) - ttt.Min(e => e.GPRS每线下行用户)) / stepbystep)) + 0.021,
EDGE下行激活信道 = (int)(p.EDGE下行激活信道/ ((ttt.Max(e => e.EDGE下行激活信道) - ttt.Min(e => e.EDGE下行激活信道)) / stepbystep)) + 0.022,
EDGE每线下行用户 = (int)(p.EDGE每线下行用户/ ((ttt.Max(e => e.EDGE每线下行用户) - ttt.Min(e => e.EDGE每线下行用户)) / stepbystep)) + 0.023,
PDCH复用度 = (int)(p.PDCH复用度/ ((ttt.Max(e => e.PDCH复用度) - ttt.Min(e => e.PDCH复用度)) / stepbystep)) + 0.024,
PPSIMMASS = (int)(p.PPSIMMASS/ ((ttt.Max(e => e.PPSIMMASS) - ttt.Min(e => e.PPSIMMASS)) / stepbystep)) + 0.025,
REJPSIMMASS = (int)(p.REJPSIMMASS/ ((ttt.Max(e => e.REJPSIMMASS) - ttt.Min(e => e.REJPSIMMASS)) / stepbystep)) + 0.026,
PSCHREQ = (int)(p.PSCHREQ/ ((ttt.Max(e => e.PSCHREQ) - ttt.Min(e => e.PSCHREQ)) / stepbystep)) + 0.027,
PREJTFI = (int)(p.PREJTFI/ ((ttt.Max(e => e.PREJTFI) - ttt.Min(e => e.PREJTFI)) / stepbystep)) + 0.028,
PREJOTH = (int)(p.PREJOTH/ ((ttt.Max(e => e.PREJOTH) - ttt.Min(e => e.PREJOTH)) / stepbystep)) + 0.029,



	};
	m_streamWriter.WriteLine( "GPRS下行流量,GPRS总流量,EDGE下行流量,EDGE总流量,PS下行流量,PS总流量,GPRS下行速率,EDGE下行速率,EDGE每信道下行速率,GPRS每信道下行速率,GPRS每PFC下行流量,EDGE每PFC下行流量,下行TBF建立成功率,上行TBF建立成功率,上行TBF异常释放,PS立即指派成功率,上行TBF掉线率,平均分配PDCH,平均使用PDCH,GPRS下行激活信道,GPRS每线下行用户,EDGE下行激活信道,EDGE每线下行用户,PDCH复用度,PPSIMMASS,REJPSIMMASS,PSCHREQ,PREJTFI,PREJOTH,") ;
	foreach(var p in m)
	{
		string strs = null;


strs += p.GPRS下行流量+",";
strs += p.GPRS总流量+",";
strs += p.EDGE下行流量+",";
strs += p.EDGE总流量+",";
strs += p.PS下行流量+",";
strs += p.PS总流量+",";
strs += p.GPRS下行速率+",";
strs += p.EDGE下行速率+",";
strs += p.EDGE每信道下行速率+",";
strs += p.GPRS每信道下行速率+",";
strs += p.GPRS每PFC下行流量+",";
strs += p.EDGE每PFC下行流量+",";
strs += p.下行TBF建立成功率+",";
strs += p.上行TBF建立成功率+",";
strs += p.上行TBF异常释放+",";
strs += p.PS立即指派成功率+",";
strs += p.上行TBF掉线率+",";
strs += p.平均分配PDCH+",";
strs += p.平均使用PDCH+",";
strs += p.GPRS下行激活信道+",";
strs += p.GPRS每线下行用户+",";
strs += p.EDGE下行激活信道+",";
strs += p.EDGE每线下行用户+",";
strs += p.PDCH复用度+",";
strs += p.PPSIMMASS+",";
strs += p.REJPSIMMASS+",";
strs += p.PSCHREQ+",";
strs += p.PREJTFI+",";
strs += p.PREJOTH+",";



		// 把richTextBox1中的内容写入文件
		m_streamWriter.WriteLine( strs ) ;
		//关闭此文件
		m_streamWriter.Flush ( ) ;
	}
	m_streamWriter.Close ( ) ;
	//m.Dump();
}

