#include <memory>
#include <iostream>
#include <cstdio>

class T {
public:
    T(int n): a(n), p(new int(4)) {
        
    }
    ~T() {
        delete p;
    }
    int a;
    int *p;
};


int main(int argc, char **argv) {
    int num = 10;
    T t(2);

    std::shared_ptr<int> p(new int(23));
    std::shared_ptr<int> q = p;
    std::printf("%p %p\n", p.get(), q.get());
    std::printf("%ld %ld\n", p.use_count(), q.use_count());

    return 0;
}
