<Query Kind="Program">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>CellLoadSharing</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>HtmlAgilityPack</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{

	/*
	t=[0,110,250,245];
	h=[47254,199713,14691,0];
	polar(pi*t/180,h,'ro')
	title('FAILDLTBFEST')

	t=[0,110,250,245];
	h=[8.95782857142857,11.4418571428571,4.18382857142857,1.83878571428571];
	polar(pi*t/180,h,'ro')
	title('PDCH复用度')

	t=[0,110,250,245];
	h=[60.5780142857143,69.4765571428571,0.0521571428571429,0.139571428571429];
	polar(pi*t/180,h,'ro')
	title('H话务比')

	t=[245,250,0,110];
	h=[1,2,8,11];
	polar(pi*t/180,h,'ro')


	theta=-pi.*30/180:0.0001:pi.*30/180;
	r3=abs(sin(8000*theta/5));
	t=[110,0,250,245];
	h=[199713,47254,14691,0];
	for i=1:4
	polar(theta+pi.*t(1,i)/180,h(1,i)*r3,'ro');
	hold on;
	end
	hold off;
	title('FAILDLTBFEST')


	theta=-pi.*30/180:0.0001:pi.*30/180;
	r3=abs(sin(8000*theta/5));
	t=[110,0,250,245];
	h=[11.4418571428571,8.95782857142857,4.18382857142857,1.83878571428571];
	for i=1:4
	polar(theta+pi.*t(1,i)/180,h(1,i)*r3,'ro');
	hold on;
	end
	hold off;
	title('PDCH复用度')


	theta=-pi.*0.01/180:0.000001:pi.*0.01/180;
	r3=abs(sin(8000*theta/5));
	t=[110,0,250,245];
	h=[1,1,1,1];
	for i=1:4
	polar(theta+pi.*t(1,i)/180,h(1,i)*r3,'ro');
	hold on;
	end
	hold off;
	title('各扇区方向角')

	**/

	//替换1
	var cellgprs2 = CELLGPRS2_0812s;
	//替换2
	var cellbase = 小区基础数据_0812s;

	var tbf = from p in cellgprs2
			  //where p.BSC == "SZ35B"
			  group p by p.小区名 into ttt
			  select new
	{
		Cell_name = ttt.Key,
		BSC = ttt.Select(e => e.BSC).FirstOrDefault(),
		下行TBF建立成功率 = ttt.Average(e => e.下行TBF建立成功率),
		FAILDLTBFEST = ttt.Average(e => e.FAILDLTBFEST),
		PDCH复用度 = ttt.Average(e => e.PDCH复用度),
		平均分配PDCH= ttt.Average(e=>e.平均分配PDCH),
		T可用信道 = ttt.Average(e => e.T可用信道),
		T话务量 = ttt.Average(e => e.T话务量),
		H话务比 = ttt.Average(e => e.H话务比),
		GPRS下行激活信道 = ttt.Average(e => e.GPRS下行激活信道),
		EDGE下行激活信道 = ttt.Average(e => e.EDGE下行激活信道),
		EDGE终端使用EDGE比例 = ttt.Average(e => e.EDGE终端使用EDGE比例),
		T话务量tch_20h = erlangbinv(0.02, 0.9 * ttt.Average(e => e.T话务量)),
	};

	//var faildltbf=tbf.OrderByDescending(e=>e.FAILDLTBFEST).Take(10).Select(e=>e.Cell_name).ToList();
	//   var cdd_nrel=from p in Cdd_Nrel_Get()
	//                join q in 小区基础数据0803s on p.Cell_name equals q.小区名
	//				select new {p,q};

	var celbase = cellbase.ToLookup(e => e.小区名);
	var rel = from p in  Cdd_Nrel_Get()
			  join q in tbf on p.N_cell_name equals q.Cell_name
			  //join t in 小区基础数据0803s  on p.N_cell_name equals t.小区名
			  select new
	{
		BSC = q.BSC,
		p.Cell_name,
		p.N_cell_name,
		方向角 = celbase[p.N_cell_name].Select(e => e.方向角).FirstOrDefault(),
		下倾角 = celbase[p.N_cell_name].Select(e => e.下倾角).FirstOrDefault(),
		海拔高度 = celbase[p.N_cell_name].Select(e => e.海拔高度).FirstOrDefault(),
		下行TBF建立成功率 = ConvNullDouble(q.下行TBF建立成功率),
		FAILDLTBFEST = ConvNullDouble(q.FAILDLTBFEST),	
		T空闲信道 = ConvNullDouble(q.T可用信道 -q.T话务量tch_20h - q.平均分配PDCH),
		PDCH复用度 = ConvNullDouble(q.PDCH复用度),
		H话务比 = ConvNullDouble(q.H话务比),
		EDGE终端使用EDGE比例 = ConvNullDouble(q.EDGE终端使用EDGE比例),
		T可用信道 = ConvNullDouble(q.T可用信道),
		T话务量 = ConvNullDouble(q.T话务量),
		百分20H话务比T信道 = ConvNullDouble(q.T话务量tch_20h),
		平均分配PDCH=ConvNullDouble(q.平均分配PDCH),
		GPRS下行激活信道 = ConvNullDouble(q.GPRS下行激活信道),
		EDGE下行激活信道 = ConvNullDouble(q.EDGE下行激活信道),
	};


	//var rellist = rel.ToList();
	//rellist.Where(e=>faildltbf.Contains(e.Cell_name)).OrderBy(e => e.Cell_name).ThenBy(e => e.FAILDLTBFEST).Dump();


	var variance = from p in rel
				   group p by p.Cell_name into ttt
				   select new
	{
		BSC = ttt.Select(e => e.BSC).FirstOrDefault(),
		Cell_name = ttt.Key,

		FAILDLTBFEST = ttt.Where(e => e.N_cell_name == ttt.Key).Sum(e => e.FAILDLTBFEST),

		Balance_T空闲信道 = ttt.Where(e => e.Cell_name == ttt.Key).Sum(e => e.T空闲信道),

		//Variance_FAILDLTBFEST = Variance(ttt.Select(e => e.FAILDLTBFEST/10000)),
		Variance_T空闲信道 = Variance(ttt.Select(e => e.T空闲信道)),
		Variance_PDCH复用度 = Variance(ttt.Select(e => e.PDCH复用度)),
		Variance_H话务比 = Variance(ttt.Select(e => e.H话务比 / 100)),
		Variance_EDGE终端使用EDGE比例 = Variance(ttt.Select(e => e.EDGE终端使用EDGE比例 / 100)),

		Variance_detail = ttt.Where(e => e.Cell_name == ttt.Key).OrderByDescending(e => e.FAILDLTBFEST)
	};

	variance.ToList().OrderByDescending(e => e.FAILDLTBFEST).Take(10).Dump();



}

// Define other methods and classes here
public  double Variance(IEnumerable<double> ncell)
{
	List<double> source = ncell.ToList();
	double avg = source.Average();
	double d = source.Aggregate(0.0, (total, next) => total += Math.Pow(next - avg, 2));
	double vd = d ;
	return Math.Sqrt(vd) / source.Count();
}

static double ConvNullDouble( double? ss)
{
	if(ss == null) return 0;
	return (double)ss;
}

static double ConvNullDouble( int? ss)
{
	if(ss == null) return 0;
	return (double)ss;
}


public List<CellName> Cdd_Nrel_Get()
{
	//替换3
	var cdd_nrel = Cdd_Nrel_0812s;

	List<CellName> nrel = new List<CellName>();
	var nrelation = from p in cdd_nrel
					select new {p.Cell_name, p.N_cell_name};
	var nnative = cdd_nrel.Select(e => e.Cell_name).Distinct();

	foreach(var n in nrelation)
	{
		CellName cn = new CellName();
		cn.Cell_name = n.Cell_name;
		cn.N_cell_name = n.N_cell_name;
		if(cn.Cell_name.IndexOf(cn.N_cell_name.Substring(0, cn.N_cell_name.Length - 1)) != -1)
			nrel.Add(cn);
	}
	foreach(string n in nnative)
	{
		CellName cn = new CellName();
		cn.Cell_name = n;
		cn.N_cell_name = n;
		nrel.Add(cn);
	}
	return nrel;
}

public class CellName
{
	public string Cell_name { get; set; }
	public string N_cell_name { get; set; }
}
private int erlangbinv(double p, double? erlang)
{
	double rho = (double)erlang;
	double B = 1;
	int n = 1;
	while (1 == 1)
	{
		B = ((rho * B) / n) / (1 + rho * B / n);
		if (B <= p)
			return n;
		n = n + 1;
	}
	return n;
}