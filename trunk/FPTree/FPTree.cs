using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// FPTree描述类
    /// </summary>
    public class FPTree
    {

        #region 属性部分
        private int _minSupCount;
        /// <summary>
        /// 最小支持度计数
        /// </summary>
        public int MinSupCount
        {
            get { return _minSupCount; }
            set { _minSupCount = value; }
        }
        /// <summary>
        /// 将所有的事务放到内存中
        /// </summary>
        private List<List<ItemInfo>> _listStrList = new List<List<ItemInfo>>();
        /// <summary>
        /// 所有事务集
        /// </summary>
        public List<List<ItemInfo>> ListStrList
        {
            get { return _listStrList; }
            set { _listStrList = value; }
        }
        /// <summary>
        /// 项头表的大小
        /// </summary>
        public int FrequentItemCount
        {
            get { return _hashFrequentItemLinkTable.Count; }
        }
        /// <summary>
        /// 树是否只有单一路径
        /// </summary>
        public bool SinglePath
        {
            get {
                bool _isSinglePath = true;
                for(int i=0;i<_hashFrequentItemLinkTable.Count;i++)
                {
                    TreeNodeItem _treeNodeItem = GetLinkNode(i);
                    if (_treeNodeItem != null && _treeNodeItem.NextNode != null)
                    {
                        _isSinglePath = false;
                        break;
                    }
                }
                return _isSinglePath;
            }
        }
        /// <summary>
        /// 根据Key返回支持度计数
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public int GetSupCountByID(object key)
        {
            return Convert.ToInt32(_hashFrequentItemTable[key]);
        }
       
        /// <summary>
        /// 频繁一项集
        /// </summary>
        //private List<FrequentItem> _frequentItemList = new List<FrequentItem>();
        private Hashtable _hashFrequentItemTable = new Hashtable();
        /// <summary>
        /// 获取频繁一项集
        /// </summary>
        public Hashtable HashFrequentItemTable
        {
            get { return _hashFrequentItemTable; }           
        }
        /// <summary>
        /// 频繁一项集链表
        /// </summary>
        private Hashtable _hashFrequentItemLinkTable = new Hashtable();
        /// <summary>
        /// 频繁一项集索引和标识的影射
        /// </summary>
        private Hashtable _indexToId = new Hashtable();
        private Hashtable _idToIndex = new Hashtable();
        
        /// <summary>
        /// FPTree的NULL节点
        /// </summary>
        private TreeNodeItem _topNode = new TreeNodeItem();
        /// <summary>
        /// FPTree的NULL节点
        /// </summary>
        public TreeNodeItem TopNode
        {
            get { return _topNode; }
            set { _topNode = value; }
        }


        #endregion

        /// <summary>
        /// 建立FPTree
        /// </summary>
        public void InitializeFPTree()
        {
            // 每个频繁项,按频繁项集列表(L)次序排序
            //生成FPTree            
            //将事务数据库压缩到树中
            BuildFPTree(_listStrList, _hashFrequentItemLinkTable, _indexToId);
        }
        /// <summary>
        /// 建立条件FPTree
        /// </summary>
        /// <param name="_listStrList"></param>
        public void InitializeConditionFPTree(List<List<TreeNodeItem>> _listStrList)
        {
            BuildConditionFPTree(_listStrList, _hashFrequentItemLinkTable, _indexToId);
            _listStrList.Clear();
        }
     
        /// <summary>
        /// 建立频繁一项集的列表
        /// </summary>
        /// <param name="_hashTable"></param>
        /// <param name="_frequentItemList"></param>       
        public void BuildFrequentItemList(string _itemName,int _count)
        {
            //将达到最小支持度计数的频繁项添加到集合    
            if (!_hashFrequentItemTable.ContainsKey(_itemName))
            {
                _indexToId.Add(_hashFrequentItemTable.Count, _itemName);
                _idToIndex.Add(_itemName, _hashFrequentItemTable.Count);
                _hashFrequentItemLinkTable.Add(_itemName,
                    new FrequentLinkItem(_hashFrequentItemTable.Count, null));
                _hashFrequentItemTable.Add(_itemName, _count);
            }
            else
                _hashFrequentItemTable[_itemName]= _count;
        }

        /// <summary>
        /// 每个频繁项,按频繁项集列表(L)次序排序
        /// 生成FPTree
        /// </summary>
        /// <param name="_listStrList"></param>
        /// <param name="_frequentItemList"></param>
        /// <param name="_conditionTreeNodeItem"></param>
        /// <param name="_hashFrequentItemLinkTable"></param>
        /// <param name="_indexToId"></param>
        private void BuildFPTree(List<List<ItemInfo>> _listStrList, Hashtable _hashFrequentItemLinkTable, Hashtable _indexToId)
        {
            //冒泡排序,排列频繁项集列表      
            int count = _hashFrequentItemTable.Count;
            for (int bubble = 0; bubble < count; bubble++)
            {
                for (int lookup = bubble + 1; lookup < count; lookup++)
                {
                    //小的计数往下排
                    if ((int)_hashFrequentItemTable[_indexToId[bubble]] <
                        (int)_hashFrequentItemTable[_indexToId[lookup]])
                    {
                        object temp = _indexToId[bubble];
                        _indexToId[bubble] = _indexToId[lookup];
                        _indexToId[lookup] = temp;
                        temp = _idToIndex[_indexToId[bubble]];
                        _idToIndex[_indexToId[bubble]] = _idToIndex[_indexToId[lookup]];
                        _idToIndex[_indexToId[lookup]] = temp;
                    }
                }
            }
            //有删除操作,都用递减
            for (int index = _listStrList.Count-1; index >-1; index--)
            {
                List<ItemInfo> _arrStrValue = _listStrList[index];

                for (int i = _arrStrValue.Count - 1; i > -1; i--)
                {
                    ItemInfo _itemInfo = _arrStrValue[i];
                    if (_hashFrequentItemTable.ContainsKey(_itemInfo.ItemName))                    
                        //以index排序
                        _itemInfo.Index = Convert.ToInt32(_idToIndex[_itemInfo.ItemName]);                    
                    else                    
                        _arrStrValue.RemoveAt(i);                    
                }
                //如果不包含,达到最小支持度计数的事务
                if (_arrStrValue.Count == 0)
                {
                    _listStrList.RemoveAt(index);
                    continue;
                }

                //冒泡排序            
                count = _arrStrValue.Count;
                for (int bubble = 0; bubble < count; bubble++)
                {
                    for (int lookup = bubble + 1; lookup < count; lookup++)
                    {
                        //小的计数往下排,项头表中Index越大,其出现次数越小                 
                        if (_arrStrValue[bubble].Index > _arrStrValue[lookup].Index)
                        {
                            ItemInfo temp = _arrStrValue[bubble];
                            _arrStrValue[bubble] = _arrStrValue[lookup];
                            _arrStrValue[lookup] = temp;
                        }
                    }
                }
                CreateTree(_arrStrValue, _topNode, _hashFrequentItemLinkTable);
            }
            //_hashFrequentItemTable.Clear();
        }
        /// <summary>
        /// 建立条件FPTree
        /// </summary>
        /// <param name="_listStrList"></param>
        /// <param name="_frequentItemList"></param>
        /// <param name="_hashFrequentItemLinkTable"></param>
        /// <param name="_indexToId"></param>
        private void BuildConditionFPTree(List<List<TreeNodeItem>> _listStrList,  Hashtable _hashFrequentItemLinkTable, Hashtable _indexToId)
        {
            //冒泡排序,排列频繁项集列表      
            int count = _hashFrequentItemTable.Count;
            for (int bubble = 0; bubble < count; bubble++)
            {
                for (int lookup = bubble + 1; lookup < count; lookup++)
                {
                    //小的计数往下排
                    if ((int)_hashFrequentItemTable[_indexToId[bubble]] <
                        (int)_hashFrequentItemTable[_indexToId[lookup]])
                    {
                        object temp = _indexToId[bubble];
                        _indexToId[bubble] = _indexToId[lookup];
                        _indexToId[lookup] = temp;
                        temp = _idToIndex[_indexToId[bubble]];
                        _idToIndex[_indexToId[bubble]] = _idToIndex[_indexToId[lookup]];
                        _idToIndex[_indexToId[lookup]] = temp;
                    }
                }
            }
            //有删除操作,都用递减
            for (int index = _listStrList.Count-1; index >-1; index--)            
            {
                List<TreeNodeItem> _arrStrValue = _listStrList[index];

                for (int i = _arrStrValue.Count - 1; i > -1; i--)
                {
                    TreeNodeItem _itemInfo = _arrStrValue[i];
                    if (_hashFrequentItemTable.ContainsKey(_itemInfo.ItemName))                    
                        //设计index以排序
                        _itemInfo.Index = Convert.ToInt32(_idToIndex[_itemInfo.ItemName]);                    
                    else                    
                        _arrStrValue.RemoveAt(i);                    
                }
                //如果不包含,达到最小支持度计数的事务
                if (_arrStrValue.Count == 0)
                {
                    _listStrList.RemoveAt(index);
                    continue;
                }

                //冒泡排序            
                count = _arrStrValue.Count;
                for (int bubble = 0; bubble < count; bubble++)
                {
                    for (int lookup = bubble + 1; lookup < count; lookup++)
                    {
                        //小的计数往下排,项头表中Index越大,其出现次数越小                             
                        if (_arrStrValue[bubble].Index > _arrStrValue[lookup].Index)
                        {
                            TreeNodeItem temp = _arrStrValue[bubble];
                            _arrStrValue[bubble] = _arrStrValue[lookup];
                            _arrStrValue[lookup] = temp;
                        }
                    }
                }               
                CreateConditionTree(_arrStrValue, _topNode, _hashFrequentItemLinkTable);
            }
           
        }


        /// <summary>
        /// 返回下一个结点
        /// </summary>
        /// <param name="_preNode"></param>
        /// <returns></returns>
        private TreeNodeItem GetNextNode(TreeNodeItem _currentNode)
        {
            if (_currentNode.NextNode == null) return _currentNode;
            else
                return GetNextNode(_currentNode.NextNode);
        }

        /// <summary>
        /// 递归建立FPTree
        /// </summary>
        /// <param name="_listItemNameList"></param>
        /// <param name="_treeNode"></param>
        private void CreateTree(List<ItemInfo> _listItemNameList, TreeNodeItem _treeNode, Hashtable _hashFrequentItemLinkTable)
        {
            if (_listItemNameList.Count == 0) return;

            bool _isExists = false;
            TreeNodeItem _currentNode = null;
            foreach (TreeNodeItem _subTreeNode in _treeNode.Nodes)
            {
                if (_subTreeNode.ItemName == _listItemNameList[0].ItemName)
                {
                    _subTreeNode.Count += 1;
                    _isExists = true;
                    _currentNode = _subTreeNode;
                    break;
                }
            }
            if (!_isExists)
            {
                _currentNode = new TreeNodeItem(_listItemNameList[0].ItemName, 1);
                FrequentLinkItem _frequentLinkItem = (FrequentLinkItem)_hashFrequentItemLinkTable[_currentNode.ItemName];
                //TreeNodeItem _nextNode = _frequentItemList[_itemIndex].NextNode;
                if (_frequentLinkItem.LinkNode == null)
                    _frequentLinkItem.LinkNode = _currentNode;
                else
                    GetNextNode(_frequentLinkItem.LinkNode).NextNode = _currentNode;

                _treeNode.Nodes.Add(_currentNode);
            }
            _listItemNameList.RemoveAt(0);
            if (_currentNode != null)
                CreateTree(_listItemNameList, _currentNode, _hashFrequentItemLinkTable);
        }
        /// <summary>
        /// 建立条件FPTree
        /// </summary>
        /// <param name="_listItemNameList"></param>
        /// <param name="_treeNode"></param>
        /// <param name="_hashFrequentItemLinkTable"></param>
        private void CreateConditionTree(List<TreeNodeItem> _listItemNameList, TreeNodeItem _treeNode, Hashtable _hashFrequentItemLinkTable)
        {
            if (_listItemNameList.Count == 0) return;

            bool _isExists = false;
            TreeNodeItem _currentNode = null;
            foreach (TreeNodeItem _subTreeNode in _treeNode.Nodes)
            {
                if (_subTreeNode.ItemName == _listItemNameList[0].ItemName)
                {
                    _subTreeNode.Count += _listItemNameList[0].Count;
                    _isExists = true;
                    _currentNode = _subTreeNode;
                    break;
                }
            }
            if (!_isExists)
            {
                _currentNode = new TreeNodeItem(_listItemNameList[0].ItemName, _listItemNameList[0].Count);
                FrequentLinkItem _frequentLinkItem = (FrequentLinkItem)_hashFrequentItemLinkTable[_currentNode.ItemName];
                //TreeNodeItem _nextNode = _frequentItemList[_itemIndex].NextNode;
                if (_frequentLinkItem.LinkNode == null)
                    _frequentLinkItem.LinkNode = _currentNode;
                else
                    GetNextNode(_frequentLinkItem.LinkNode).NextNode = _currentNode;

                _treeNode.Nodes.Add(_currentNode);
            }
            _listItemNameList.RemoveAt(0);
            if (_currentNode != null)
                CreateConditionTree(_listItemNameList, _currentNode, _hashFrequentItemLinkTable);
        }

        /// <summary>
        /// 根据索引获取项头表的Header
        /// </summary>
        /// <param name="_index"></param>
        /// <returns></returns>
        public TreeNodeItem GetLinkNode(int _index)
        {
            if (_index > _hashFrequentItemLinkTable.Count || _index < 0) return null;
            FrequentLinkItem _frequentLinkItem = (FrequentLinkItem)_hashFrequentItemLinkTable[_indexToId[_index]];            
            TreeNodeItem _treeNodeItem = _frequentLinkItem.LinkNode;
            return _treeNodeItem;
        }

        /// <summary>
        /// 清空所有的数据集
        /// </summary>
        public void DisposeAll()
        {
            _listStrList.Clear();                       
            _hashFrequentItemLinkTable.Clear();
            _indexToId.Clear();
            _topNode.Nodes.Clear();
            _hashFrequentItemTable.Clear();
        }
        
    }
}
