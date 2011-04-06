using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace FPTree
{
    /// <summary>
    /// FPTree获取频繁模式
    /// </summary>
    public partial class FormFPTree : Form
    {
        /// <summary>
        /// 最小支持度计数
        /// </summary>
        static int MinSupCount = 1000;
        /// <summary>
        /// 最小支持度
        /// </summary>
        static int MinSupPercent = 8;
        /// <summary>
        /// 事务数据库中的所有事务总数
        /// </summary>
        static int TotalItems = 0;
        /// <summary>
        /// 测试集的文件名
        /// </summary>
        static string _testSet = "";
        


        #region 测试数据集路径
        static string TestData1 = Application.StartupPath + "\\testdata1_for_FP.dat";
        static string TestData2 = Application.StartupPath + "\\testdata2_for_FP.dat";
        static string TestData3 = Application.StartupPath + "\\testdata3_for_FP.dat";        
        #endregion

       
        public FormFPTree()
        {
            InitializeComponent();
            cbMinSupPercent.SelectedIndex = 0;
        }
       
        /// <summary>
        /// 分析数据
        /// </summary>
        /// <param name="_dataSetFileName"></param>
        private void DoAnalysis(string _dataSetFileName)
        {
            //FPTree
            FPTree _fpTree = new FPTree();
            //FPTree挖掘
            FPGrowthFacade _fpGrowthFacade = new FPGrowthFacade();    

            DateTime _dateTimeStart = DateTime.Now; 
            //读取数据集,并遍历取得频繁一项集
            ReadFile(_dataSetFileName, _fpTree);

            _fpTree.InitializeFPTree();
          
            _fpGrowthFacade.FPGrowth(_fpTree, null);

            _content.Text = "";

            if (_fpGrowthFacade.FrequentPattern.Count > 0)
            {
                DateTime _dateTimeEnd = DateTime.Now;
                TimeSpan _span = (_dateTimeEnd - _dateTimeStart);
                _content.Text += "程序分析总时间:" + _span.TotalSeconds.ToString() + "\r\n" +
                "支持度阀值设置:计数" + MinSupCount + ",支持度" + Math.Round((float)MinSupCount / TotalItems, 3) * 100 + "%\r\n\r\n";                

                foreach (DictionaryEntry _entry in _fpGrowthFacade.FrequentPattern)
                {
                    List<ItemSet> _tmpItemSetList = (List<ItemSet>)_fpGrowthFacade.FrequentPattern[_entry.Key];
                    _content.Text += "频繁" + _entry.Key + "项集" + _tmpItemSetList.Count+ "个\r\n";
                    
                    foreach (ItemSet _itemSet in _tmpItemSetList)
                    {
                        string[] _contents = new string[_itemSet.Content.Count];
                        _itemSet.Content.CopyTo(_contents);
                        _content.Text += ShowInOrder(_fpTree, _contents) + " (计数" + _itemSet.Count + 
                            ",支持度" + Math.Round((float)_itemSet.Count / TotalItems,3)*100 +"%)\r\n";
                    }
                }
                _content.Text += "频繁一项集" + _fpTree.FrequentItemCount + "个:\r\n";
                foreach (DictionaryEntry _entry in _fpTree.HashFrequentItemTable)
                {
                    _content.Text += _entry.Key + " (计数" + _entry.Value.ToString() +
                        ",支持度" + Math.Round((float)Convert.ToInt32(_entry.Value) / TotalItems, 3) * 100 + "%)\r\n";
                }
            }
            _fpTree.DisposeAll();
            _fpGrowthFacade.DisposeAll();
        }
        /// <summary>
        /// 按顺序显示字符
        /// </summary>
        /// <param name="_fpTree"></param>
        /// <param name="_contents"></param>
        /// <returns></returns>
        private string ShowInOrder(FPTree _fpTree,string[] _contents)
        {
            int[] _indexs = new int[_contents.Length];
            for (int i = 0; i < _indexs.Length; i++)
            {
                _indexs[i] = _fpTree.GetSupCountByID(_contents[i]);
            }
            int count = _indexs.Length;
            for (int bubble = 0; bubble < count; bubble++)
            {
                for (int lookup = bubble + 1; lookup < count; lookup++)
                {
                    //小的计数往下排
                    if (_indexs[bubble] <
                        _indexs[lookup])
                    {
                        string temp = _contents[bubble];
                        _contents[bubble] = _contents[lookup];
                        _contents[lookup] = temp;
                    }
                }
            }
            string _tmpContents = "";
            for (int i = 0; i < _contents.Length; i++)
            {
                _tmpContents += _contents[i] +" ";
            }
            return _tmpContents;
        }

        #region 系统方法
        private void button1_Click(object sender, EventArgs e)
        {
            if (TotalItems == 0)
            {
                MessageBox.Show("请先预处理数据,以计算事务总数", "系统提示");
                return;
            }

            SetFilePath();
            SetMinSupPercent();
            if (_testSet != "")
                DoAnalysis(_testSet);
            flowLayoutPanel1.Enabled = true;
            TotalItems = 0;
        }
        
       
        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            Close();
        }
        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            SetFilePath();
            //预处理,计算事务总项数
            flowLayoutPanel1.Enabled = false;
            StreamReader sr = new StreamReader(_testSet, System.Text.Encoding.Default);
            while (sr.Peek() >= 0)
            {
                //读取当前行
                sr.ReadLine();
                TotalItems++;
            }
            sr.Close();
        }
        /// <summary>
        /// 设置数据所在位置
        /// </summary>
        private void SetFilePath()
        {
            if (smallSet.Checked)
                _testSet = TestData1;
            if (bigSet.Checked)
                _testSet = TestData2;
            if (standardSet.Checked)
                _testSet = TestData3;
        }
        /// <summary>
        /// 设计支持度
        /// </summary>
        private void SetMinSupPercent()
        {          
            MinSupPercent = Convert.ToInt32(
                cbMinSupPercent.SelectedItem.ToString().Replace("%",""));
        }       
        #endregion

        #region 读取数据集,并遍历取得频繁一项集
        public void ReadFile(string strFileName, FPTree _fpTree)
        {
            //设置支持度计数
            MinSupCount = Convert.ToInt32(MinSupPercent) * TotalItems / 100;

            StreamReader sr = new StreamReader(strFileName, System.Text.Encoding.Default);
            string  sLineString = "";                     
            
            string[] _arrItemName = null;
            string _itemName = null;
           
            //设置测试数据集开始位置
            int _start = 0;
            if (_testSet == TestData1)
                _start = 1;
            //用Hashtable来保存一项集
            Hashtable _hashTable = new Hashtable();
            List<List<ItemInfo>> _listStrList = new List<List<ItemInfo>>();
                      
            while (sr.Peek() >= 0)
            {                
                //读取当前行
                sLineString = sr.ReadLine();
                //提取当前行的数组
                _arrItemName = sLineString.Split(new Char[] { ' ' }, 
                    StringSplitOptions.RemoveEmptyEntries);
                //将数组放到内存中
                //_arrStrList.Add(_arrItemName);
                List<ItemInfo> _listItemName = new List<ItemInfo>();
                //将频繁项集合放到Hashtable中

                for (int i = _start; i < _arrItemName.Length; i++)
                {                    
                    _itemName = _arrItemName[i];                     
                    _listItemName.Add(
                        new ItemInfo(_itemName,0));
                    int _count = 1;
                    if (_hashTable.ContainsKey(_itemName))
                    {
                        _count = Convert.ToInt32(_hashTable[_itemName]) + 1;
                        _hashTable[_itemName] = _count;                       
                    }
                    else
                        _hashTable.Add(_itemName, _count);
                    //频繁
                    if (_count > MinSupCount - 1)
                    {
                        _fpTree.BuildFrequentItemList(_itemName, _count);
                    }
                }
                _listStrList.Add(_listItemName); //对FPTree传参
                
            }
            _fpTree.ListStrList = _listStrList; 
            //设置支持度
            _fpTree.MinSupCount = MinSupCount; //对FPTree传参
            sr.Close();
        }
        #endregion         

       
    }
    
}