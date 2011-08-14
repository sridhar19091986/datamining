<Query Kind="Program">
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
	HtmlAgilityPack.HtmlDocument doc = new HtmlAgilityPack.HtmlDocument();
	HtmlNode.ElementsFlags.Remove("option");
	string path=@"G:\datamining\sql_fptree\prediction.txt";
    string s = System.IO.File.ReadAllText( path,Encoding.Default );
    doc.LoadHtml(s);
	//doc.Dump();
	
foreach (HtmlNode node in doc.DocumentNode.SelectNodes("//select[@name='menu']//option"))
{
	//node.Attributes["value"].Value.Dump();
	 node.InnerText.Dump();
	 //webbrowser提交select option问题不知道怎么提交 select列表项，模拟鼠标onchange事件,选择哪一项
	 //node.WriteContentTo(node.InnerText);
//	 webbrowser.oleobject.document.all.item('.bmon',0).selectedindex:=3;//選擇第三項 
//	 WebBrowser1.Document.getElementsByTagName("option")(2).value

}
}

// Define other methods and classes here