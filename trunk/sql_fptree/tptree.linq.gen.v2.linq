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
	string txtlinqfile=null;
	string txtlinq = null;
	double step = 0;
	string txtfile = null;
	string txtheader=null;
	Type t = typeof(PS总流量10000);//此处替换//注意修改Pdchredian1000s
	foreach (var item in t.GetMembers(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance))
	{
		if (item.MemberType.ToString() == "Field")
		{
			FieldInfo field = (FieldInfo)item;
			string fn = field.Name;
			if(fn != "时间" && fn != "小区名" && fn != "小区号" && fn != "BSC")
			{
			    txtheader += fn+",";
				txtfile += "strs += p." + fn + "+" + "\"" + "," + "\"" + ";\r\n";
				step += 0.001;
				txtlinq += fn + " = (int)(p." + fn + "/ ((ttt.Max(e => e." + fn + ") - ttt.Min(e => e." + fn + ")) / stepbystep)) + " 
				+ step.ToString() + ",";
				txtlinq += "\r\n";
			}
		}
	}
	txtfile += "\r\n";
	txtheader=  "\""+txtheader+"\"";
	txtlinqfile=a + "\r\n" + txtlinq + "\r\n" +  b  + txtheader+ e +"\r\n" + txtfile + "\r\n" + c + "\r\n";
	txtlinqfile=txtlinqfile.Replace("Pdchredian1000","PS总流量10000");//此处替换//注意修改Pdchredian1000s
	Console.WriteLine(txtlinqfile);
	
	FileStream fs = new FileStream ( outfile , FileMode.Create, FileAccess.Write ) ;
	StreamWriter m_streamWriter = new StreamWriter ( fs ,Encoding.Unicode) ;
	m_streamWriter.Flush ( ) ;
	m_streamWriter.WriteLine( txtlinqfile ) ;
	m_streamWriter.Flush ( ) ;
	m_streamWriter.Close ( ) ;
}
string a = ReadPublicKey(@"G:\datamining\sql_fptree\csharpScript\a.txt");
string b =  ReadPublicKey(@"G:\datamining\sql_fptree\csharpScript\b.txt");
static string d = "@" + "\"" + @"C:\fptree.txt" + "\"";
string c = ReadPublicKey(@"G:\datamining\sql_fptree\csharpScript\c.txt");  
string e = ReadPublicKey(@"G:\datamining\sql_fptree\csharpScript\e.txt"); 
string outfile=@"G:\datamining\sql_fptree\test.tptree.linq";
static string ReadPublicKey(string path)
{
	StreamReader reader = new StreamReader(path,Encoding.Default);
	string publickey = reader.ReadToEnd();
	reader.Close();
	return publickey;
}