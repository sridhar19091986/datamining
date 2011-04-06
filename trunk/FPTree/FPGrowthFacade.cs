using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// FPGrowth操作类
    /// </summary>
    public class FPGrowthFacade 
    {
        /// <summary>
        /// 计算频繁K项集的大小
        /// </summary>
        Hashtable _frequentPattern = new Hashtable();
        /// <summary>
        /// 计算频繁K项集的大小
        /// </summary>
        public Hashtable FrequentPattern
        {
            get { return _frequentPattern; }            
        }
        
        
        /// <summary>
        /// 在频繁一项集表中,从后到前遍历
        /// 生成条件FPTree
        /// </summary>
        /// <param name="_frequentItemList"></param>
        public void FPGrowth(FPTree _fpTree, List<TreeNodeItem> _itemSet)
        {
            if (_fpTree.SinglePath)
            {  
                //返回所有组合,用_itemSetList保存
                List<ItemSet> _itemSetList = new List<ItemSet>();
                CombineNodes(_fpTree, _itemSetList);
                //产生模式 β∪α,_itemSetList是β的集合                
                AddPattern(_itemSet, _itemSetList);
                
                _itemSet.Clear();
            }
            else
            {
                for (int i = _fpTree.FrequentItemCount - 1; i > -1; i--)
                {                    
                    //头表中的每个a(i)
                    TreeNodeItem _treeNodeItem = _fpTree.GetLinkNode(i);                    
                    //产生模式β = a(i)∪α
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
                    //构造条件FPTree(1)
                    FPTree _conditionFPTree = new FPTree();
                    _conditionFPTree.MinSupCount = _fpTree.MinSupCount;
                    List<List<TreeNodeItem>> _listStrList = new List<List<TreeNodeItem>>();                    
                    Hashtable _hashTable = new Hashtable();  

                    bool _doLoop = (_treeNodeItem != null);
                    while (_doLoop)
                    {
                        //构造条件模式基,支持度计数由小到大放入List
                        List<TreeNodeItem> _treeNodeItemList = new List<TreeNodeItem>();
                        GetParentPath(_treeNodeItem.CloneNode(), _treeNodeItemList, _hashTable, _conditionFPTree);

                        //如果模式已经产生并且不是频繁一项集
                        if (_treeNodeItemList.Count == 0 && _itemSetConbine.Count >1)                        
                            //产生模式 
                            AddPattern(_itemSetConbine,_fpTree);                            
                        
                        //构造条件FPTree(2)
                        if(_treeNodeItemList.Count>0)                            
                        _listStrList.Add(_treeNodeItemList);                    
                        
                        //多个条件模式基
                        if (_treeNodeItem.NextNode != null)
                            _treeNodeItem = _treeNodeItem.NextNode;
                        else
                            _doLoop = false;
                    }                   
                    //构造条件FPTree(3)
                    if(_listStrList.Count >0)
                    _conditionFPTree.InitializeConditionFPTree(_listStrList);
                
                    //检查树是否为空
                    if (_conditionFPTree.TopNode.Nodes.Count > 0)
                        FPGrowth(_conditionFPTree, _itemSetConbine);   
                    else
                        _itemSetConbine = _itemSet;
                }                
            }
        }
        /// <summary>
        /// 树为空时产生模式
        /// </summary>
        /// <param name="_itemSetConbine">传入的β = a(i)∪α</param>
        private void AddPattern(List<TreeNodeItem> _itemSetConbine,FPTree _fpTree)
        {
            //产生模式 
            if (!_frequentPattern.ContainsKey(_itemSetConbine.Count))
            {
                List<ItemSet> _tmpItemSetList = new List<ItemSet>();
                ItemSet _tmpItemSet = new ItemSet();
                //产生模式              
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
                //增加/设置α的内容           
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
        /// 树为单一路径时产生模式
        /// </summary>
        /// <param name="_itemSet">传入的α</param>
        /// <param name="_itemSetList">节点的组合β'</param>
        private void AddPattern(List<TreeNodeItem> _itemSet, List<ItemSet> _itemSetList)
        {
            for (int i = 0; i < _itemSetList.Count; i++)
            {
                //每个组合β
                ItemSet _tmpItemSet = _itemSetList[i];

                int _count = _itemSet.Count + _tmpItemSet.Content.Count;
                if (!_frequentPattern.ContainsKey(_count))
                {
                    List<ItemSet> _tmpItemSetList = new List<ItemSet>();
                    //增加α的内容          
                    foreach (TreeNodeItem _nodes in _itemSet)
                    {
                        //β∪α最小支持度为β(在Combine中已经设置节点最小的)
                        _tmpItemSet.Content.Add(_nodes.ItemName);
                        //_tmpItemSet.Count = _tmpItemSet.Count;
                    }
                    _tmpItemSetList.Add(_tmpItemSet);
                    _frequentPattern.Add(_count, _tmpItemSetList);
                }
                else
                {
                    List<ItemSet> _tmpItemSetList = (List<ItemSet>)_frequentPattern[_count];
                    //增加α的内容
                    foreach (TreeNodeItem _nodes in _itemSet)
                    {
                        //β∪α最小支持度为β(在Combine中已经设置节点最小的)
                        _tmpItemSet.Content.Add(_nodes.ItemName);
                        //_tmpItemSet.Count = _tmpItemSet.Count;
                    }
                    _tmpItemSetList.Add(_tmpItemSet);
                    //_frequentPattern[_count] = _tmpItemSetList;
                }
            }
        }

        /// <summary>
        /// 返回一个包含子节点的父结点
        /// 设置父结点的计数为叶子结点计数
        /// 复制树
        /// </summary>
        /// <param name="_treeNodeItem"></param>
        /// <param name="_treeNodeItemList"></param>
        public void GetParentPath(TreeNodeItem _treeNodeItem, List<TreeNodeItem> _treeNodeItemList, Hashtable _hashTable,FPTree _fpTree)
        {
            while (_treeNodeItem.Parent != null && _treeNodeItem.Parent.ItemName != null)
            {
                //条件模式基所有的节点的支持度计数都为叶子计数
                TreeNodeItem _newTreeNodeItem = _treeNodeItem.Parent.CloneNode();
                _newTreeNodeItem.Count = _treeNodeItem.Count;
                
                _treeNodeItemList.Insert(0, _newTreeNodeItem);
                //建立频繁一项集
                if (_hashTable.ContainsKey(_newTreeNodeItem.ItemName))
                    _hashTable[_newTreeNodeItem.ItemName] = Convert.ToInt32(_hashTable[_newTreeNodeItem.ItemName]) +
                        _newTreeNodeItem.Count;
                else
                    _hashTable.Add(_newTreeNodeItem.ItemName, _newTreeNodeItem.Count);
                //频繁
                if (Convert.ToInt32(_hashTable[_newTreeNodeItem.ItemName]) > _fpTree.MinSupCount - 1)
                {
                    _fpTree.BuildFrequentItemList(_newTreeNodeItem.ItemName,
                        Convert.ToInt32(_hashTable[_newTreeNodeItem.ItemName]));
                }
                _treeNodeItem = _newTreeNodeItem;
            } 
        }
        
        /// <summary>
        /// 获得所有的模式
        /// 获取子集算法
        /// </summary>
        /// <param name="_treeNodeItemList"></param>
        public void CombineNodes(FPTree _fpTree, List<ItemSet> _itemSetList)
        {
            //获取所有结点
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
            //从1开始,因为不取空集
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
                            //支持度等于β中结点最小的支持度计数
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
        /// 清空所有的数据集
        /// </summary>
        public void DisposeAll()
        {
            _frequentPattern.Clear();           
        }       

    }
}
