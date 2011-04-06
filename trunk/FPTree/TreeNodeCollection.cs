using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{    
    /// <summary>
    /// 树结点集合
    /// </summary>
    public class TreeNodeItemCollection : CollectionBase
    {
        private TreeNodeItem _parent = null;
        public TreeNodeItemCollection(TreeNodeItem _parent)
            : base()
        {
            this._parent = _parent;
        }

        #region 属性
        private bool isEmpty = true;

        public bool IsEmpty
        {
            get
            {
                return this.isEmpty;
            }
        }
        #endregion

        #region 索引
        public TreeNodeItem this[int index]
        {
            get
            {
                return (TreeNodeItem)base.InnerList[index];
            }
        }
        #endregion


        #region 方法
        public void Add(TreeNodeItem _item)
        {
            _item.Parent = _parent;
            base.InnerList.Add(_item);
            this.isEmpty = false;
        }

        public void AddRange(TreeNodeItem[] _arrItem)
        {
            foreach (TreeNodeItem _item in _arrItem)
            {
                _item.Parent = _parent;
            }
            base.InnerList.AddRange(_arrItem);
            this.isEmpty = false;
        }

        public void Remove(TreeNodeItem _item)
        {
            base.InnerList.Remove(_item);
        }

        public void Remove(int index)
        {
            base.InnerList.RemoveAt(index);
        }

        public void CopyTo(TreeNodeItem[] _arrItem)
        {
            this.InnerList.CopyTo(_arrItem);
        }

        public void CopyTo(TreeNodeItem[] _arrItem, int arrayIndex)
        {
            this.InnerList.CopyTo(_arrItem, arrayIndex);
        }
        #endregion
    }
}
