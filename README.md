# MyVector — Custom STL-Compliant Vector in C++

A from-scratch implementation of `std::vector` in C++17, demonstrating deep understanding of low-level memory management, the Rule of Five, template metaprogramming, and modern C++ idioms.

---

## Features

- **Rule of Five** — destructor, copy constructor, move constructor, copy assignment, move assignment all explicitly defined
- **Move semantics** — `noexcept` move operations eliminate redundant deep copies
- **`emplace_back`** — true in-place construction via placement new and perfect forwarding (`std::forward`)
- **`push_back`** — overloaded for both lvalues and rvalues
- **`reserve`** — capacity reallocation using `std::move` for performance
- **Iterator support** — `begin()`/`end()` enabling range-based for loops and STL algorithm compatibility
- **Bounds-checked access** — `at()` throws `std::out_of_range` on invalid index
- **Amortized O(1) insertions** — capacity doubles on reallocation

---

## Build & Run

```bash
g++ -std=c++17 -Wall -Wextra -o myvector myvector.cpp
./myvector
```

---

## Sample Output

```
MyVector Constructor
Constructor

Push Back with Lvalue:
Copy Assignment

Push Back with Rvalue:
Constructor
Move Assignment
Destructor

Emplace Back:
Parameterized Constructor: 10

Range-based for loop:
Element visited
Element visited
Element visited

Bounds-checked access (at):
Caught: MyVector: index out of range

Size: 3 Capacity: 4
```

---

## Implementation Details

### Rule of Five

All five special member functions explicitly manage the raw heap-allocated buffer:

| Operation | Behaviour |
|---|---|
| Copy constructor | Allocates new buffer, deep copies all elements |
| Move constructor | Steals pointer from source, nulls out source |
| Copy assignment | Deletes existing buffer, deep copies from source |
| Move assignment | Deletes existing buffer, steals pointer from source |
| Destructor | Releases heap memory with `delete[]` |

### emplace_back vs push_back

`push_back` copies or moves an existing object into the buffer. `emplace_back` constructs the object directly in buffer memory using placement new — no temporary object is created.

```cpp
v.push_back(Test(10));  // constructs temporary, then moves into buffer
v.emplace_back(10);     // constructs directly in buffer — no temporary
```

### Capacity Growth Strategy

Capacity doubles on each reallocation — amortized O(1) insertions. `reserve()` uses `std::move` to transfer existing elements into the new buffer without unnecessary copies.

### Iterator Support

Raw pointer iterators enable range-based for loops and full STL algorithm compatibility:

```cpp
MyVector<int> v;
v.push_back(1);
v.push_back(2);
for (const auto& x : v) {
    std::cout << x << "\n";  // works via begin()/end()
}
std::sort(v.begin(), v.end());  // STL algorithm compatible
```

---

## API Reference

| Method | Description |
|---|---|
| `push_back(const T&)` | Insert lvalue at end |
| `push_back(T&&)` | Insert rvalue at end |
| `emplace_back(Args&&...)` | Construct in-place at end |
| `reserve(size_t)` | Pre-allocate capacity |
| `operator[](size_t)` | Unchecked element access |
| `at(size_t)` | Bounds-checked element access |
| `begin()` / `end()` | Iterator support |
| `size()` | Number of elements |
| `capacity()` | Current allocated capacity |
| `empty()` | Returns true if size == 0 |

---

## Known Limitations

- Uses `new T[capacity]` which default-constructs all slots including unoccupied ones. Production implementations use raw memory (`::operator new`) with placement new to avoid this overhead.
- No `shrink_to_fit`, `insert`, `erase`, or `pop_back` — scope intentionally limited to core operations.
- No allocator support.

---

## What This Demonstrates

Built to understand the internals that `std::vector` abstracts away:

- Why move semantics exist and when they trigger
- The cost of copy vs move during reallocation
- How `emplace_back` avoids temporaries via variadic templates and perfect forwarding
- How iterators enable STL algorithm compatibility
- Memory ownership and the dangers of raw pointer management

---

## Author

**Manish Paul** — IIT Kharagpur  
[github.com/manishpaulish](https://github.com/manishpaulish) | [linkedin.com/in/manish-paul-381b72324](https://linkedin.com/in/manish-paul-381b72324)
