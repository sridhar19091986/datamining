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
    /// FPTree��ȡƵ��ģʽ
    /// </summary>
    public partial class FormFPTree : Form
    {
        /// <summary>
        /// ��С֧�ֶȼ���
        /// </summary>
        static int MinSupCount = 1000;
        /// <summary>
        /// ��С֧�ֶ�
        /// </summary>
        static int MinSupPercent = 8;
        /// <summary>
        /// �������ݿ��е�������������
        /// </summary>
        static int TotalItems = 0;
        /// <summary>
        /// ���Լ����ļ���
        /// </summary>
        static string _testSet = "";
        


        #region �������ݼ�·��
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
        /// ��������
        /// </summary>
        /// <param name="_dataSetFileName"></param>
        private void DoAnalysis(string _dataSetFileName)
        {
            //FPTree
            FPTree _fpTree = new FPTree();
            //FPTree�ھ�
            FPGrowthFacade _fpGrowthFacade = new FPGrowthFacade();    

            DateTime _dateTimeStart = DateTime.Now; 
            //��ȡ���ݼ�,������ȡ��Ƶ��һ�
            ReadFile(_dataSetFileName, _fpTree);

            _fpTree.InitializeFPTree();
          
            _fpGrowthFacade.FPGrowth(_fpTree, null);

            _content.Text = "";

            if (_fpGrowthFacade.FrequentPattern.Count > 0)
            {
                DateTime _dateTimeEnd = DateTime.Now;
                TimeSpan _span = (_dateTimeEnd - _dateTimeStart);
                _content.Text += "���������ʱ��:" + _span.TotalSeconds.ToString() + "\r\n" +
                "֧�ֶȷ�ֵ����:����" + MinSupCount + ",֧�ֶ�" + Math.Round((float)MinSupCount / TotalItems, 3) * 100 + "%\r\n\r\n";                

                foreach (DictionaryEntry _entry in _fpGrowthFacade.FrequentPattern)
                {
                    List<ItemSet> _tmpItemSetList = (List<ItemSet>)_fpGrowthFacade.FrequentPattern[_entry.Key];
                    _content.Text += "Ƶ��" + _entry.Key + "�" + _tmpItemSetList.Count+ "��\r\n";
                    
                    foreach (ItemSet _itemSet in _tmpItemSetList)
                    {
                        string[] _contents = new string[_itemSet.Content.Count];
                        _itemSet.Content.CopyTo(_contents);
                        _content.Text += ShowInOrder(_fpTree, _contents) + " (����" + _itemSet.Count + 
                            ",֧�ֶ�" + Math.Round((float)_itemSet.Count / TotalItems,3)*100 +"%)\r\n";
                    }
                }
                _content.Text += "Ƶ��һ�" + _fpTree.FrequentItemCount + "��:\r\n";
                foreach (DictionaryEntry _entry in _fpTree.HashFrequentItemTable)
                {
                    _content.Text += _entry.Key + " (����" + _entry.Value.ToString() +
                        ",֧�ֶ�" + Math.Round((float)Convert.ToInt32(_entry.Value) / TotalItems, 3) * 100 + "%)\r\n";
                }
            }
            _fpTree.DisposeAll();
            _fpGrowthFacade.DisposeAll();
        }
        /// <summary>
        /// ��˳����ʾ�ַ�
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
                    //С�ļ���������
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

        #region ϵͳ����
        private void button1_Click(object sender, EventArgs e)
        {
            if (TotalItems == 0)
            {
                MessageBox.Show("����Ԥ��������,�Լ�����������", "ϵͳ��ʾ");
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
            //Ԥ����,��������������
            flowLayoutPanel1.Enabled = false;
            StreamReader sr = new StreamReader(_testSet, System.Text.Encoding.Default);
            while (sr.Peek() >= 0)
            {
                //��ȡ��ǰ��
                sr.ReadLine();
                TotalItems++;
            }
            sr.Close();
        }
        /// <summary>
        /// ������������λ��
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
        /// ���֧�ֶ�
        /// </summary>
        private void SetMinSupPercent()
        {          
            MinSupPercent = Convert.ToInt32(
                cbMinSupPercent.SelectedItem.ToString().Replace("%",""));
        }       
        #endregion

        #region ��ȡ���ݼ�,������ȡ��Ƶ��һ�
        public void ReadFile(string strFileName, FPTree _fpTree)
        {
            //����֧�ֶȼ���
            MinSupCount = Convert.ToInt32(MinSupPercent) * TotalItems / 100;

            StreamReader sr = new StreamReader(strFileName, System.Text.Encoding.Default);
            string  sLineString = "";                     
            
            string[] _arrItemName = null;
            string _itemName = null;
           
            //���ò������ݼ���ʼλ��
            int _start = 0;
            if (_testSet == TestData1)
                _start = 1;
            //��Hashtable������һ�
            Hashtable _hashTable = new Hashtable();
            List<List<ItemInfo>> _listStrList = new List<List<ItemInfo>>();
                      
            while (sr.Peek() >= 0)
            {                
                //��ȡ��ǰ��
                sLineString = sr.ReadLine();
                //��ȡ��ǰ�е�����
                _arrItemName = sLineString.Split(new Char[] { ' ' }, 
                    StringSplitOptions.RemoveEmptyEntries);
                //������ŵ��ڴ���
                //_arrStrList.Add(_arrItemName);
                List<ItemInfo> _listItemName = new List<ItemInfo>();
                //��Ƶ����Ϸŵ�Hashtable��

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
                    //Ƶ��
                    if (_count > MinSupCount - 1)
                    {
                        _fpTree.BuildFrequentItemList(_itemName, _count);
                    }
                }
                _listStrList.Add(_listItemName); //��FPTree����
                
            }
            _fpTree.ListStrList = _listStrList; 
            //����֧�ֶ�
            _fpTree.MinSupCount = MinSupCount; //��FPTree����
            sr.Close();
        }
        #endregion         

       
    }
    
}