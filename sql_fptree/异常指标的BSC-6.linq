<Query Kind="Statements">
  <Connection>
    <ID>4bf09d82-2274-4382-a6e9-bea773c75ba2</ID>
    <Server>localhost</Server>
    <Persist>true</Persist>
    <Database>PFCredianhedian</Database>
    <ShowServer>true</ShowServer>
  </Connection>
  <IncludePredicateBuilder>true</IncludePredicateBuilder>
</Query>

//C#声明一个100大小的数组 随机生成1-100之间不重复的数
	 
	 int[] intArray = new int[100];
         ArrayList newArray = new ArrayList();
             Random rnd = new Random();
             while (newArray.Count < 100)
             {
                 int tempNumber = rnd.Next(1, 101);
                 if (!newArray.Contains(tempNumber))
                    newArray.Add(tempNumber);
            }
            for (int i = 0; i < 100; i++)
            {
                intArray[i] = (int)newArray[i];
                //Console.Write("{0},", intArray[i]);
            }


var tbf=from p in intArray
		let key=p
		select new {
		下行TBF建立成功率=p,
		SZ47G_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ47G").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ35B_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ35B").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ35A_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ35A").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ15B_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ15B").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ06A_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ06A").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ56D_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ56D").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ04B_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ04B").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ21B_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ21B").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ46H_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ46H").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		SZ41B_平均分配PDCH总数=BSCGPRS2_20110626s.Where(e=>e.BSC=="SZ41B").Where(e=>(int)e.下行TBF建立成功率==key).Average(e=>e.平均分配PDCH总数),
		};
		
	tbf.OrderByDescending(e=>e.下行TBF建立成功率).Dump();