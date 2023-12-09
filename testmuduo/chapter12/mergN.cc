/* @author chenchao 
@date 
@file 
@brief 用堆排序进行多路归并。
       题目：用一台4GB的机器对磁盘上的单个100GB磁盘文件排序
       思路：标准思路是先分块排序，再多路归并成一个文件。多路归并很容易用堆排序实现。
       先对100GB文件分块，对每个块进行排序。再构造一个内存中的堆结构，堆元素为
       std::pair<Record,FILE*>，再进行归并。
       实例：把100GB文件分成32块，并对32块的磁盘文件排序。再构造一个32个堆元素为
       std::pair<Record,FILE*>的堆。然后每次取堆顶元素，将其Record写入输出文件；
       如果FILE* 还可读，就读入一条Record，再想heap中添加std::pair<Record,FILE*>.
       这样，当堆空时，多路归并就完成了
       这种方法比传统的二路归并要快，因为它节省了很多的磁盘读写。如果用教科书上的二路归并
       算法，先读一遍32个文件，两两归并为16个文件，再读一遍这16个文件，两两归并为8个文件
       如此往复，最后归并一个文件，输出
*/
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

typedef int Record;
typedef std::vector<Record> File;

struct Input
{
    Record _value;
    size_t _index;
    const File* _file;

    explicit Input(const File* f)
    :_value(-1),
    _index(0),
    _file(f)
    { }

    bool next() {
        if(_index < _file->size()) {
            _value =  (*_file)[_index];
            ++_index;
            return true;
        }else {
            return false;
        }
    }

    bool operator<(const Input& rhs) const {  //const 成员函数
        //make_heap to build min-heap, for merging
        return _value > rhs._value;
    }

}; //end of struct Input

File mergeN(const std::vector<File>& files) {
    File output;
    std::vector<Input> inputs;

    //将数据读入inputs中
    for(size_t i =0; i < files.size(); ++i) {
        Input input(&files[i]);
        if(input.next()) {
            inputs.push_back(input);
        }
    }
    //建堆
    std::make_heap(inputs.begin(),inputs.end());
    while(!inputs.empty()) {
        std::pop_heap(inputs.begin(),inputs.end());
        output.push_back(inputs.back()._value);

        if(inputs.back().next()) {
            std::push_heap(inputs.begin(),inputs.end());
        }else {
            inputs.pop_back();
        }
    }

    return output;
}

/* @brief @pram @return */
void test(){
    const int kFiles = 32;
    std::vector<File> files(kFiles);
    for(int i = 0 ; i < kFiles ; ++i) {
        File file(rand() % 1000);
        //将相继的函数调用结果赋值给一个范围内的每一个元素
        std::generate<File::iterator, int (*)()>(file.begin(),file.end(),rand);
        std::sort(file.begin(),file.end());
        files[i].swap(file);
    }

    File output = mergeN(files);

    std::copy(output.begin(),output.end(),
        std::ostream_iterator<Record>(std::cout,"\n"));
}
int main()
{
    test();
    return 0;
}

