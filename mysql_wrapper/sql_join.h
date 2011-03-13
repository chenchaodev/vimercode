/*=============================================================================
#
#     FileName: sql_join.h
#         Desc: sql语句的拼装器
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-03-12 19:55:42
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-03-12 19:55:42 | initialization
#
=============================================================================*/
#ifndef __SQL_JOIN_H__
#define __SQL_JOIN_H__
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

class SQLPair
{
public:
    /**
     * @brief   SQLPair的构造函数，支持各种数据类型
     *
     * @param   key         列名
     * @param   value       列值
     */
    template <typename T> 
    SQLPair (const string& key, const T& value)
    {
        m_ss_value.clear();
        m_ss_value.str("");
        _init(key,value);
    }

    /**
     * @brief   拷贝构造函数
     *
     * @param   pair_data   被copy对象
     */
    SQLPair(const SQLPair& pair_data)
    {
        m_ss_value.clear();
        m_ss_value.str("");
        m_key = pair_data.m_key;
        m_ss_value << pair_data.m_ss_value.str();
    }
    /**
     * @brief   赋值操作符
     *
     * @param   pair_data   等号右边的对象
     *
     * @return  返回自身引用
     */
    SQLPair& operator=(const SQLPair& pair_data)
    {
        m_ss_value.clear();
        m_ss_value.str("");
        m_key = pair_data.m_key;
        m_ss_value << pair_data.m_ss_value.str();
        return *this;
    }

    /**
     * @brief   返回列名
     *
     * @return  列名
     */
    string key()const
    {
        return m_key;
    }

    /**
     * @brief   返回string类型的列值
     *
     * @return  列值
     */
    string value()const
    {
        return m_ss_value.str();
    }

    /**
     * @brief   返回列名=列值的pair
     *
     * @return  列名=列值的pair
     */
    string pair()const
    {
        stringstream ss_pair;
        ss_pair << m_key
            << "="
            << m_ss_value.str();
        return ss_pair.str();
    }

    virtual ~SQLPair() {}

private:
    /**
     * @brief   实际初始化函数，特殊处理了列值为char*类型（判断NULL，并且在列值两边加上'）
     *
     * @param   key         列名   
     * @param   value       列值
     *
     * @return  0
     */
    int _init(const string& key, const char* &value)
    {
        m_key = key;
        if (NULL == value)
        {
            //这真没办法了，否则就会变成0，所以不要传NULL
            m_ss_value << "''";
        }
        else
        {
            m_ss_value << "'" << value << "'";
        }
        return 0;
    }
    /**
     * @brief   实际初始化函数，特殊处理了列值为string类型（在列值两边加上'）
     *
     * @param   key         列名   
     * @param   value       列值
     *
     * @return  0
     */
    int _init(const string& key, const string& value)
    {
        m_key = key;
        m_ss_value << "'" << value << "'";
        return 0;
    }
    /**
     * @brief   实际初始化函数，模版化处理。
     *          在内部实现的好处是外边不会用<string>强制调到不应该调用的处理string、char*的函数
     *
     * @param   key         列名   
     * @param   value       列值
     *
     * @return  0
     */
    template <typename T> 
    int _init(const string& key, const T& value)
    {
        m_key = key;
        m_ss_value << value;
        return 0;
    }

private:
    //列名
    string m_key;
    //转化后的列值
    stringstream m_ss_value;
};

class SQLJoin
{
private:
    //用来查找是否有重名key的
    //类内部可见即可
    class MYSQLEqualKey
    {
    public:
        MYSQLEqualKey(const string& key) {
            m_key = key;
        }

        /**
         * @brief   用来实现判断是否相等
         *
         * @param   pair_data       vector中的每个item
         *
         * @return  true            相等
         *          false           不相等
         */
        bool operator() (const vector<SQLPair>::value_type &pair_data)
        {
            return m_key == pair_data.key();
        }
    private:
        string m_key;
    };
public:
    SQLJoin() {}
    virtual ~SQLJoin() {}

    /**
     * @brief   添加一个列名
     *
     * @param   key         列名
     *
     * @return  0
     */
    int AddPair(const string& key)
    {
        return AddPair(SQLPair(key,""));
    }

    /**
     * @brief   添加一个列名-列值pair
     *
     * @param   key         列名
     * @param   value       列值
     *
     * @return  0
     */
    template <typename T> 
    int AddPair(const string& key, const T& value)
    {
        return AddPair(SQLPair(key,value));
    }

    /**
     * @brief   添加一个SQLPair对象
     *
     * @param   pair_data   SQLPair对象
     *
     * @return  0
     */
    int AddPair(const SQLPair& pair_data)
    {
        vector<SQLPair>::iterator findit = find_if(m_vecPairs.begin(),
                                                   m_vecPairs.end(),
                                                   MYSQLEqualKey(pair_data.key())
                                                  );
        if (m_vecPairs.end() == findit)
        {
            m_vecPairs.push_back(pair_data);
        }
        else
        {
            *findit = pair_data;
        }
        return 0;
    }

    /**
     * @brief   用流处理的方式，添加一个列名
     *
     * @param   key         列名
     *
     * @return  0
     */
    SQLJoin& operator << (const string& key)
    {
        AddPair(key);
        return *this;
    }

    /**
     * @brief   用流处理的方式，添加一个SQLPair对象
     *
     * @param   pair_data       SQLPair对象
     *
     * @return  0
     */
    SQLJoin& operator << (const SQLPair& pair_data)
    {
        AddPair(pair_data);
        return *this;
    }

    /**
     * @brief   输出所有列名（如name, sex, age）
     *
     * @return  所有列名
     */
    string keys()
    {
        stringstream ss;
        ss << " ";
        for(vector<SQLPair>::iterator it = m_vecPairs.begin(); it != m_vecPairs.end(); ++it)
        {
            if (it != m_vecPairs.begin())
            {
                ss << ",";
            }
            ss << it->key();
        }
        ss << " ";

        return ss.str();
    }

    /**
     * @brief   输出所有列值（如'dante', 1, 25）
     *
     * @return  所有列值
     */
    string values()
    {
        stringstream ss;
        ss << " ";
        for(vector<SQLPair>::iterator it = m_vecPairs.begin(); it != m_vecPairs.end(); ++it)
        {
            if (it != m_vecPairs.begin())
            {
                ss << ",";
            }
            ss << it->value();
        }
        ss << " ";

        return ss.str();
    }

    /**
     * @brief   输入所有列名-列值，并用指定分隔符分割（如name='dante', sex=1, age=25）
     *
     * @param   split_str   分割符，默认是用','，也可以用and、or之类
     *
     * @return  所有列名-列值
     */
    string pairs(const string& split_str = ",")
    {
        stringstream ss;
        ss << " ";
        for(vector<SQLPair>::iterator it = m_vecPairs.begin(); it != m_vecPairs.end(); ++it)
        {
            if (it != m_vecPairs.begin())
            {
                ss << " " << split_str << " ";
            }
            ss << it->pair();
        }
        ss << " ";

        return ss.str();
    }

    /**
     * @brief   清空所有数据
     */
    void clear()
    {
        m_vecPairs.clear();
    }

private:
    vector<SQLPair> m_vecPairs;
};
#endif
