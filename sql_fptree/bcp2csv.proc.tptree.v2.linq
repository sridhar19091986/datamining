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
								 select new		{
				GPRS下行流量 = (int)(p.GPRS下行流量 / ((ttt.Max(e => e.GPRS下行流量) - ttt.Min(e => e.GPRS下行流量)) / stepbystep)) + 0.001
		};
		string strs = null;
		foreach(var p in m) {
				strs += p.GPRS下行流量 +",";
				//			strs +=p.Cell_name+","+p.Cell_no+","+p.lat+","+p.lon+","+p.基站代号+","+p.基站名;
				strs += "\r\n";
		}
		write_rp(strs,@"C:\fptree.txt");
		m.Dump();
}

// Define other methods and classes here
void write_rp(string line,string filename)
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