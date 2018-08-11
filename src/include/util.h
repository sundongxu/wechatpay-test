#ifndef UTIL_H
#define UTIL_H

#include <uuid/uuid.h>
#include <string>
#include <sstream>

using namespace std;

namespace util
{
inline int generateDeviceId(string &deviceId)
{
    uuid_t uuid;
    char *in = nullptr;
    uuid_generate(uuid);
    uuid_parse(in, uuid);
    deviceId = in;
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