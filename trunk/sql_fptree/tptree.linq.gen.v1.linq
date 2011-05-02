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
	string fn = null;
	double step = 0;
	string txtfile=null;
	Type t = typeof(Pdchredian1000);
	foreach (var item in t.GetMembers(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance))
	{
		if (item.MemberType.ToString() == "Field")
		{
			FieldInfo field = (FieldInfo)item;
			fn = field.Name;
			txtfile +="p."+fn+",\r\n";
			step += 0.001;
			fn = fn + " = (int)(p." + fn + "/ ((ttt.Max(e => e." + fn + ") - ttt.Min(e => e." + fn + ")) / stepbystep)) + " + step.ToString();
			Console.WriteLine(fn);	
		}
	}
	Console.WriteLine(txtfile);
}
