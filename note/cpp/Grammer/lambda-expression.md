# Lambda expressions (since C++11)

> Constructs a closure: an unnamed function object capable of capturing variables in scope.

## Syntax

[ captures ] <tparams>(optional)(c++20) ( params ) specifiers exception attr -> ret requires(optional)(c++20) { body }	(1)
[ captures ] ( params ) -> ret { body }	(2)
[ captures ] ( params ) { body }	(3)
[ captures ] { body }	(4)
1) Full declaration.
2) Declaration of a const lambda: the objects captured by copy are const in the lambda body.
3) Omitted trailing-return-type: the return type of the closure's operator() is deduced from return statements as if for a function whose return type is declared auto.
4) Omitted parameter list: function takes no arguments, as if the parameter list was (). This form can only be used if none of constexpr, mutable, exception specification, attributes, or trailing return type is used.

## Explanation
### captures
> a comma-separated list of zero or more captures, optionally beginning with a capture-default.
>> Capture list can be passed as follows (see below for the detailed description):
>>> [a,&b] where a is captured by copy and b is captured by reference.  
>>> [this] captures the current object (*this) by reference  
>>> [&] captures all automatic variables used in the body of the lambda by reference and current object by reference if exists  
>>> [=] captures all automatic variables used in the body of the lambda by copy and current object by reference if exists  
>>> [] captures nothing  

> A lambda expression can use a variable without capturing it if the variable:
>> is a non-local variable or has static or thread local storage duration (in which case the variable cannot be captured), or  
>> is a reference that has been initialized with a constant expression.

> A lambda expression can read the value of a variable without capturing it if the variable  
>> has const non-volatile integral or enumeration type and has been initialized with a constant expression, or  
>> is constexpr and trivially copy constructible.

Structured bindings cannot be captured. (since C++17)

### tparams(C++20)
a template parameter list (in angle brackets), used to provide names to the template parameters of a generic lambda (see ClosureType::operator() below). Like in a template declaration, the template parameter list may be followed by an optional requires-clause, which specifies the constraints on the template arguments. If provided, the template parameter list cannot be empty (<> is not allowed).

### params
The list of parameters, as in named functions, except that default arguments are not allowed (until C++14). If auto is used as a type of a parameter, the lambda is a generic lambda. (since C++14)

### specifiers
> Optional sequence of specifiers.The following specifiers are allowed:
>> mutable: allows body to modify the parameters captured by copy, and to call their non-const member functions  
>> constexpr: explicitly specifies that the function call operator is a constexpr function. When this specifier is not present, the function call operator will be constexpr anyway, if it happens to satisfy all constexpr function requirements (since C++17)

### exception
provides the exception specification or the noexcept clause for operator() of the closure type

### attr
provides the attribute specification for operator() of the closure type

### ret
Return type. If not present it's implied by the function return statements (or void if it doesn't return any value)

### requires
adds a constraint to operator() of the closure type

### body
Function body


# Example
```cpp
[=] () mutable throw() -> int
{
    int n = x + y;
    x = y;
    y = n;
    return n;
}
```

# Attention
* 编译器会将lambda表达式翻译成一个匿名函数，然后通过普通的函数调用使用lambda表达式
* 默认捕获规则分为值捕获[=]和引用捕获[&]两种，但是不可重复，例如[&, &var]是错的
* lambda表达式默认的参数和返回值都是const的

# Reference
大部分内容来自cppreference.com，例子来自msdn
