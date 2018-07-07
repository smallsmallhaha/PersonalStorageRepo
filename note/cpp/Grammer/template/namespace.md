# 命名空间声明
namespace ns_name { declarations }
inline namespace ns_name { declarations }       (since C++11)
namespace { declarations }
ns_name::name
using namespace ns_name;
using ns_name::name;
namespace name = qualified-namespace ;
namespace ns_name::name                         (since C++17)

# 命名空间别名
namespace alias_name = ns_name;
namespace alias_name = ::ns_name;
namespace alias_name = nested_name::ns_name;

# 注意
"::"是作用域符号，可以作为全局作用域符(::name)、类作用域符(classname::name)、命名空间作用域符(namespace::name)。
