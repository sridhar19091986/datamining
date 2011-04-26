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
            writelog.write_rp("OK");

            StreamWriter sw = File.AppendText(writelog.strTsou);

            DataClasses1DataContext db = new DataClasses1DataContext();
            GPRSossCollection oss = new GPRSossCollection();

            Console.WriteLine("Collection,ok");
            //Dictionary<gprs_tousu, int?> d = new Dictionary<gprs_tousu, int?>();
            List<Tuple<gprs_tousu, int?>> d = new List<Tuple<gprs_tousu, int?>>();

            int i = 0;
            int? totalAlarms = 0;
            foreach (var m in db.gprs_tousu)
            {
                GPRStuple g = new GPRStuple(m, oss.oss);
                totalAlarms = g.Get_RPPLOAD_Maybe();
                Tuple<gprs_tousu, int?> a = new Tuple<gprs_tousu, int?>(m, totalAlarms);
                d.Add(a);

                i++;
                Console.Write(i); Console.Write("---");
                Console.WriteLine(totalAlarms);
                sw.Write(m.请求编号); sw.Write("---");
                sw.WriteLine(totalAlarms);
                sw.Flush();

            }


            sw.Close();

            var k = d.Where(e => e.Item2 == 0).Count();
            var l = d.Where(e => e.Item2 > 0).Count();

            var qqq = from q in d.Select(e => e.Item1)
                      group q by q.纬度 into ttt
                      select new
                      {
                          ttt.Key,
                          summ = ttt.Count()
                      };

            string lll = null;
            lll += "RP告警===0" + k.ToString() + "\n";
            lll += "RP告警=/=0" + l.ToString() + "\n";

            foreach (var q in qqq)
            {
                lll += "3月份数据业务投诉信息---经纬度类别---" + q.Key==null?"null":q.Key.ToString() + "\n";
                lll += "3月份数据业务投诉信息---经纬度数量---" + q.summ.ToString() + "\n";
            }

            writelog.write_rp(lll);
            Console.WriteLine("Complete");
        }
    }
}
