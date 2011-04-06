using System;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// 项集实体
    /// </summary>
    public class ItemSet
    {
        private int _count;
        /// <summary>
        /// 项集的大小
        /// </summary>
        public int Count
        {
            get { return _count; }
            set { _count = value; }
        }
        private List<string> _content = new List<string>();
        /// <summary>
        /// 项集内容
        /// </summary>
        public List<string> Content
        {
            get { return _content; }
            set { _content = value; }
        }
        /// <summary>
        /// 返回所有内容
        /// </summary>
        /// <returns></returns>
        public string GetAllContent()
        {
            string _allContent = "";
            foreach (string _subContent in _content)
            {
                _allContent += _subContent + " ";
            }
            return _allContent;
        }
    }
}
