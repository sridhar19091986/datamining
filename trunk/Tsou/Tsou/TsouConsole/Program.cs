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
                totalAlarms=g.Get_RPPLOAD_Maybe();
                Tuple<gprs_tousu, int?> a = new Tuple<gprs_tousu, int?>(m,totalAlarms );
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
            var o = d.Where(e => e.Item1.请求编号 != null).Count();
            var n = d.Where(e => e.Item1.纬度 !=null ).Count();
            var q = d.Where(e => e.Item1.纬度 ==null ).Count();

            string lll = null;
            lll += k.ToString() + "\n";
            lll += l.ToString() + "\n";
            lll += o.ToString() + "\n";
            lll += n.ToString() + "\n";
            lll += q.ToString() + "\n";

            writelog.write_rp(lll);
            Console.WriteLine("Complete");
        }
    }
}
