/*  @file @brief @author chenchao @brief */
#ifndef  _TASK_HPP
#define  _TASK_HPP
class Task
{
public:
/* @file @brief @pram*/
    Task()=default;
    //纯虚函数
    virtual void process()=0;
    ~Task() {}

private:

};
#endif  //_TASK_HPP

