using System;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// �����е�Ƶ�����������
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
        /// �����λ��
        /// </summary>
        public int Index
        {
            get { return _index; }
            set { _index = value; }
        }
        private string _itemName;
        /// <summary>
        /// ��������
        /// </summary>
        public string ItemName
        {
            get { return _itemName; }
            set { _itemName = value; }
        }
    }
}
