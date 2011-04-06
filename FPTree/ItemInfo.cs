using System;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// 事务中的频繁项的描述类
    /// </summary>
    public class ItemInfo
    {
        public ItemInfo(string _itemName, int _index)
        {
            this._itemName = _itemName;
            this._index = _index;
        }

        private int _index = 0;
        /// <summary>
        /// 事务的位置
        /// </summary>
        public int Index
        {
            get { return _index; }
            set { _index = value; }
        }
        private string _itemName;
        /// <summary>
        /// 事务名称
        /// </summary>
        public string ItemName
        {
            get { return _itemName; }
            set { _itemName = value; }
        }
    }
}
