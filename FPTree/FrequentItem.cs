using System;

using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// 频繁一列集列表中使用的频繁项
    /// </summary>
    public class FrequentItem
    {
        public FrequentItem(string _itemName)
        {
            this._itemName = _itemName;
            this._count = 0;
        }

        public FrequentItem(string _itemName, int _count)
        {
            this._itemName = _itemName;
            this._count = _count;
        }

        private int _count = 0;
        /// <summary>
        /// 该频繁项的计数
        /// </summary>
        public int Count
        {
            get { return _count; }
            set { _count = value; }
        }

        private string _itemName;
        /// <summary>
        /// 该频繁项的名称
        /// </summary>
        public string ItemName
        {
            get { return _itemName; }
            set { _itemName = value; }
        }

        private TreeNodeItem _nextNode;
        /// <summary>
        /// 指向第一个同名的结点
        /// </summary>
        public TreeNodeItem NextNode
        {
            get { return _nextNode; }
            set { _nextNode = value; }
        }        
        
    }
}
