#ifndef UTIL_H
#define UTIL_H

#include <uuid/uuid.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

namespace util
{
inline int generateDeviceId(string &deviceId)
{
    uuid_t uuid;
    char out[37];
    uuid_generate(uuid);
    uuid_unparse_lower(uuid, out);
    deviceId = out;
    cout << "当前设备UUID：" << deviceId << endl;
    return 0;
}

inline void read(const string &filename, string &data)
{
    ifstream file(filename.c_str(), ios::in);
    if (file.is_open())
    {
        stringstream ss;
        ss << file.rdbuf();
        file.close();
        data = ss.str();
    }
    return;
}
} // namespace util

#endif // UTIL_H