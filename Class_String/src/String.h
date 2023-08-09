#ifndef __STRING_H__
#define __STRING_H__

class String
{
public:
    String(const char* cstr = 0);
    String(const String& str);
    String& operator=(const String& str);
    ~String();
    char* get_c_str() const {return m_data;}

private:
    char* m_data;
};


#endif