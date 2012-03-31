<Query Kind="Program">
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{
Dictionary<ParkingSpaceKey, int> garage = new Dictionary<ParkingSpaceKey, int>(
	new ParkingSpaceKey.EqualityComparer());  //这里申明 自定义字典
	
ParkingSpaceKey key = new ParkingSpaceKey(5, 777);

garage.Add(new ParkingSpaceKey(5, 666),1);

garage.Add(new ParkingSpaceKey(1, 667),1);
garage.Add(new ParkingSpaceKey(2, 668),1);

if (garage.ContainsKey(key)) {
	Console.WriteLine("包含主键，信令关联");
} else {
	Console.WriteLine("不包含，信令不关联");
}	
}

// Define other methods and classes here

public class ParkingSpaceKey {

	private int _floor, _parkingSpace;

	public ParkingSpaceKey(int floor, int parkingSpace) {
		_floor = floor;
		_parkingSpace = parkingSpace;
	}

//搞定这个实现即可  ？？？？？

//字典比较  ？？？？

//帧号  ？？？？？

//  用时间+imsi  或者 sccp 生成 guid  , 都可转换成 int ?

	public class EqualityComparer : IEqualityComparer<ParkingSpaceKey> {

		public bool Equals(ParkingSpaceKey x, ParkingSpaceKey y) {
		
			return x._floor == y._floor;
			//|| x._parkingSpace == y._parkingSpace;
		}

//do "fuzzy" compare
//IEqualityComparer  fuzzy match
		public int GetHashCode(ParkingSpaceKey x) {
			//return x._floor;
			//^ x._parkingSpace;
			
			 if (Object.ReferenceEquals(x, null)) return 0;      return 1;
		}

	}

}