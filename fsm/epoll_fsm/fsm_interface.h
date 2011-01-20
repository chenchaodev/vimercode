/*=============================================================================
#  Author:          dantezhu - http://www.vimer.cn
#  Email:           zny2008@gmail.com
#  FileName:        interfaces.h
#  Description:     公共接口
#  Version:         1.0
#  LastChange:      2011-01-19 23:24:33
#  History:         
=============================================================================*/
#ifndef _INTERFACES_H_
#define _INTERFACES_H_
class IActor
{
public:
    virtual ~IActor() {}
    virtual int ChangeState(int destState)=0;
};


class IFsm
{
public:
    virtual ~IFsm() {}
    /**
     * @brief   在进入这个状态的时候，obj需要做的事情
     *
     * @param   obj
     *
     * @return  0           succ
     *          else        fail
     */
    virtual int Init(IActor* obj)=0;

    /**
     * @brief   执行这个状态该做的事情
     *
     * @param   obj
     *
     * @return  应该进入的另一个状态
     *          
     */
    virtual int Process(IActor* obj)=0;

    /**
     * @brief   退出这个状态时需要做的事情
     *
     * @param   obj
     *
     * @return  0           succ
     *          else        fail
     */
    virtual int Fini(IActor* obj)=0;
};

#endif
