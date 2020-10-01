# enumerate
C++11 single header library that adds support for enumerating items in a for-range loop.

## Examples

#### Enumerating vector's contents from 0 with step = 1 (default)
```cpp
std::vector<int> items{ 11, 22, 33, 44 };
for (auto& i : helpers::enumerate(items)) {
    std::cout << i.count << ": " << i.value << "\n";
}
```

Output:
```
0: 11
1: 22
2: 33
3: 44
```

#### Iterating backwards and counting down from items.size() with step = -1
```cpp
for (auto& i : helpers::enumerate(items.rbegin(), items.rend(), items.size(), -1)) {
    std::cout << i.count << ": " << i.value << "\n";
}
```

Output:
```
4: 44
3: 33
2: 22
1: 11
```

#### Enumerating contents of std::initializer_list with structural binding (C++17)
```cpp
for (auto& [count, value] : helpers::enumerate({ "oh my", "them", "char pointers!" })) {
    std::cout << count << ": " << value << "\n";
}
```

Output:
```
0: oh my
1: them
2: char pointers!
```

## Supported containers
All that can be used in a for-range loop.

## How to use
* Copy the `enumerate.h` file into your project.
* Optionally rename the default outer namespace (`helpers`) as you see fit.
* Enjoy!

## License
Public domain (The Unlicense). See `LICENSE` file for details.