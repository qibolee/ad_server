#include <iostream>
#include <cstdio>
#include <ctime>

int main(int argc, char **argv) {
    std::size_t len = 1024;
    char *buf = new char[len];

    std::time_t t = std::time(NULL);
    std::size_t nl = std::strftime(buf, len, "%Y/%m/%d %H:%M:%S", std::localtime(&t));
    printf("%.*s\n", nl, buf);
    
    delete [] buf;
    return 0;
}
