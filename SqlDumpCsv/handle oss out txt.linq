<Query Kind="Program">
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
    bool next_line_is_rp=false;
	string writeLine=null;
	FileStream fs = new FileStream ( @"C:\20110421RPP2.txt" , FileMode.Open , FileAccess.Read ) ;
	StreamReader m_streamReader = new StreamReader ( fs ) ; 
	//使用StreamReader类来读取文件
	m_streamReader.BaseStream.Seek ( 0 , SeekOrigin.Begin ) ;
	// 从数据流中读取每一行，直到文件的最后一行，并在richTextBox1中显示出内容
	string strLine = m_streamReader.ReadLine ( ) ;
	while ( strLine != null )
	{
	strLine = m_streamReader.ReadLine ( ) ;
    if(next_line_is_rp==true)
	{
	writeLine +=strLine+"\n";
	
	next_line_is_rp=false;
	}
	if(strLine !=null)
	{
	if(strLine.IndexOf("CELL     NOPDCH  RP") != -1)
	  next_line_is_rp=true;
	  }
	}
	write_rp(writeLine );
	//关闭此StreamReader对象
	m_streamReader.Close ( ) ; 
	
}

// Define other methods and classes here
void write_rp(string line)
{
	//创建一个文件流，用以写入或者创建一个StreamWriter
FileStream fs = new FileStream ( @"C:\20110421RPP2.txt.txt" , FileMode.OpenOrCreate , FileAccess.Write ) ;
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