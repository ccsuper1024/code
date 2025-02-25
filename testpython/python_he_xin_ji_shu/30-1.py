import unittest

#将要被测试的排序函数
def sort(arr):
    l = len(arr)
    for i in range(0, l):
        for j in range(i + 1, l):
            if arr[i] >= arr[j]:
                tmp = arr[i]
                arr[i] = arr[j]
                arr[j] = tmp


#编写子类继承unittest.TestCase
class TestSort(unittest.TestCase):

    #以test开头的函数将会被测试
    def test_sort(self):
        arr = [3, 4, 1, 5 , 6]
        sort(arr)
        #assert 结果
        self.assertEqual(arr, [1,3,4,5,6])

if __name__ == '__main__':
    ##如果在jupyter或者Ipython中用如下方式测试
    #unittest.main(argv=['first-arg-is-ignored'], exit=False)
    
    ##如果以命令行方式运行使用如下方式测试
    unittest.main()