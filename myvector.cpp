#include<iostream>
#include <utility>
#include <stdexcept>

class Test {
public:
    Test() {
        std::cout << "Constructor\n";
    }
    Test(int x) {
        std::cout << "Parameterized Constructor: " << x << "\n";
    }
    ~Test() {
        std::cout << "Destructor\n";
    }
    Test(const Test&) {
        std::cout << "Copy Constructor\n";
    }
    Test(Test&&) noexcept {
        std::cout << "Move Constructor\n";
    }
    Test& operator=(const Test&) {
        std::cout << "Copy Assignment\n";
        return *this;
    }
    Test& operator=(Test&&) noexcept {
        std::cout << "Move Assignment\n";
        return *this;
    }
};

template<typename T>
class MyVector {
private:
    size_t sz;
    size_t cap;
    T* data;

public:
    // Default Constructor
    MyVector() : sz(0), cap(1), data(new T[1]) {
        std::cout << "MyVector Constructor\n";
    }

    // Destructor
    ~MyVector() {
        delete[] data;
        std::cout << "MyVector Destructor\n";
    }

    // Copy Constructor
    MyVector(const MyVector& other) : sz(other.sz), cap(other.cap), data(new T[other.cap]) {
        for (size_t i = 0; i < sz; i++) {
            data[i] = other.data[i];
        }
        std::cout << "MyVector Copy Constructor\n";
    }

    // Move Constructor
    MyVector(MyVector&& other) noexcept : data(other.data), sz(other.sz), cap(other.cap) {
        other.data = nullptr;
        other.sz = 0;
        other.cap = 0;
        std::cout << "MyVector Move Constructor\n";
    }

    // Copy Assignment Operator
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            cap = other.cap;
            data = new T[cap];
            for (size_t i = 0; i < sz; i++) {
                data[i] = other.data[i];
            }
        }
        std::cout << "MyVector Copy Assignment\n";
        return *this;
    }

    // Move Assignment Operator
    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            sz = other.sz;
            cap = other.cap;
            other.data = nullptr;
            other.sz = 0;
            other.cap = 0;
        }
        std::cout << "MyVector Move Assignment\n";
        return *this;
    }

    // Reserve — uses std::move for performance
    void reserve(size_t newCapacity) {
        if (newCapacity <= cap) return;

        T* newData = new T[newCapacity];

        for (size_t i = 0; i < sz; i++) {
            newData[i] = std::move(data[i]);  // move instead of copy
        }

        delete[] data;
        data = newData;
        cap = newCapacity;
    }

    // Push back for lvalues
    void push_back(const T& value) {
        if (sz == cap) reserve(cap * 2);
        data[sz++] = value;
    }

    // Push back for rvalues
    void push_back(T&& value) {
        if (sz == cap) reserve(cap * 2);
        data[sz++] = std::move(value);
    }

    // Emplace back — true in-place construction via placement new
    template<typename... Args>
    void emplace_back(Args&&... args) {
        if (sz == cap) reserve(cap * 2);
        new(&data[sz]) T(std::forward<Args>(args)...);  // placement new — no temporary
        sz++;
    }

    // Access operator — no bounds check (matches std::vector[])
    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    // at() — bounds-checked access
    T& at(size_t index) {
        if (index >= sz) throw std::out_of_range("MyVector: index out of range");
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= sz) throw std::out_of_range("MyVector: index out of range");
        return data[index];
    }

    // Iterator support — enables range-based for loops and STL algorithms
    T* begin() { return data; }
    T* end()   { return data + sz; }
    const T* begin() const { return data; }
    const T* end()   const { return data + sz; }

    // STL-convention accessors
    size_t size()     const { return sz; }
    size_t capacity() const { return cap; }
    bool   empty()    const { return sz == 0; }
};

int main() {
    MyVector<Test> v;

    Test t;

    std::cout << "\nPush Back with Lvalue:\n";
    v.push_back(t);

    std::cout << "\nPush Back with Rvalue:\n";
    v.push_back(Test());

    std::cout << "\nEmplace Back:\n";
    v.emplace_back(10);

    std::cout << "\nRange-based for loop:\n";
    for (const auto& elem : v) {
        (void)elem;
        std::cout << "Element visited\n";
    }

    std::cout << "\nBounds-checked access (at):\n";
    try {
        v.at(100);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    std::cout << "\nSize: " << v.size() << " Capacity: " << v.capacity() << "\n";

    return 0;
}