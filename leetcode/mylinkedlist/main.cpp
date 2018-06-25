/*************************************************************************
	> File Name: main.cpp
	> Author: Zhu Brocade Soar
	> Mail: zhujinteng2012@163.com
	> Created Time: 2018年06月13日 星期三 16时51分31秒
 ************************************************************************/

#include<iostream>
using namespace std;

class MyLinkedList{
    private :
    // l[addr][0] val
    // l[addr][1] next
    // l[addr][1] == -1 tail flag
    // l[addr][1] == -2 void flag
    int l[1000][2];
    // head == -1 empty list
    int head;

    int findSpace(){
        for(int i = 0; i < 1000; i++){
            if(this->l[i][1] == -2) return i;
        }
        return -1; // no more space
    }

    int findTail(){
        for(int i = 0; i < 1000; i++){
            if(this->l[i][1] == -1) return i;
        }
        return -1; // empty list
    }

    int findTailIndex(){
        int count = 0;
        for(int i = 0; i < 1000; i++){
            count++;
            if(this->l[i][1] == -1) return count;
        }
        return -1; // empty list
    }

    int findPrev(int index){
        if(this->head == -1 || index < 1 || index > 1000){
            return -1;
        }else{
            int curr = this->head;
            int count = 1;
            while((index - 1) > count++){
                if(this->l[curr][1] == -1) return -1;
                curr = l[curr][1];
            }
            return curr;
        }
    }

    int findCurr(int index){
        if(this->head == -1 || index < 1 || index > 1000){
            return -1;
        }else{
            int curr = this->head;
            int count = 1;
            while((index) > count++){
                if(this->l[curr][1] == -1) return -1;
                curr = l[curr][1];
            }
            return curr;
        }
    }

    int findNext(int index){
        if(this->head == -1 || index < 1 || index > 1000){
            return -1;
        }else{
            int curr = this->head;
            int count = 1;
            while((index + 1) > count++){
                if(this->l[curr][1] == -1) return -1;
                curr = l[curr][1];
            }
            return curr;
        }
    }

    public :
    MyLinkedList(){
        for(int i = 0; i < 1000; i++){
            // set all space to void
            this->l[i][1] = -2;
            // set list state to empty
            this->head = -1;
        }
    }

    int get(int index){
        if(this->head == -1 || index < 1 || index > 1000){
            return -1;
        }else{
            int curr = this->head;
            int count = 1;
            while(index > count++){
                if(this->l[curr][1] == -1) return -1;
                curr = l[curr][1];
            }
            return this->l[curr][0];
        }
    }

    bool addAtHead(int val){
        if(this->head == -1){
            // empty list
            this->l[0][0] = val;
            this->l[0][1] = -1;
            this->head = 0;
        }else{
            int newid = this->findSpace();
            if(newid == -1) return false;
            this->l[newid][0] = val;
            this->l[newid][1] = this->head;
            this->head = newid;
        }
        return true;
    }

    bool addAtTail(int val){
        if(this->head == -1) this->addAtHead(val);
        else{
            int newid = this->findSpace();
            if(newid == -1) return false;
            this->l[newid][0] = val;
            this->l[newid][1] = -1;
            this->l[this->findTail()][1] = newid;
        }
        return true;
    }

    bool addAtIndex(int index, int val){
        if(this->head == -1){
            if(index != 1) return false;
            else this->addAtHead(val);
        }else{
            int tail = this->findTail();
            if(index < 1 || index > 1000 || index > this->findTailIndex()) return false;
            else{
                int newid = this->findSpace();
                if(newid == -1) return false;
                else{
                    if(index == 1){
                        int curr = this->findCurr(index);
                        if(curr == -1) return false;
                        this->head = newid;
                        this->l[newid][0] = val;
                        this->l[newid][1] = curr;
                    }else{
                        int prev = this->findPrev(index);
                        if(prev == -1) return false;
                        int curr = this->findCurr(index);
                        if(curr == -1) return false;
                        this->l[prev][1] = newid;
                        this->l[newid][0] = val;
                        this->l[newid][1] = curr;
                    }
                }
            }
        }
        return true;
    }

    bool deleteAtIndex(int index){
        if(this->head == -1){
            return false;
        }else{
            int tail = this->findTail();
            if(index < 1 || index > 1000 || index > tail) return false;
            else{
                int prev = this->findPrev(index);
                if(prev == -1) return false;
                int curr = this->findCurr(index);
                // if(curr == -1) return false;
                int next = this->findNext(index);
                this->l[prev][1] = next;
                this->l[curr][1] = -1;
            }
        }
        return true;
    }

};

int main(void){
    MyLinkedList linkedList;
    /*
    linkedList.addAtHead(10);
    linkedList.addAtHead(5);
    linkedList.addAtTail(15);
    linkedList.addAtHead(1);
    linkedList.addAtTail(2);
    linkedList.addAtIndex(3, 100);
    linkedList.addAtIndex(1, 50);
    linkedList.addAtIndex(9, 100);
    linkedList.addAtTail(12);
    linkedList.addAtTail(13);
    linkedList.addAtTail(14);
    linkedList.addAtIndex(10, 15);
    linkedList.addAtIndex(12, 16);
    linkedList.addAtHead(99);
    */

    cout << linkedList.addAtHead(1) << endl;
    cout << linkedList.addAtTail(3) << endl;
    cout << linkedList.addAtIndex(1, 2) << endl;
    cout << linkedList.get(1) << endl;
    // cout << linkedList.deleteAtIndex(1) << endl;
    // cout << linkedList.get(1) << endl;

    cout << "List:" << endl;
    int val;
    int i = 1;
    cout << "HEAD->";
    while((val = linkedList.get(i)) != -1){
        cout << val << "->";
        if(++i == 1000) break;
    }
    cout << "TAIL" << endl;

    return 0;
}

