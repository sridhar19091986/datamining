<Query Kind="Program">
  <Connection>
    <ID>76932288-1882-4022-a8fb-c01693c0810c</ID>
    <Persist>true</Persist>
    <Driver Assembly="IQDriver" PublicKeyToken="5b59726538a49684">IQDriver.IQDriver</Driver>
    <Provider>Devart.Data.MySql</Provider>
    <CustomCxString>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAACTNuYXkfk66zdd04KiO8wGAAAAAASAAACgAAAAEAAAAIOK7HFTUbC4NvHwmY54AThAAAAAYKS1JdvPz5iWCN+bVClSqdEdGOTZYMPe388yxJbFLsrQLNQremir6pv+EuhDT42GyAqHjKDtruU3cS1x7UlBXBQAAAA9C6jhKoipfqKkln91vqhRHssrBA==</CustomCxString>
    <Server>localhost</Server>
    <UserName>root</UserName>
    <Password>AQAAANCMnd8BFdERjHoAwE/Cl+sBAAAAZLcVh8Gjs02sCq6PBfQDZAAAAAACAAAAAAADZgAAqAAAABAAAADkrYJ5ii5dOlB54Q4g4VNRAAAAAASAAACgAAAAEAAAALRZpGA5TrMbTQf5Q7s/6/IIAAAAo4mX5DXQQiEUAAAArcrXZk87oLcEm3N3crj52U5a5Ak=</Password>
    <EncryptCustomCxString>true</EncryptCustomCxString>
    <Database>test</Database>
    <DriverData>
      <StripUnderscores>false</StripUnderscores>
      <QuietenAllCaps>false</QuietenAllCaps>
      <Port>3306</Port>
    </DriverData>
  </Connection>
  <Reference>G:\htmlconvertsql\SqlCompact.v.2011.05.21\Soccer Score Forecast\Soccer Score Forecast\bin\Release\HtmlAgilityPack.dll</Reference>
  <Reference>D:\Program Files\Microsoft SQL Server\100\SDK\Assemblies\Microsoft.SqlServer.Types.dll</Reference>
  <Namespace>HtmlAgilityPack</Namespace>
  <Namespace>Microsoft.SqlServer.Types</Namespace>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

void Main()
{

	List<CauseDescribe>  CauseDescribeList= new List<CauseDescribe>();
	foreach(var m in CauseDescribes)
	CauseDescribeList.Add(m);

	//lcd.Take(1).Dump();
	var tables=Abcdefgs;

	double total = tables.Where(e => e.Message_Type == 37).Count();

	var cause = from p in tables
				where p.Message_Type == 37
				group p by new {p.Message_Type, p.Cause_type, p.Cause_Value, p.Protocol} into ttt

				select new

	{
		Message_Type_Code = ttt.Key.Message_Type, //{ 0x25,	"Disconnect" },
		Message_Type = "Disconnect", //{ 0x25,	"Disconnect" },
		Cause_type = ttt.Key.Cause_type,
		Cause_Value = (int)ttt.Key.Cause_Value,
		//Cause=lcd.Where(e=>e.causecode==ttt.Key.Cause_Value).FirstOrDefault().causecode,
		Protocol = ttt.Key.Protocol,
		valueCount = ttt.Count(),
		valueRate = ttt.Count() / total,


	};

	//cause.ToList().OrderBy(e=>e.valueCount).Dump();

	var fromabis = cause.ToList();


	var causenew = from p in fromabis
				   join q in CauseDescribeList   on   p.Cause_Value equals q.Cause_Code
				   //into ttt
				   select new 
				   {
				   q.Cause_Describe,
				   p.Message_Type_Code ,
				   p.Message_Type,
				   p.Cause_type, 
				   p.Cause_Value, 
				   p.Protocol,
				   p.valueCount, 
				   p.valueRate, 
				   q.Cause_Code,
				   
				   };

	causenew.OrderByDescending(e => e.valueCount).Dump();
}

// Define other methods and classes here
public class CauseDescribe
{
	public int Cause_Code;
	public string  Cause_Describe;
	public CauseDescribe(int code, string cause)
	{
		this.Cause_Code = code;
		this.Cause_Describe = cause;
	}
}
public	CauseDescribe[] CauseDescribes = new CauseDescribe[]
{
	new CauseDescribe(	1	,	"Unassigned (unallocated) number"	),
	new CauseDescribe(		3	,	"No route to destination"	),
	new CauseDescribe(		6	,	"Channel unacceptable"	),
	new CauseDescribe(		8	,	"Operator determined barring"	),
	new CauseDescribe(		16	,	"Normal call clearing"	),
	new CauseDescribe(		17	,	"User busy"	),
	new CauseDescribe(		18	,	"No user responding"	),
	new CauseDescribe(		19	,	"User alerting, no answer"	),
	new CauseDescribe(		21	,	"Call rejected"	),
	new CauseDescribe(		22	,	"Number changed"	),
	new CauseDescribe(		25	,	"Pre-emption"	),
	new CauseDescribe(		26	,	"Non selected user clearing"	),
	new CauseDescribe(		27	,	"Destination out of order"	),
	new CauseDescribe(		28	,	"Invalid number format (incomplete number)"	),
	new CauseDescribe(		29	,	"Facility rejected"	),
	new CauseDescribe(		30	,	"Response to STATUS ENQUIRY"	),
	new CauseDescribe(		31	,	"Normal, unspecified"	),
	new CauseDescribe(		34	,	"No circuit/channel available"	),
	new CauseDescribe(		38	,	"Network out of order"	),
	new CauseDescribe(		41	,	"Temporary failure"	),
	new CauseDescribe(		42	,	"Switching equipment congestion"	),
	new CauseDescribe(		43	,	"Access information discarded"	),
	new CauseDescribe(		44	,	"requested circuit/channel not available"	),
	new CauseDescribe(		47	,	"Resources unavailable, unspecified"	),
	new CauseDescribe(		49	,	"Quality of service unavailable"	),
	new CauseDescribe(		50	,	"Requested facility not subscribed"	),
	new CauseDescribe(		55	,	"Incoming calls barred within the CUG"	),
	new CauseDescribe(		57	,	"Bearer capability not authorized"	),
	new CauseDescribe(		58	,	"Bearer capability not presently available"	),
	new CauseDescribe(		63	,	"Service or option not available, unspecified"	),
	new CauseDescribe(		65	,	"Bearer service not implemented"	),
	new CauseDescribe(		68	,	"ACM equal to or greater than ACMmax"	),
	new CauseDescribe(		69	,	"Requested facility not implemented"	),
	new CauseDescribe(		70	,	"Only restricted digital information bearer capability is available"	),
	new CauseDescribe(		79	,	"Service or option not implemented, unspecified"	),
	new CauseDescribe(		81	,	"Invalid transaction identifier value"	),
	new CauseDescribe(		87	,	"User not member of CUG"	),
	new CauseDescribe(		88	,	"Incompatible destination"	),
	new CauseDescribe(		91	,	"Invalid transit network selection"	),
	new CauseDescribe(		95	,	"Semantically incorrect message"	),
	new CauseDescribe(		96	,	"Invalid mandatory information"	),
	new CauseDescribe(		97	,	"Message type non-existent or not implemented"	),
	new CauseDescribe(		98	,	"Message type not compatible with protocol state"	),
	new CauseDescribe(		99	,	"Information element non-existent or not implemented"	),
	new CauseDescribe(		100	,	"Conditional IE error"	),
	new CauseDescribe(		101	,	"Message not compatible with protocol state"	),
	new CauseDescribe(		102	,	"Recovery on timer expiry"	),
	new CauseDescribe(		111	,	"Protocol error, unspecified"	),
	new CauseDescribe(		127	,	"Interworking, unspecified"	),
	new CauseDescribe(	 0,    "" ),
};