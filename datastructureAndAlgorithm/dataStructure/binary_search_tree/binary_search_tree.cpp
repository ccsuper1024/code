/**
 * @brief A simple tree implementation using structured notes
 * BST是二叉搜索树不会再删除和插入后保证树的平衡。BBT(balance binary tree)平衡二叉树才会保证树的平衡。
 * 但是BBT的调整平衡操作会消耗大量的运算时间，所以为了即保证平衡有不消耗太多的运算时间出现了红黑树，要理解
 * 红黑树要先学2-3-4树。
 * 
*/
#include <iostream>

struct Node
{
    int val;
    Node* left;
    Node* right;
};

struct Queue {
    Node* t[100];
    int front;
    int rear;
};

Queue queue;

void enqueue(Node* n ){
    queue.t[queue.rear++] = n;
}

Node* dequeue() {
    return queue.t[queue.front++];
}

void Insert(Node* n, int x) {
    if (x < n->val) {
        if(n ->left == nullptr) {
            Node* temp = new Node;
            temp->val = x;
            temp->left = nullptr;
            temp->right = nullptr;
            n->left = temp;
        } else {
            Insert(n->left, x);
        }
    } else {
        if (n->right == nullptr) {
            Node* temp = new Node;
            temp->val = x;
            temp->left = nullptr;
            temp->right = nullptr;
            n->right = temp;
        } else {
            Insert(n->right, x);
        }
    }
}

int findMaxInleftST(Node* n) {
    while(n->right != nullptr) {
        n = n->right;
    }
    return n->val;
}

void Remove(Node* p, Node* n, int x) {
    if (n->val == x) {
        if(n->right == nullptr && n->left == nullptr) {
            if(x < p->val) {
                p->right = nullptr;
            } else {
                p->left = nullptr;
            }
        }else if (n->right == nullptr) {
            if (x < p->val) {
                p->right = n->left;
            } else {
                p->left = n->left;
            }
        }else if(n->left == nullptr) {
            if (x < p->val) {
                p->right = n->right;
            } else {
                p->left = n->right;
            }
        }else {
            int y = findMaxInleftST(n->left);
            n->val = y;
            Remove(n, n->left, y);
        }
    }else if(x < n->val) {
        Remove(n, n->left, x);
    }else {
        Remove(n, n->right, x);
    }
}

//层次遍历
void BFT(Node* n) { /*输入根节点*/
    if(n != nullptr) {
        std::cout <<n->val << " " ;
        enqueue(n->left);
        enqueue(n->right);
        BFT(dequeue());
    }
}

//先序遍历
void Pre(Node* n) {
    if (n != nullptr) {
        std::cout << n->val << " ";
        //递归查询
        Pre(n->left);
        Pre(n->right);
    }
}

//中根遍历
void In(Node* n) {
    if(n != nullptr) {
        In(n->left);
        std::cout << n->val << " ";
        In(n->right);
    }
}

//后根遍历
void Post(Node* n) {
    if(n != nullptr) {
        Post(n->left);
        Post(n->right);
        std::cout << n->val << " ";
    }
}

int main() {
    //初始化队列
    queue.front = 0;
    queue.rear = 0;
    int value;
    int ch;
    //初始化一个树节点，这是树的根节点
    Node* root = new Node;
    std::cout << "\n Enter the value of root node: ";
    std::cin >> value;
    root->val = value;
    root->left = nullptr;
    root->right = nullptr;
    do {
        std::cout <<  "\n1. Insert"
                  << "\n2. Delete"
                  << "\n3. Breadth First"
                  << "\n4. Preorder Depth First"
                  << "\n5. Inorder Depth First"
                  << "\n6. Postorder Depth First";
        std::cout << "\n Enter Your Choice: ";
        std::cin >> ch;
        int x;
        switch (ch)
        {
        case 1:
            std::cout << "\Enter the value to be Inserted: ";
            std::cin >> x;
            Insert(root, x);    //插入树节点
            break;      //防止case穿透
         case 2:
            std::cout << "\nEnter the value to be Deleted : ";
            std::cin >> x;
            Remove(root, root, x);  //移除节点
            break;
        case 3:
            BFT(root);      
            break;
        case 4:
            Pre(root);
            break;
        case 5:
            In(root);
            break;
        case 6:
            Post(root);
            break;
        default:
            break;
        }
    }while(ch != 0);

    return 0;
}