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
    Ring_Queue<char> queue(8);

    string str = "hifangzi";

    const char* ptr = str.c_str();

    size_t push_len = strlen(ptr);

    int rc = 0;
    rc = queue.ring_push(ptr, push_len);

    string str1 = "worldiamcome";

    const char* ptr1 = str1.c_str();

    size_t push_len1 = strlen(ptr1);

    rc = queue.ring_push(ptr1, push_len1);

    char buf1[10] = {};
    rc = queue.ring_pop(buf1, 8);
    cout << "buf1 is " << buf1 << endl;
    
    return 0;
}