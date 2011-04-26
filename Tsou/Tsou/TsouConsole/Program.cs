using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace TsouConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            File.Delete(writelog.strTsou);
            File.Delete(writelog.strPath1);

            writelog.write_rp("OK");

            StreamWriter sw = File.AppendText(writelog.strTsou);

            DataClasses1DataContext db = new DataClasses1DataContext();
            GPRSossCollection oss = new GPRSossCollection();

            Console.WriteLine("Collection,ok");
            //Dictionary<gprs_tousu, int?> d = new Dictionary<gprs_tousu, int?>();
            List<Tuple<gprs_tousu, int?>> d = new List<Tuple<gprs_tousu, int?>>();

            sw.Write("请求编号"); sw.Write("-");
            sw.Write("经度"); sw.Write("-");
            sw.Write("totalAlarms"); sw.Write("-");
            sw.WriteLine("totalCells");

            int i = 0;
            int? totalAlarms = 0;
            string totalCells = null;
            foreach (var m in db.gprs_tousu)
            {
                GPRStuple g = new GPRStuple(m, oss.oss);
                totalAlarms = g.Get_RPPLOAD_Maybe();
                totalCells = g.Get_Min_Distance_Cell();
                Tuple<gprs_tousu, int?> a = new Tuple<gprs_tousu, int?>(m, totalAlarms);
                d.Add(a);

                i++;
                Console.Write(i); Console.Write("---");
                Console.WriteLine(totalAlarms);
                sw.Write(m.请求编号); sw.Write("-");
                sw.Write(m.经度); sw.Write("-");
                sw.Write(totalAlarms); sw.Write("-");
                sw.WriteLine(totalCells);
                sw.Flush();

            }


            sw.Close();


            StreamWriter sw1 = File.AppendText(writelog.strPath1);

            var k = d.Where(e => e.Item2 == 0).Count();
            var l = d.Where(e => e.Item2 > 0).Count();
            sw1.WriteLine("RP告警===0" + k.ToString() + "\n");
            sw1.WriteLine("RP告警=/=0" + l.ToString() + "\n");

            var qqq = from q in d.Select(e => e.Item1)
                      group q by q.纬度 into ttt
                      select new
                      {
                          ttt.Key,
                          summ = ttt.Count()
                      };

            foreach (var q in qqq)
            {
                sw1.Write("---经纬度类别---");
                sw1.Write(q.Key);
                sw1.Write("---经纬度数量---");
                sw1.WriteLine(q.summ);
            }

            Console.WriteLine("Complete");
        }
    }
}
