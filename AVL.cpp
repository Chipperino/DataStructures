#include<iostream>
using namespace std; 
class AVL {
    //node contains pointers to left and right children, its own height in the tree, and has its own key
    struct node{
        int key;
        node* leftChild;
        node* rightChild;
        int height;
    };
    //creates root node for use in later functions
    node* root;
    //finds place to insert new node based on comparisons to the root node t (called by public insert method below)
    node* insert(int x, node* t){
        //if tree is empty, create a root with minimum values and return it
        if(t==NULL){
            t=new node;
            t->key=x;
            t->leftChild=NULL;
            t->rightChild=NULL;
            t->height=0;
        }
        else if(x<t->key){
            t->leftChild = insert(x, t->leftChild);
            //checking if balancing required
            if(getHeight(t->leftChild)-getHeight(t->rightChild)==2){
                if(x<t->leftChild->key)
                    t=singleRotationRight(t);
                else
                    t=doubleRotationRight(t);
            }
        }
        else if(x > t->key){
            t->rightChild=insert(x, t->rightChild);
            //checking if balancing required
            if(getHeight(t->rightChild)-getHeight(t->leftChild)==2){
                if(x > t->rightChild->key)
                    t=singleRotationLeft(t);
                else
                    t=doubleRotationLeft(t);
            }
        }
        //tree's height is increased by 1 after insertion; uses whichever subtree's height is larger
        t->height=max(getHeight(t->leftChild),getHeight(t->rightChild))+1;
        return t;
    }
    //recursively traverses the tree from the root to find the key x and removes it
    node* remove(int x, node* t){
        node* temp;
        //Element not found
        if(t==NULL)
            return NULL;
        //Search for element based on comparisons
        else if(x<t->key)
            t->leftChild=remove(x,t->leftChild);
        else if(x>t->key)
            t->rightChild=remove(x,t->rightChild);
        // Element found with 2 children
        else if(t->leftChild&&t->rightChild){
            temp=findMin(t->rightChild);
            t->key=temp->key;
            t->rightChild=remove(t->key,t->rightChild);
        }
        // Element found with one or zero children
        else{
            temp=t;
            if(t->leftChild==NULL)
                t=t->rightChild;
            else if(t->rightChild==NULL)
                t=t->leftChild;
        }
        if(t==NULL)
            return t;
        t->height=max(getHeight(t->leftChild),getHeight(t->rightChild))+1;
        //checking if balance required
        if(getHeight(t->leftChild)-getHeight(t->rightChild)==2){
            if(getHeight(t->leftChild->leftChild)-getHeight(t->leftChild->rightChild)==1)
                return singleRotationLeft(t);
            else
                return doubleRotationLeft(t);
        }
        //checking if balance required
        else if(getHeight(t->rightChild)-getHeight(t->leftChild)==2){
            if(getHeight(t->rightChild->rightChild)-getHeight(t->rightChild->leftChild)==1)
                return singleRotationRight(t);
            else
                return doubleRotationRight(t);
        }
        return t;
    }
    //rotates t->rightChild up to t, t->rightChild->leftChild up to t->rightChild, and shifts t into left subtree, Case4
    node* singleRotationLeft(node* &t){
        node* tr=t->rightChild;
        t->rightChild=tr->leftChild;
        tr->leftChild=t;
        t->height=max(getHeight(t->leftChild),getHeight(t->rightChild))+1;
        tr->height=max(getHeight(t->rightChild),t->height)+1 ;
        return tr;
    }
    //rotates node t->leftChild up to t, t->leftChild->rightChild up to t->leftChild, and shifts t into right subtree, Case1
    node* singleRotationRight(node* &t){
        node* tl=t->leftChild;
        t->leftChild=tl->rightChild;
        tl->rightChild=t;
        t->height=max(getHeight(t->leftChild),getHeight(t->rightChild))+1;
        tl->height=max(getHeight(tl->leftChild),t->height)+1;
        return tl;
    }
    //performs single rotation to the right for t's right child, and then performs single left rotation for t, Case3
    node* doubleRotationLeft(node* &t){
        t->rightChild=singleRotationRight(t->rightChild);
        return singleRotationLeft(t);
    }
    //performs single rotation to the left for t's left child, and then performs single right rotation for t, Case2
    node* doubleRotationRight(node* &t){
        t->leftChild=singleRotationLeft(t->leftChild);
        return singleRotationRight(t);
    }
    //prints out left subtree, then root, then right subtree (inorder traversal)
    void inorderPrint(node* t){
        if(t==NULL)
            return;
        inorderPrint(t->leftChild);
        cout<<t->key<<" ";
        inorderPrint(t->rightChild);
    }
    //goes down the right subtrees until a null pointer is found, and returns the previous non-null node
    node* findMax(node* t){
        if(t==NULL)
            return NULL;
        else if(t->rightChild==NULL)
            return t;
        else
            return findMax(t->rightChild);
    }
    //goes down left subtrees until a null pointer is found, and returns the previous non-null node
    node* findMin(node* t){
        if(t==NULL)
            return NULL;
        else if(t->leftChild==NULL)
            return t;
        else
            return findMin(t->leftChild);
    }
    //if the node is null, then the height is 0. otherwise, use the height of the current node
    int getHeight(node* t){
        if(t==NULL){
            return 0;
        }else{
            return t->height;
        }
    }
    //returns difference in height of sibling trees. rotation needed if value returned is > 1 or <-1
    int getBalance(node* t){
        if(t==NULL)
            return 0;
        else
            return getHeight(t->leftChild)-getHeight(t->rightChild);
    }
    //traverses tree to a null node and deletes it, repeating recursively until tree is null
    void emptyTree(node* t){
        if(t==NULL)
            return;
        emptyTree(t->leftChild);
        emptyTree(t->rightChild);
    }
    
public:
    //constructor case for creating a new tree
    AVL(){
        root=NULL;
    }
    //sends key and root to insert method in AVL class
    void insert(int x){
        root=insert(x, root);
    }
    //sends key to be removed and the root to the remove method in AVL class
    void remove(int x){
        root=remove(x, root);
    }
    //displays the tree using inorder traversal by sending the root to inorder method in AVL class
    void printTree(){
        inorderPrint(root);
        cout<<endl;
    }
};

int main() {
    AVL t;
    int array[15];
    int var;
    //takes input of 15 numbers from user
    for(int i=0; i<sizeof(array)/sizeof(array[0]); i++){
        cout<<"Enter number "<<(i+1)<<" to add to the array (up to 15)"<<endl;
        cin>>var;
        array[i]=var;
    }
    for(int i=0; i<sizeof(array)/sizeof(array[0]); i++){
        t.insert(array[i]);
    }
    t.printTree();
}