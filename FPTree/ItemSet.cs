using System;
using System.Collections.Generic;
using System.Text;

namespace FPTree
{
    /// <summary>
    /// �ʵ��
    /// </summary>
    public class ItemSet
    {
        private int _count;
        /// <summary>
        /// ��Ĵ�С
        /// </summary>
        public int Count
        {
            get { return _count; }
            set { _count = value; }
        }
        private List<string> _content = new List<string>();
        /// <summary>
        /// �����
        /// </summary>
        public List<string> Content
        {
            get { return _content; }
            set { _content = value; }
        }
        /// <summary>
        /// ������������
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
