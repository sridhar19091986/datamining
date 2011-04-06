using System;

using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// Ƶ��һ�м��б���ʹ�õ�Ƶ����
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
        /// ��Ƶ����ļ���
        /// </summary>
        public int Count
        {
            get { return _count; }
            set { _count = value; }
        }

        private string _itemName;
        /// <summary>
        /// ��Ƶ���������
        /// </summary>
        public string ItemName
        {
            get { return _itemName; }
            set { _itemName = value; }
        }

        private TreeNodeItem _nextNode;
        /// <summary>
        /// ָ���һ��ͬ���Ľ��
        /// </summary>
        public TreeNodeItem NextNode
        {
            get { return _nextNode; }
            set { _nextNode = value; }
        }        
        
    }
}
