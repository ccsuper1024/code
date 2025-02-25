/*
    @file skip_list.cpp
    @brief Data structure for fast searching and insertion in \f$O(\log n)\f$
    time
    @details
    A skip list is a data structure that is used for storing a sorted list of items
    with a help of hierarchy of linked lists that connect increasingly sparese
    subsequences of the items

    References used: [GeeksForGeek](https://www.geeksforgeeks.org/skip-list/),
 * [OpenGenus](https://iq.opengenus.org/skip-list) for PseudoCode and Code
 * @author [enqidu](https://github.com/enqidu)
 * @author [Krishna Vedala](https://github.com/kvedala)
*/
#include <cstring>
#include <ctime>
#include <array>
#include <vector>
#include <iostream>
#include <memory>

namespace data_structures
{
    constexpr int MAX_LEVEL = 2;       // Maximum level of skip list
    constexpr float PROBABILITY = 0.5; // Current probablity for coin toss //抛硬币概率

    /*
        Node structure [Key][Node*, Node*,...]
    */
    struct Node
    {
        int key_;                                    // key integer
        void *value_;                                // pointer of value
        std::vector<std::shared_ptr<Node>> forward_; // nodes of the given one in all levels

        /*
            creates node with provided key, level and value
            @param key is number that is used for comparision
            @param level is the maximum level node's going to added
        */
        Node(int key, int level, void *value = nullptr)
            : key_(key),
              value_()
        {
            for (int i = 0; i < level + 1; ++i)
            {
                forward_.push_back(nullptr);
            }
        }
    };

    /*
         SkipList class implemetation with basic methods
    */
    class SkipList
    {
    private:
        int level_ ;                    // Maximum level of the skiplist
        std::shared_ptr<Node> header_; // Pointer to the header node

    public:
        /*
        Skip list constructor. Initializes header, start
        Node for searching in the list
   */
        SkipList()
            : level_(0)
        {
            fprintf(stdout, "SkipList()\n");
            header_ = std::make_shared<Node>(-1, MAX_LEVEL);    //??
        }
        /**
         * Returns random level of the skip list.
         * Every higher level is 2 times less likely.
         * @return random level for skip list
         */
        int randomLevel()
        {
            int lvl = 0;
            while (static_cast<float>(std::rand()) / RAND_MAX < PROBABILITY &&
                   lvl < MAX_LEVEL)
            {
                ++lvl;
            }
            return lvl;
        }

        /**
         * Inserts elements with given key and value;
         * It's level is computed by randomLevl() function
         * @param key is number that is used for comparison
         * @param value pointer to a value, that can be any type
         */
        void insertElement(int key, void *value)
        {
            std::cout << "Inserting: " << key << "...";
            std::shared_ptr<Node> x = header_;  //获取跳表头节点的地址，shared_ptr对象共享底层，修改会发生同步
            std::array<std::shared_ptr<Node>, MAX_LEVEL + 1> update;
            update.fill(nullptr);

            for (int i = level_; i >= 0; --i)
            {
                //当跳表两个节点都不为空且跳表节点的键值小于输入的key时
                while (x->forward_[i] != nullptr && x->forward_[i]->key_ < key)
                {
                    x = x->forward_[i]; //移动指针。
                }
                update[i] = x;  //找到跳表节点的指针为空且其key 小于输入的key的节点 跳表是基于有序链表的
            }

            x = x->forward_[0];

            bool doesnt_exist = (x == nullptr || x->key_ != key);

            if (doesnt_exist)
            {
                int rlevel = randomLevel();

                if (rlevel > level_)
                {
                    for (int i = level_ + 1; i < rlevel + 1; ++i)
                    {
                        update[i] = header_;
                    }
                    //update current level
                    level_ = rlevel;
                }
                std::shared_ptr<Node> n = std::make_shared<Node>(key, rlevel, value);
                for (int i = 0; i <= rlevel; ++i)
                {
                    n->forward_[i] = update[i]->forward_[i];
                    update[i]->forward_[i] = n;
                }
                std::cout << "Inserted" << std::endl;
            } else {
                std::cout << "Exists" << std::endl;
            }
        }

        /**
         * Deletes an element by key and prints if has been removed successfully
         * @param key is number that is used for comparison.
         */
        void deleteElement(int key)
        {
            std::shared_ptr<Node> x = header_;
            std::array<std::shared_ptr<Node>, MAX_LEVEL + 1> update;
            update.fill(nullptr);

            for (int i = level_; i >= 0; --i)
            {
                while (x->forward_[i] != nullptr && x->forward_[i]->key_ < key)
                {
                    x = x->forward_[i]; //移动指针
                }
                update[i] = x;          //更新update中shared_ptr元素指向的位置
            }

            x = x->forward_[0];

            bool doesnt_exist = (x == nullptr || x->key_ != key);

            if(!doesnt_exist) {
                for(int i = 0; i <= level_; ++i) {
                    if (update[i]->forward_[i] != x) {
                        break;
                    }
                    update[i]->forward_[i] = x->forward_[i];
                }

                /* Remove empty levels*/
                while(level_ > 0 && header_->forward_[level_] == nullptr)
                    --level_;
                std::cout << "Deleted" << std::endl;
            }else {
                std::cout << "Doesn't exist" << std::endl;
            }
        }

        /**
         * Searching element in skip list structure
         * @param key is number that is used for comparision
         * @return pointer to the value of the node
        */
       void* searchElement(int key) {
            std::shared_ptr<Node> x = header_;
            std::cout << "Searching for" << key << std::endl;

            for (int i = level_; i >=0; --i) {
                while (x->forward_[i] && x->forward_[i]->key_ < key) 
                {
                    /* code */
                    x = x->forward_[i];
                }
                
            }
            x = x->forward_[0];
            if(x && x->key_== key) {
                std::cout << "Found " << std::endl;
                return x->value_;
            } else {
                std::cout << "Not Found" << std::endl;
                return nullptr;
            }
       }

       /**
        * Display skip list level
       */
      void displayList() {
            std::cout << "Displaying list:\n";
            for(int i = 0; i <= level_; ++i) {
                std::shared_ptr<Node> node =header_->forward_[i];
                std::cout <<"Level " << i << ": ";
                while (node != nullptr)
                {
                    std::cout << node->key_ << " ";
                    node = node->forward_[i]; //移动指针
                }
                std::cout << std::endl;
            }
      }
    };
}   //namespace data_structure

/**
 * Main function
 * Creates and inserts random 2^[number of levels]
 * elements into the skip lists and than display it 
*/

int main() {
    std::srand(std::time(nullptr));

    data_structures::SkipList skipList;

    for(int j = 0; j < (1 << (data_structures::MAX_LEVEL + 1)); ++j) {
        int k = (std::rand() % ( 1 << (data_structures::MAX_LEVEL + 2)) + 1);
        skipList.insertElement(k, &j);
    }

    skipList.displayList();

    return 0;
}