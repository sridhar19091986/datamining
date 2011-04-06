using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// FPGrowth������
    /// </summary>
    public class FPGrowthFacade 
    {
        /// <summary>
        /// ����Ƶ��K��Ĵ�С
        /// </summary>
        Hashtable _frequentPattern = new Hashtable();
        /// <summary>
        /// ����Ƶ��K��Ĵ�С
        /// </summary>
        public Hashtable FrequentPattern
        {
            get { return _frequentPattern; }            
        }
        
        
        /// <summary>
        /// ��Ƶ��һ�����,�Ӻ�ǰ����
        /// ��������FPTree
        /// </summary>
        /// <param name="_frequentItemList"></param>
        public void FPGrowth(FPTree _fpTree, List<TreeNodeItem> _itemSet)
        {
            if (_fpTree.SinglePath)
            {  
                //�����������,��_itemSetList����
                List<ItemSet> _itemSetList = new List<ItemSet>();
                CombineNodes(_fpTree, _itemSetList);
                //����ģʽ �¡Ȧ�,_itemSetList�Ǧµļ���                
                AddPattern(_itemSet, _itemSetList);
                
                _itemSet.Clear();
            }
            else
            {
                for (int i = _fpTree.FrequentItemCount - 1; i > -1; i--)
                {                    
                    //ͷ���е�ÿ��a(i)
                    TreeNodeItem _treeNodeItem = _fpTree.GetLinkNode(i);                    
                    //����ģʽ�� = a(i)�Ȧ�
                    List<TreeNodeItem> _itemSetConbine = new List<TreeNodeItem>();
                    if (_treeNodeItem != null)
                    {
                        if (_itemSet == null)
                            _itemSet = new List<TreeNodeItem>();
                        TreeNodeItem[] _arrTreeNodeItem = new TreeNodeItem[_itemSet.Count];
                        _itemSet.CopyTo(_arrTreeNodeItem);
                        _itemSetConbine.AddRange(_arrTreeNodeItem);
                        _itemSetConbine.Insert(0,_treeNodeItem);                        
                    }
                    //��������FPTree(1)
                    FPTree _conditionFPTree = new FPTree();
                    _conditionFPTree.MinSupCount = _fpTree.MinSupCount;
                    List<List<TreeNodeItem>> _listStrList = new List<List<TreeNodeItem>>();                    
                    Hashtable _hashTable = new Hashtable();  

                    bool _doLoop = (_treeNodeItem != null);
                    while (_doLoop)
                    {
                        //��������ģʽ��,֧�ֶȼ�����С�������List
                        List<TreeNodeItem> _treeNodeItemList = new List<TreeNodeItem>();
                        GetParentPath(_treeNodeItem.CloneNode(), _treeNodeItemList, _hashTable, _conditionFPTree);

                        //���ģʽ�Ѿ��������Ҳ���Ƶ��һ�
                        if (_treeNodeItemList.Count == 0 && _itemSetConbine.Count >1)                        
                            //����ģʽ 
                            AddPattern(_itemSetConbine,_fpTree);                            
                        
                        //��������FPTree(2)
                        if(_treeNodeItemList.Count>0)                            
                        _listStrList.Add(_treeNodeItemList);                    
                        
                        //�������ģʽ��
                        if (_treeNodeItem.NextNode != null)
                            _treeNodeItem = _treeNodeItem.NextNode;
                        else
                            _doLoop = false;
                    }                   
                    //��������FPTree(3)
                    if(_listStrList.Count >0)
                    _conditionFPTree.InitializeConditionFPTree(_listStrList);
                
                    //������Ƿ�Ϊ��
                    if (_conditionFPTree.TopNode.Nodes.Count > 0)
                        FPGrowth(_conditionFPTree, _itemSetConbine);   
                    else
                        _itemSetConbine = _itemSet;
                }                
            }
        }
        /// <summary>
        /// ��Ϊ��ʱ����ģʽ
        /// </summary>
        /// <param name="_itemSetConbine">����Ħ� = a(i)�Ȧ�</param>
        private void AddPattern(List<TreeNodeItem> _itemSetConbine,FPTree _fpTree)
        {
            //����ģʽ 
            if (!_frequentPattern.ContainsKey(_itemSetConbine.Count))
            {
                List<ItemSet> _tmpItemSetList = new List<ItemSet>();
                ItemSet _tmpItemSet = new ItemSet();
                //����ģʽ              
                foreach (TreeNodeItem _nodes in _itemSetConbine)
                {
                    _tmpItemSet.Content.Add(_nodes.ItemName);                    
                }
                _tmpItemSet.Count =
                        _fpTree.GetSupCountByID(
                        _itemSetConbine[0].ItemName);
                _tmpItemSetList.Add(_tmpItemSet);
                _frequentPattern.Add(_itemSetConbine.Count, _tmpItemSetList);
            }
            else
            {
                List<ItemSet> _tmpItemSetList = (List<ItemSet>)_frequentPattern[_itemSetConbine.Count];
                ItemSet _tmpItemSet = new ItemSet();
                //����/���æ�������           
                foreach (TreeNodeItem _nodes in _itemSetConbine)
                {
                    _tmpItemSet.Content.Add(_nodes.ItemName);                    
                }
                _tmpItemSet.Count =
                        _fpTree.GetSupCountByID(
                        _itemSetConbine[0].ItemName);
                _tmpItemSetList.Add(_tmpItemSet);
                //_frequentPattern[_itemSetConbine.Count] = _tmpItemSetList;
            }
        }
        /// <summary>
        /// ��Ϊ��һ·��ʱ����ģʽ
        /// </summary>
        /// <param name="_itemSet">����Ħ�</param>
        /// <param name="_itemSetList">�ڵ����Ϧ�'</param>
        private void AddPattern(List<TreeNodeItem> _itemSet, List<ItemSet> _itemSetList)
        {
            for (int i = 0; i < _itemSetList.Count; i++)
            {
                //ÿ����Ϧ�
                ItemSet _tmpItemSet = _itemSetList[i];

                int _count = _itemSet.Count + _tmpItemSet.Content.Count;
                if (!_frequentPattern.ContainsKey(_count))
                {
                    List<ItemSet> _tmpItemSetList = new List<ItemSet>();
                    //���Ӧ�������          
                    foreach (TreeNodeItem _nodes in _itemSet)
                    {
                        //�¡Ȧ���С֧�ֶ�Ϊ��(��Combine���Ѿ����ýڵ���С��)
                        _tmpItemSet.Content.Add(_nodes.ItemName);
                        //_tmpItemSet.Count = _tmpItemSet.Count;
                    }
                    _tmpItemSetList.Add(_tmpItemSet);
                    _frequentPattern.Add(_count, _tmpItemSetList);
                }
                else
                {
                    List<ItemSet> _tmpItemSetList = (List<ItemSet>)_frequentPattern[_count];
                    //���Ӧ�������
                    foreach (TreeNodeItem _nodes in _itemSet)
                    {
                        //�¡Ȧ���С֧�ֶ�Ϊ��(��Combine���Ѿ����ýڵ���С��)
                        _tmpItemSet.Content.Add(_nodes.ItemName);
                        //_tmpItemSet.Count = _tmpItemSet.Count;
                    }
                    _tmpItemSetList.Add(_tmpItemSet);
                    //_frequentPattern[_count] = _tmpItemSetList;
                }
            }
        }

        /// <summary>
        /// ����һ�������ӽڵ�ĸ����
        /// ���ø����ļ���ΪҶ�ӽ�����
        /// ������
        /// </summary>
        /// <param name="_treeNodeItem"></param>
        /// <param name="_treeNodeItemList"></param>
        public void GetParentPath(TreeNodeItem _treeNodeItem, List<TreeNodeItem> _treeNodeItemList, Hashtable _hashTable,FPTree _fpTree)
        {
            while (_treeNodeItem.Parent != null && _treeNodeItem.Parent.ItemName != null)
            {
                //����ģʽ�����еĽڵ��֧�ֶȼ�����ΪҶ�Ӽ���
                TreeNodeItem _newTreeNodeItem = _treeNodeItem.Parent.CloneNode();
                _newTreeNodeItem.Count = _treeNodeItem.Count;
                
                _treeNodeItemList.Insert(0, _newTreeNodeItem);
                //����Ƶ��һ�
                if (_hashTable.ContainsKey(_newTreeNodeItem.ItemName))
                    _hashTable[_newTreeNodeItem.ItemName] = Convert.ToInt32(_hashTable[_newTreeNodeItem.ItemName]) +
                        _newTreeNodeItem.Count;
                else
                    _hashTable.Add(_newTreeNodeItem.ItemName, _newTreeNodeItem.Count);
                //Ƶ��
                if (Convert.ToInt32(_hashTable[_newTreeNodeItem.ItemName]) > _fpTree.MinSupCount - 1)
                {
                    _fpTree.BuildFrequentItemList(_newTreeNodeItem.ItemName,
                        Convert.ToInt32(_hashTable[_newTreeNodeItem.ItemName]));
                }
                _treeNodeItem = _newTreeNodeItem;
            } 
        }
        
        /// <summary>
        /// ������е�ģʽ
        /// ��ȡ�Ӽ��㷨
        /// </summary>
        /// <param name="_treeNodeItemList"></param>
        public void CombineNodes(FPTree _fpTree, List<ItemSet> _itemSetList)
        {
            //��ȡ���н��
            List<TreeNodeItem> _treeNodeItemList = new List<TreeNodeItem>();
            TreeNodeItem _treeNodeItem = _fpTree.TopNode;
            bool _hasNext = (_treeNodeItem.Nodes.Count > 0);
            _treeNodeItem = _treeNodeItem.Nodes[0];
            while (_hasNext)
            {
                _treeNodeItemList.Add(_treeNodeItem);
                if (_treeNodeItem.Nodes.Count > 0)
                    _treeNodeItem = _treeNodeItem.Nodes[0];
                else
                    _hasNext = false;
            }

            int _length = _treeNodeItemList.Count;
            //��1��ʼ,��Ϊ��ȡ�ռ�
            for (int i = 1; i < Math.Pow(2, _length); i++)
            {
                String str = Convert.ToString(int.Parse(i.ToString()), 2);
                str = str.PadLeft(_length, '0');
                char[] _arrChr = str.ToCharArray();             
                ItemSet _itemSet = new ItemSet();
                for (int j = _arrChr.Length-1; j >-1; j--)
                {
                    if (_arrChr[j] == '1')
                    {
                        _itemSet.Content.Add(_treeNodeItemList[j].ItemName);
                        if (_itemSet.Count == 0)
                            _itemSet.Count = _treeNodeItemList[j].Count;
                        else
                        {
                            //֧�ֶȵ��ڦ��н����С��֧�ֶȼ���
                            if(_treeNodeItemList[j].Count < _itemSet.Count)
                            _itemSet.Count = _treeNodeItemList[j].Count;
                        }
                    }
                }
                if(_itemSet.Content.Count!=0)
                    _itemSetList.Add(_itemSet);               
            }
        }

        /// <summary>
        /// ������е����ݼ�
        /// </summary>
        public void DisposeAll()
        {
            _frequentPattern.Clear();           
        }       

    }
}
