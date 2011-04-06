using System;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// ∆µ∑±œÓ¡¥Ω”
    /// </summary>
    public class FrequentLinkItem
    {
        public FrequentLinkItem(int _index, TreeNodeItem _linkNode)
        {
            this._index = _index;
            this._linkNode = _linkNode;
        }

        private int _index;

        public int Index
        {
            get { return _index; }
            set { _index = value; }
        }
        private TreeNodeItem _linkNode;

        public TreeNodeItem LinkNode
        {
            get { return _linkNode; }
            set { _linkNode = value; }
        }
    }
}
