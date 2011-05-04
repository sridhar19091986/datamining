<Query Kind="Program">
  <Connection>
    <ID>337b631f-296e-4a3b-aa36-09f23a9a9a38</ID>
    <Server>.\SQLEXPRESS</Server>
    <Database>RedianHedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
    var cis=Gprs_cis;
	var ci=from p in  cis
	       select new { p.Bsc,p.Cell_name,p.Cell_no,p.基站代号,p.基站名,lat=L2Lat(p.Latitude),lon=L2Lon(p.Longitude)};
	//ci.Dump();
	string strs=null;
	foreach(var p in ci)
	{
	    strs +=p.Bsc+","+p.Cell_name+","+p.Cell_no+","+p.lat+","+p.lon+","+p.基站代号+","+p.基站名;
		strs +="\n";
	}
	
	write_rp(strs);
		
//	var ts=from p in Gprs_tousus
//	       from q in  ci
//	       where p.经度==q.lat && p.纬度==q.lon
//		   select new 
//		   {
//		       p.请求编号,q.Cell_name
//		   };
//	var a=ts.ToList();
//    a.Dump();
}

// Define other methods and classes here
string  ttt;
string L2Lat (string input)
{
    if(input.Length<2) return "1";
	if(input.IndexOf("tude") !=-1 || input.IndexOf("#") !=-1) return "1";
	string l=input.Replace("N","").Replace("-","").Replace(".","");
	string a=l.Substring(0,2);
	string b=l.Substring(2,l.Length-2);
	ttt=a+"."+b;
	return ttt;
}
// Define other methods and classes here
string  nnn;
string L2Lon (string input)
{
	if(input.Length<2) return "1";
	if(input.IndexOf("tude") !=-1 || input.IndexOf("#") !=-1) return "1";
	string l=input.Replace("E","").Replace("-","").Replace(".","");
	string a=l.Substring(0,3);
	string b=l.Substring(3,l.Length-3);
	nnn=a+"."+b;
	return nnn;
}

// Define other methods and classes here
void write_rp(string line)
{
	//创建一个文件流，用以写入或者创建一个StreamWriter
FileStream fs = new FileStream ( @"C:\Gprs_cissss.txt" , FileMode.Create, FileAccess.Write ) ;
StreamWriter m_streamWriter = new StreamWriter ( fs ) ;
m_streamWriter.Flush ( ) ;
// 使用StreamWriter来往文件中写入内容
m_streamWriter.BaseStream.Seek ( 0 , SeekOrigin.Begin ) ;
// 把richTextBox1中的内容写入文件
m_streamWriter.Write ( line ) ;
//关闭此文件
m_streamWriter.Flush ( ) ;
m_streamWriter.Close ( ) ; 

}