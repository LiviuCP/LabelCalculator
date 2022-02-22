#ifndef AUXDATA_H
#define AUXDATA_H

//ssize_t is not properly implemented on Windows so it has to be manually defined
#ifdef _WIN32
using ssize_t = long long;
#endif

#endif // AUXDATA_H
