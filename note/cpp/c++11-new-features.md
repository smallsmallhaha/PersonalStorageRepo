# c++11新特性
## C++核心语言扩展
### 核心语言运行时性能增强
#### 右值引用和移动语义
#### constexpr - 编译期常量标识符

### 核心语言构建性能增强
#### extern template
```cpp
// c++03 声明 + 实例化模板
template class std::vector<MyClass>;
// C++11 声明但不实例化模板
extern template class std::vector<MyClass>;
```

### 核心语言可用性增强
#### 初始化列表
```cpp
struct Object { float first; int second; };
Object scalar = {0.43f, 10};
```
#### Uniform initialization
```cpp
std::vector<std::string> v{ "xyzzy", "plugh", "abracadabra" };
```
#### 自动类型推导
```cpp
// auto
for (auto itr = myvec.cbegin(); itr != myvec.cend(); ++itr) { }
for (auto& x : myvec) { }
// decltype
auto c = 0;           // c has type int
auto d = c;           // d has type int
decltype(c) e;        // e has type int, the type of the entity named by c
decltype((c)) f = c;  // f has type int&, because (c) is an lvalue
decltype(0) g;        // g has type int, because 0 is an rvalue
```
#### 基于范围的for循环
```cpp
for (auto& x : {1, 2, 3, 4, 5}) { }
```
#### 匿名函数: lambda表达式
```cpp
auto func = [](int x, int y) -> int { return x + y; };
```
#### 替代性的函数语法
```cpp
template<class Lhs, class Rhs>
auto adding_func(const Lhs &lhs, const Rhs &rhs) -> decltype(lhs+rhs) {return lhs + rhs;}
```
#### 显式的override和final关键字
```cpp
// override
struct Base {
  virtual void some_func(float);
};

struct Derived : Base {
  virtual void some_func(int) override; // ill-formed - doesn't override a base class method
};

// final
struct Base1 final { };
struct Derived1 : Base1 { }; // ill-formed because the class Base1 has been marked final

struct Base2 {
  virtual void f() final;
};

struct Derived2 : Base2 {
  void f(); // ill-formed because the virtual function Base2::f has been marked final
};
```
#### 空指针常量nullptr
```cpp
char *pc = nullptr;     // OK
int  *pi = nullptr;     // OK
bool   b = nullptr;     // OK. b is false.
int    i = nullptr;     // error
#### 强类型枚举: enum class
#### 显式转换关键字: explicit
#### 模板别名
```cpp
template <typename First, typename Second, int Third>
class SomeType;

template <typename Second>
using TypedefName = SomeType<OtherType, Second, 5>;
```

### 核心语言功能增强
#### 可变参数模板
```cpp
void print() { }

template <class T, typename ...Args> 
void print(const T _First, const Args ..._Rest) {
  cout << _First << endl;
  print(_Rest...);
}
```
#### 成员函数显式访问声明: default delete
```cpp
struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};
struct NoInt {
  void f(double i);
  void f(int) = delete;
};
```

## C++标准库变化
#### 线程: std::thread std::mutex std::condition_variable
#### tuple
#### hash表: std::unordered_set std::unordered_map
#### 正则表达式库: regex
#### 智能指针
```cpp
/***************** unique_ptr ****************/
std::unique_ptr<int> p1(new int(5));
std::unique_ptr<int> p2 = p1;             // compile error
std::unique_ptr<int> p3 = std::move(p1);  // transfers ownership, p3 now owns the memory and p1 is set to nullptr
p3.reset(); // deletes the memory
p1.reset(); // does nothing

/***************** shared_ptr ****************/
// warning when using shared_ptr:
// 1. never use an original pointer to initialize multiple shared_ptr(s)
// 2. never create shared_ptr in function arguments
// 3. use weak_ptr to break circular reference, if necessary
std::shared_ptr<int> p0(new int(5));      // valid, allocates 1 integer and initialize it with value 5
std::shared_ptr<int[]> p1(new int[5]);    // valid, allocates 5 integers
std::shared_ptr<int[]> p2 = p1;           // both now own the memory
p1.reset();                               // memory still exists, due to p2
p2.reset();                               // deletes the memory, since no one else owns the memory

/***************** weak_ptr ****************/
std::shared_ptr<int> p1(new int(5));
std::weak_ptr<int> wp1 = p1;              // p1 owns the memory
{
  std::shared_ptr<int> p2 = wp1.lock();   // Now p1 and p2 own the memory
  if(p2) {                                // As p2 is initialized from a weak pointer,
                                          // you have to check if the memory still exists!
    // Do something with p2
  }
}                                         // p2 is destroyed. Memory is owned by p1
p1.reset();                               // Memory is deleted
std::shared_ptr<int> p3 = wp1.lock();     // Memory is gone, so we get an empty shared_ptr
if(p3) {
  // Will not execute this
}
```
#### 随机数库

## 启用C++11
#### gcc
-std=c++11
#### clang
-std=c++11
#### cmake
set(CMAKE_CXX_STANDARD 11)
#### qmake
CONFIG += c++11
