#include <chrono>
#include <cstdio>
#include <memory>
#include <thread>
#include <vector>

struct VeryLargeStruct
{
    int a[1024];
    int b[2048];
    int c[4096];
    int d[8192];
};

namespace detail
{

void sleep_and_push(std::vector<std::shared_ptr<VeryLargeStruct>> &vec)
{
    for (int i = 0; i < 1000; i++)
    {
        vec.push_back(std::make_shared<VeryLargeStruct>());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void sleep_and_malloc()
{
    std::vector<VeryLargeStruct *> vec;
    for (int i = 0; i < 1000; i++)
    {
        auto ptr = new VeryLargeStruct();
        vec.push_back(ptr);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for (auto ptr : vec)
    {
        delete ptr;
    }
}

} // namespace detail

int main()
{
    std::vector<std::shared_ptr<VeryLargeStruct>> vec;
    detail::sleep_and_push(vec);

    std::thread t(detail::sleep_and_malloc);

    t.join();
    return 0;
}
