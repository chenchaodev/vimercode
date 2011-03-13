/*=============================================================================
#
#     FileName: test_join.cpp
#         Desc:
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-03-12 20:42:45
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-03-12 20:42:45 | initialization
#
=============================================================================*/

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "sql_join.h"
using namespace std;
int main(int argc, char **argv)
{
    SQLJoin sql_join;
    sql_join.AddPair(SQLPair("name","dandan"));
    sql_join.AddPair(SQLPair("sex",1));
    sql_join.AddPair(SQLPair("high",1.1));
    sql_join.AddPair(SQLPair("high",true));

    typedef SQLPair P;
    sql_join
        << P("what","go")
        << P("iend",false);

    cout << sql_join.keys()
        << endl
        << sql_join.values()
        << endl
        << sql_join.pairs();

    return 0;
}
