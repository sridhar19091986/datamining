using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
//using System.Windows.Forms;

namespace TsouConsole
{
    class Class1
    {
    }
    // Define other methods and classes here
    public class GPRStuple
    {
        //public Gprs_tousu tousu=new Gprs_tousu();
        //public List<GPRSoss> oss=new List<GPRSoss>();
        private double distance;
        private double lat1;
        private double lon1;
        private double lat2;
        private double lon2;
        private double minv;
        //private Dictionary<GPRSoss, double> tousu_oss_distance = new Dictionary<GPRSoss, double>();
        private List<Tuple<GPRSoss, double>> tousu_oss_distance = new List<Tuple<GPRSoss, double>>();
        public GPRStuple(gprs_tousu tousu, List<GPRSoss> oss)
        {
            foreach (var o in oss)
            {
                lat1 = L2Lat(o.Latitude);
                lat2 = T2Lon(tousu.纬度);
                lon1 = L2Lon(o.Longitude);
                lon2 = T2Lat(tousu.经度);
                //Console.WriteLine(lat1);
                //Console.WriteLine(lat2);
                //Console.WriteLine(lon1);
                //Console.WriteLine(lon2);
                if (lat1 < 10 || lat2 < 10 || lon1 < 10 || lon2 < 10)
                    distance = 20;
                else
                    distance = (lat1 - lat2) * (lat1 - lat2) + (lon1 - lon2) * (lon1 - lon2);
                //Console.WriteLine(distance);
                //Console.WriteLine("----");
                Tuple<GPRSoss, double> a = new Tuple<GPRSoss, double>(o, distance);
                tousu_oss_distance.Add(a);
                //tousu_oss_distance.Add(o, distance);
            }
            minv = Min_Distance();

            Console.WriteLine(minv);
        }

        private double Min_Distance()
        {
            return tousu_oss_distance.Min(e => e.Item2);
            //return tousu_oss_distance.Min(e => e.Value);
        }
        public int? Get_RPPLOAD_Maybe()
        {
            if (minv == 0) return 0;
            if (minv > 10) return 0;
            return tousu_oss_distance.Where(e => e.Item2 == minv).Select(e => e.Item1).Sum(e => e.RPPLOAD);
            //return tousu_oss_distance.Where(e => e.Value == minv).Select(e => e.Key).Sum(e => e.RPPLOAD);
        }
        public string Get_Min_Distance_Cell()
        {
            if (minv == 0) return "0";
            if (minv > 10) return "0";
            string minc = tousu_oss_distance.Where(e => e.Item2 == minv).Select(e => e.Item1.Cell_no)
                .Aggregate((name, next) => name + "," + next);
            //string minc = tousu_oss_distance.Where(e => e.Value == minv).Select(e => e.Key.Cell_no)
            //      .Aggregate((name, next) => name + "," + next);
            return minc;
        }
        private double L2Lat(string input)
        {
            string ttt;
            if (input.Length < 2) return 0;
            if (input.IndexOf("tude") != -1 || input.IndexOf("#") != -1) return 0;
            string l = input.Replace("N", "").Replace("-", "").Replace(".", "");
            string a = l.Substring(0, 2);
            string b = l.Substring(2, l.Length - 2);
            ttt = a + "." + b;
            return double.Parse(ttt);
        }
        private double L2Lon(string input)
        {
            string nnn;
            if (input.Length < 2) return 0;
            if (input.IndexOf("tude") != -1 || input.IndexOf("#") != -1) return 0;
            string l = input.Replace("E", "").Replace("-", "").Replace(".", "");
            string a = l.Substring(0, 3);
            string b = l.Substring(3, l.Length - 3);
            nnn = a + "." + b;
            return double.Parse(nnn);
        }
        private double T2Lat(string input)
        {
            if (input == null) return 0;
            if (input.IndexOf("无") != -1) return 0;
            return double.Parse(input);
        }
        private double T2Lon(string input)
        {
            if (input == null) return 0;
            if (input.IndexOf("无") != -1) return 0;
            return double.Parse(input);
        }
    }

    public class GPRSoss
    {
        public string BSC { get; set; }
        public int? RPPID { get; set; }
        public int? RPPLOAD { get; set; }
        public string Cell_name { get; set; }
        public string Cell_no { get; set; }
        public string Latitude { get; set; }
        public string Longitude { get; set; }
        public string 基站代号 { get; set; }
        public string 基站名 { get; set; }
        public string CELL { get; set; }
        public string NOPDCH { get; set; }
        public string RP { get; set; }
    }

    public class GPRSossCollection
    {
        private DataClasses1DataContext db = new DataClasses1DataContext();
        public List<GPRSoss> oss = new List<GPRSoss>();
        private List<gprs_ci> ci = new List<gprs_ci>();
        private List<gprs_cell_rp> rp = new List<gprs_cell_rp>();
        private List<gprs_rpp> rpp = new List<gprs_rpp>();

        public GPRSossCollection()
        {
            ci = db.gprs_ci.ToList();
            Console.WriteLine("1..ok");
            rp = db.gprs_cell_rp.ToList();
            Console.WriteLine("2..ok");
            rpp = db.gprs_rpp.ToList();
            Console.WriteLine("3..ok");
            oss = get().ToList();
        }
        public IEnumerable<GPRSoss> get()
        {
            var m = from p in ci//Gprs_cell_gis
                    join t in rp
                    on p.cell_no equals t.CELL
                    select new { p, t };

            var n = from q in rpp
                    group q by new { q.BSC, q.RPPID } into ttt
                    select new { BSC = ttt.Key.BSC, RPPID = ttt.Key.RPPID, RPPLOAD = ttt.Sum(e => e.RPPLOAD) };

            var k = from q in n
                    join t in m
                    on new { BSC = q.BSC, RPPID = q.RPPID.ToString() } equals new { BSC = t.p.bsc, RPPID = t.t.RP }
                    select new
                    {
                        q.BSC,
                        q.RPPID,
                        q.RPPLOAD,
                        t.p.bsc,
                        t.p.cell_name,
                        t.p.cell_no,
                        t.p.latitude,
                        t.p.longitude,
                        t.p.基站代号,
                        t.p.基站名,
                        t.t.CELL,
                        t.t.NOPDCH,
                        t.t.RP
                    };

            var kk = k.Where(e => e.bsc == e.BSC).Where(e => e.BSC != null).
            Where(e => e.RPPID.ToString() == e.RP).Where(e => e.RP != null);

            foreach (var l in kk)
            {
                GPRSoss oss = new GPRSoss();
                oss.BSC = l.BSC;
                oss.CELL = l.CELL;
                oss.Cell_name = l.cell_name;
                oss.Cell_no = l.cell_no;
                oss.Latitude = l.latitude;
                oss.Longitude = l.longitude;
                oss.NOPDCH = l.NOPDCH;
                oss.RP = l.RP;
                oss.RPPID = l.RPPID;
                oss.RPPLOAD = l.RPPLOAD;
                oss.基站代号 = l.基站代号;
                oss.基站名 = l.基站名;
                yield return oss;
            }

        }
    }

    public static class writelog
    {
        public static void write_rp(string line)
        {
            string strPath = Directory.GetCurrentDirectory() + @"\Gprs_cissss.txt";
            FileStream fs = new FileStream(strPath, FileMode.Create, FileAccess.Write);
            StreamWriter m_streamWriter = new StreamWriter(fs);
            m_streamWriter.Flush();
            m_streamWriter.BaseStream.Seek(0, SeekOrigin.Begin);
            m_streamWriter.Write(line);
            m_streamWriter.Flush();
            m_streamWriter.Close();
        }
        public static string strPath1 = Directory.GetCurrentDirectory() + @"\Gprs_cissss1.txt";
        public static string strTsou = Directory.GetCurrentDirectory() + @"\tousu.txt";
    }
}
