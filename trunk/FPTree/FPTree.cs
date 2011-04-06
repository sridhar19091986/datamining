using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// FPTree������
    /// </summary>
    public class FPTree
    {

        #region ���Բ���
        private int _minSupCount;
        /// <summary>
        /// ��С֧�ֶȼ���
        /// </summary>
        public int MinSupCount
        {
            get { return _minSupCount; }
            set { _minSupCount = value; }
        }
        /// <summary>
        /// �����е�����ŵ��ڴ���
        /// </summary>
        private List<List<ItemInfo>> _listStrList = new List<List<ItemInfo>>();
        /// <summary>
        /// ��������
        /// </summary>
        public List<List<ItemInfo>> ListStrList
        {
            get { return _listStrList; }
            set { _listStrList = value; }
        }
        /// <summary>
        /// ��ͷ��Ĵ�С
        /// </summary>
        public int FrequentItemCount
        {
            get { return _hashFrequentItemLinkTable.Count; }
        }
        /// <summary>
        /// ���Ƿ�ֻ�е�һ·��
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
        /// ����Key����֧�ֶȼ���
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public int GetSupCountByID(object key)
        {
            return Convert.ToInt32(_hashFrequentItemTable[key]);
        }
       
        /// <summary>
        /// Ƶ��һ�
        /// </summary>
        //private List<FrequentItem> _frequentItemList = new List<FrequentItem>();
        private Hashtable _hashFrequentItemTable = new Hashtable();
        /// <summary>
        /// ��ȡƵ��һ�
        /// </summary>
        public Hashtable HashFrequentItemTable
        {
            get { return _hashFrequentItemTable; }           
        }
        /// <summary>
        /// Ƶ��һ�����
        /// </summary>
        private Hashtable _hashFrequentItemLinkTable = new Hashtable();
        /// <summary>
        /// Ƶ��һ������ͱ�ʶ��Ӱ��
        /// </summary>
        private Hashtable _indexToId = new Hashtable();
        private Hashtable _idToIndex = new Hashtable();
        
        /// <summary>
        /// FPTree��NULL�ڵ�
        /// </summary>
        private TreeNodeItem _topNode = new TreeNodeItem();
        /// <summary>
        /// FPTree��NULL�ڵ�
        /// </summary>
        public TreeNodeItem TopNode
        {
            get { return _topNode; }
            set { _topNode = value; }
        }


        #endregion

        /// <summary>
        /// ����FPTree
        /// </summary>
        public void InitializeFPTree()
        {
            // ÿ��Ƶ����,��Ƶ����б�(L)��������
            //����FPTree            
            //���������ݿ�ѹ��������
            BuildFPTree(_listStrList, _hashFrequentItemLinkTable, _indexToId);
        }
        /// <summary>
        /// ��������FPTree
        /// </summary>
        /// <param name="_listStrList"></param>
        public void InitializeConditionFPTree(List<List<TreeNodeItem>> _listStrList)
        {
            BuildConditionFPTree(_listStrList, _hashFrequentItemLinkTable, _indexToId);
            _listStrList.Clear();
        }
     
        /// <summary>
        /// ����Ƶ��һ����б�
        /// </summary>
        /// <param name="_hashTable"></param>
        /// <param name="_frequentItemList"></param>       
        public void BuildFrequentItemList(string _itemName,int _count)
        {
            //���ﵽ��С֧�ֶȼ�����Ƶ������ӵ�����    
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
        /// ÿ��Ƶ����,��Ƶ����б�(L)��������
        /// ����FPTree
        /// </summary>
        /// <param name="_listStrList"></param>
        /// <param name="_frequentItemList"></param>
        /// <param name="_conditionTreeNodeItem"></param>
        /// <param name="_hashFrequentItemLinkTable"></param>
        /// <param name="_indexToId"></param>
        private void BuildFPTree(List<List<ItemInfo>> _listStrList, Hashtable _hashFrequentItemLinkTable, Hashtable _indexToId)
        {
            //ð������,����Ƶ����б�      
            int count = _hashFrequentItemTable.Count;
            for (int bubble = 0; bubble < count; bubble++)
            {
                for (int lookup = bubble + 1; lookup < count; lookup++)
                {
                    //С�ļ���������
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
            //��ɾ������,���õݼ�
            for (int index = _listStrList.Count-1; index >-1; index--)
            {
                List<ItemInfo> _arrStrValue = _listStrList[index];

                for (int i = _arrStrValue.Count - 1; i > -1; i--)
                {
                    ItemInfo _itemInfo = _arrStrValue[i];
                    if (_hashFrequentItemTable.ContainsKey(_itemInfo.ItemName))                    
                        //��index����
                        _itemInfo.Index = Convert.ToInt32(_idToIndex[_itemInfo.ItemName]);                    
                    else                    
                        _arrStrValue.RemoveAt(i);                    
                }
                //���������,�ﵽ��С֧�ֶȼ���������
                if (_arrStrValue.Count == 0)
                {
                    _listStrList.RemoveAt(index);
                    continue;
                }

                //ð������            
                count = _arrStrValue.Count;
                for (int bubble = 0; bubble < count; bubble++)
                {
                    for (int lookup = bubble + 1; lookup < count; lookup++)
                    {
                        //С�ļ���������,��ͷ����IndexԽ��,����ִ���ԽС                 
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
        /// ��������FPTree
        /// </summary>
        /// <param name="_listStrList"></param>
        /// <param name="_frequentItemList"></param>
        /// <param name="_hashFrequentItemLinkTable"></param>
        /// <param name="_indexToId"></param>
        private void BuildConditionFPTree(List<List<TreeNodeItem>> _listStrList,  Hashtable _hashFrequentItemLinkTable, Hashtable _indexToId)
        {
            //ð������,����Ƶ����б�      
            int count = _hashFrequentItemTable.Count;
            for (int bubble = 0; bubble < count; bubble++)
            {
                for (int lookup = bubble + 1; lookup < count; lookup++)
                {
                    //С�ļ���������
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
            //��ɾ������,���õݼ�
            for (int index = _listStrList.Count-1; index >-1; index--)            
            {
                List<TreeNodeItem> _arrStrValue = _listStrList[index];

                for (int i = _arrStrValue.Count - 1; i > -1; i--)
                {
                    TreeNodeItem _itemInfo = _arrStrValue[i];
                    if (_hashFrequentItemTable.ContainsKey(_itemInfo.ItemName))                    
                        //���index������
                        _itemInfo.Index = Convert.ToInt32(_idToIndex[_itemInfo.ItemName]);                    
                    else                    
                        _arrStrValue.RemoveAt(i);                    
                }
                //���������,�ﵽ��С֧�ֶȼ���������
                if (_arrStrValue.Count == 0)
                {
                    _listStrList.RemoveAt(index);
                    continue;
                }

                //ð������            
                count = _arrStrValue.Count;
                for (int bubble = 0; bubble < count; bubble++)
                {
                    for (int lookup = bubble + 1; lookup < count; lookup++)
                    {
                        //С�ļ���������,��ͷ����IndexԽ��,����ִ���ԽС                             
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
        /// ������һ�����
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
        /// �ݹ齨��FPTree
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
        /// ��������FPTree
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
        /// ����������ȡ��ͷ���Header
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
        /// ������е����ݼ�
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
