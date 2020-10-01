#include <vector>
#include <map>
#include <string>

#include <iostream>

#include "enumerate.h"

int main(int /*argc*/, const char** /*argv*/) {

    {
        std::cout << "Enumerating an empty collection is legit\n";
        std::vector<int> emptyVector{};
        for (auto& i : helpers::enumerate(emptyVector)) {
            std::cout << i.count << ": " << i.value << "\n";
        }
        std::cout << "\n";
    }
    {
        std::cout << "Vector test, counting from 0 with step = 1 (default)\n";
        std::vector<int> items{ 11, 22, 33, 44 };
        for (auto& i : helpers::enumerate(items)) {
            std::cout << i.count << ": " << i.value << "\n";
        }
        std::cout << "\n";

        std::cout << "Reversed Vector test, counting backwards from items.size() with step = -1\n";
        for (auto& i : helpers::enumerate(items.rbegin(), items.rend(), items.size(), -1)) {
            std::cout << i.count << ": " << i.value << "\n";
        }
        std::cout << "\n";

        {
            const auto& constItems = items;
            std::cout << "Const vector test, counting from 1 with step = 1\n";
            for (auto& i : helpers::enumerate(constItems, 1)) {
                std::cout << i.count << ": " << i.value << "\n";
            }
            std::cout << "\n";
        }


        std::cout << "vector as rvalue test, counting from 0 with step = 2\n";
        for (auto& i : helpers::enumerate(std::vector<int>{1, 2, 3, 4, 5, 6}, 0, 2) ) {
            std::cout << i.count << ": " << i.value << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

#ifdef __cpp_structured_bindings
    {
        std::cout << "Initializer list test (pass by rvalue)\n";
        for (auto& [count, value] : helpers::enumerate({ "oh my", "them", "char pointers!" })) {
            std::cout << count << ": " << value << "\n";
        }
        std::cout << "\n";

        std::cout << "Initializer list test (pass by ref)\n";
        auto list = { "oh my", "them", "char pointers!" };
        for (auto& [count, value] : helpers::enumerate(list)) {
            std::cout << count << ": " << value << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    {
        std::cout << "Array test\n";
        double doubles[] = { 0.1, 0.2, 0.7, 0.454, 12234.44 };
        for (auto& [i, v] : helpers::enumerate(doubles)) {
            std::cout << i << ": " << v << "\n";
        }
        std::cout << "\n";

        std::cout << "Const array test\n";
        for (auto& [i, v] : helpers::enumerate(std::as_const(doubles))) {
            std::cout << i << ": " << v << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
#endif

    {
        std::cout << "Const map test\n";

        const std::map<int, const char*> map{
            {1, "one"},
            {10, "ten"}
        };

        for (auto& it : helpers::enumerate(map)) {
            std::cout << it.count << ": (" << it.value.first << ", " << it.value.second << ")\n";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    return 0;
}