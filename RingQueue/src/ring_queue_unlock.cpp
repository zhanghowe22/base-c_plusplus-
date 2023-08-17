#include <iostream>
#include <cstring>
#include "ring_queue_unlock.h"

using namespace std;

int main()
{
    cout << "Test ring queue" << endl;
    cout <<"--------------------" << endl;

/*
// test
*/
    // Ring_Queue<int> queue(5);
    // int num[5] = {};
    // for(int i = 0; i < 2; i++) {
    //     num[i] = i + 2;
    // }

    // for(int j = 0; j < 5; j++) {
    //     cout << num[j] << " ";
    // }
    // cout << endl;

    // queue.ring_push(num, 2);

    Ring_Queue<char> queue(6);

    string str = "hello";

    const char* ptr = str.c_str();

    size_t push_len = strlen(ptr);

    queue.ring_push(ptr, push_len);

    string str1 = "world";

    const char* ptr1 = str1.c_str();

    size_t push_len1 = strlen(ptr1);

    queue.ring_push(ptr1, push_len1);
//

    // while (!queue.ring_is_empty())
    // {
    //     cout << "Size = " << queue.ring_size() << endl;
    //     cout << "Front = " << queue.ring_front() << endl;
    //     cout << "Rear = " << queue.ring_rear() << endl;
    //     cout << "*************" << endl;
    //     queue.ring_pop();
    // }
    
    return 0;
}