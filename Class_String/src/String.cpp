#include "String.h"
#include <iostream>
#include <string.h>

using namespace std;

inline String::String(const char* cstr = 0)
{
    if(cstr) {
        m_data = new char[strlen(cstr) + 1];
        strcpy(m_data, cstr);
    }

    else { // 未指定初值的情况
        m_data = new char[1];
        *m_data = '\0';
    }
}

inline String::~String()
{
    delete[] m_data;
}

inline String::String(const String& str)
{
    m_data = new char[strlen(str.m_data) + 1];
    strcpy(m_data, str.m_data);
}

String& String::operator=(const String& str)
{
    if(this == &str) { // 避免自我赋值
        return *this;
    }

    delete[] m_data; // 赋值之前先清空之前申请的内存，再重新申请

    m_data = new char[strlen(str.m_data) + 1];
    strcpy(m_data, str.m_data);
    return *this;
} 