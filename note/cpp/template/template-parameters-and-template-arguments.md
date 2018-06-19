# Template parameters and template arguments

declaration syntax:
template < parameter-list > declaration

## Template parameters
### Non-type template parameter
> type name(optional)             (1)
> type name(optional) = default   (2)
> type ... name(optional)         (3) (since C++11)
> auto name                       (4) (since C++17)

type is one of the following types:
> std::nullptr_t (since C++11);
> integral type;
> lvalue reference type (to object or to function);
> pointer type (to object or to function);
> pointer to member type (to member object or to member function);
> enumeration type.

### Type template parameter
> typename name(optional)                 (1)
> class name(optional)                    (2)
> typename|class name(optional) = default (3)
> typename|class ... name(optional)       (4) (since C++11)

### Template template parameter
> template < parameter-list > typename(C++17)|class name(optional)           (1) 
> template < parameter-list > typename(C++17)|class name(optional) = default (2) 
> template < parameter-list > typename(C++17)|class ... name(optional)       (3) (since C++11)

### Constrained template parameter


## Template arguments
### Template non-type arguments
### Template type arguments
### Template template arguments

