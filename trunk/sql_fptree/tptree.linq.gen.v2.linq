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
	string txtlinq = null;
	double step = 0;
	string txtfile = null;
	Type t = typeof(Pdchredian1000);
	foreach (var item in t.GetMembers(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance))
	{
		if (item.MemberType.ToString() == "Field")
		{
			FieldInfo field = (FieldInfo)item;
			string fn = field.Name;
			if(fn != "时间" && fn != "小区名" && fn != "小区号" && fn != "BSC")
			{
				txtfile += "strs += p." + fn + "+" + "\"" + "," + "\"" + ";\r\n";
				step += 0.001;
				txtlinq += fn + " = (int)(p." + fn + "/ ((ttt.Max(e => e." + fn + ") - ttt.Min(e => e." + fn + ")) / stepbystep)) + " + step.ToString() + ",";
				//Console.WriteLine(fn);
				txtlinq += "\r\n";
			}
		}
	}
	txtfile += "\r\n";
	Console.WriteLine(a + "\r\n" + txtlinq + "\r\n" + b + "\r\n" + txtfile + "\r\n" + c + "\r\n");
}

//注意修改Pdchredian1000s

string a = @"
		   void Main()
		   {
		   var ttt = from p in Pdchredian1000s
		   select p;
		   var m = from p in ttt
		   let stepbystep = 0.1 * ttt.Count()
		   select new		{

		   ";

string b = @"
		   };
		   string strs = null;
		   foreach(var p in m) {
		   ";


static string d = "@" + "\"" + @"C:\fptree.txt" + "\"";

string c = @"
		   }
		   write_rp(strs," + d + @");
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
		   ";
