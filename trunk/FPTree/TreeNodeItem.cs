using System;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// FpTree��������ʵ������
    /// </summary>
    public class TreeNodeItem
    {
        public TreeNodeItem()
        {
            _nodes = new TreeNodeItemCollection(this);
        }
        public TreeNodeItem(string _itemName, int _count)
            :this()
        {
            this._itemName = _itemName;
            this._count = _count;
        }
        public TreeNodeItem(TreeNodeItem _treeNodeItem)
            : this()
        {
            this._itemName = _treeNodeItem.ItemName;
            this._count = _treeNodeItem.Count;
        }
        private int _index = 0;
        /// <summary>
        /// ��������
        /// </summary>
        public int Index
        {
            get { return _index; }
            set { _index = value; }
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

        private TreeNodeItemCollection _nodes = null;
        /// <summary>
        /// �ӽ�㼯��
        /// </summary>
        public TreeNodeItemCollection Nodes
        {
            get { return _nodes; }
            set { _nodes = value; }
        }

        private TreeNodeItem _nextNode;
        /// <summary>
        /// ָ����һ��ͬ���Ľ��
        /// </summary>
        public TreeNodeItem NextNode
        {
            get { return _nextNode; }
            set { _nextNode = value; }
        }
        private TreeNodeItem _parent;
        /// <summary>
        /// ָ�򸸽��
        /// </summary>
        public TreeNodeItem Parent
        {
            get { return _parent; }
            set { _parent = value; }
        }
        /// <summary>
        /// ��������
        /// ʹItemName���,Count���,Parent���
        /// </summary>
        /// <returns></returns>
        public TreeNodeItem CloneNode()
        {
            TreeNodeItem _newNode = new TreeNodeItem();
            _newNode.ItemName = this.ItemName;
            _newNode.Count = this.Count;
            _newNode.Parent = this.Parent;
            return _newNode;
        }       
    }
}
