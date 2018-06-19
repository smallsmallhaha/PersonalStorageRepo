# Class template

Syntax
template < parameter-list > class-declaration

## Class template instantiation
### Explicit instantiation
> template class|struct template-name < argument-list > ;

### Implicit instantiation
```c++
template<class T> struct Z {
    void f() {}
    void g(); // never defined
}; // template definition
template struct Z<double>; // explicit instantiation of Z<double>
Z<int> a; // implicit instantiation of Z<int>
Z<char>* p; // nothing is instantiated here
p->f(); // implicit instantiation of Z<char> and Z<char>::f() occurs here.
// Z<char>::g() is never needed and never instantiated: it does not have to be defined
```
If a class template has been declared, but not defined, at the point of instantiation, the instantiation yields an incomplete class type:
```c++
template<class T> class X; // declaration, not definition
X<char> ch;                // error: incomplete type
```
