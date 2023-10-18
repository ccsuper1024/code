#include <iostream>
#include <google/protobuf/compiler/plugin.h>
#include "student.pb.h"
 
 
int main(int argc,char** argv) {
    std::string data_str;
    {
        //使PB 进行序列化，并将结果打印出来。
        //创建对象
        student::StudentInfo _student;
        _student.set_name("小豪豪");
        _student.set_ismale(true);
        _student.set_age(20);

        if (!_student.SerializeToString(&data_str)) {
            std::cerr << "序列化_student失败！" << std::endl;
            return -1;
        }
        std::cout << "序列化_student成功，结果：" << data_str << std::endl;
    }

    {
        // 使用PB 进行反序列，解析出联系人信息并打印出来。
        student::StudentInfo _student;
        if (!_student.ParseFromString(data_str)) {
            std::cerr << "反序列化失败！" << std::endl;
            return -1;
        }
        std::cout << "反序列化成功！" << std::endl;
        std::cout << "name:" << _student.name() << std::endl;
        std::cout << "ismale:" << _student.ismale() << std::endl;
        std::cout << "age:" << _student.age() << std::endl;
    }

    return 0;
}

