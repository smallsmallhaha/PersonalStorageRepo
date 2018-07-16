# Eigen3 Matrix类详细介绍

Matrix是Eigen中最基本的类，类的声明是
```cpp
Matrix<typename Scalar,
       int RowsAtCompileTime,
       int ColsAtCompileTime,
       int Options = 0,
       int MaxRowsAtCompileTime = RowsAtCompileTime,
       int MaxColsAtCompileTime = ColsAtCompileTime>
```
前三个模板参数分别是矩阵元素类型、矩阵行数、矩阵列数。

## Matrix构造函数
```cpp
Matrix3f a;
MatrixXf b;

MatrixXf a(10,15);
VectorXf b(30);

// 元素个数小于4的vector可以直接初始化
Vector2d a(5.0, 6.0);
Vector3d b(5.0, 6.0, 7.0);
Vector4d c(5.0, 6.0, 7.0, 8.0);
```

## 存取元素
```cpp
Matrix3f m;
// 正常存取
m(0, 1) = 1;
m(1, 1) = m(0, 0);
// 逗号表达式初始化
m << 1, 2, 3,
     4, 5, 6,
     7, 8, 9;
```

## 衍生类型
Matrix有以下衍生类型
```cpp
MatrixNt for Matrix<type, N, N> 例如 MatrixXi for Matrix<int, Dynamic, Dynamic>
VectorNt for Matrix<type, N, 1> 例如 Vector2f for Matrix<float, 2, 1>
RowVectorNt for Matrix<type, 1, N> 例如 RowVector3d for Matrix<double, 1, 3>
其中
N 可以是 2, 3, 4, 或 X (动态).
t 可以是 i (int), f (float), d (double), cf (complex<float>), 或 cd (complex<double>).
```

Matrix通过下面的方式定义上述衍生类型
```cpp
#define EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, Size, SizeSuffix)   \
/** \ingroup matrixtypedefs */                                    \
typedef Matrix<Type, Size, Size> Matrix##SizeSuffix##TypeSuffix;  \
/** \ingroup matrixtypedefs */                                    \
typedef Matrix<Type, Size, 1>    Vector##SizeSuffix##TypeSuffix;  \
/** \ingroup matrixtypedefs */                                    \
typedef Matrix<Type, 1, Size>    RowVector##SizeSuffix##TypeSuffix;

#define EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, Size)         \
/** \ingroup matrixtypedefs */                                    \
typedef Matrix<Type, Size, Dynamic> Matrix##Size##X##TypeSuffix;  \
/** \ingroup matrixtypedefs */                                    \
typedef Matrix<Type, Dynamic, Size> Matrix##X##Size##TypeSuffix;

#define EIGEN_MAKE_TYPEDEFS_ALL_SIZES(Type, TypeSuffix) \
EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 2, 2) \
EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 3, 3) \
EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, 4, 4) \
EIGEN_MAKE_TYPEDEFS(Type, TypeSuffix, Dynamic, X) \
EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 2) \
EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 3) \
EIGEN_MAKE_FIXED_TYPEDEFS(Type, TypeSuffix, 4)

EIGEN_MAKE_TYPEDEFS_ALL_SIZES(int,                  i)
EIGEN_MAKE_TYPEDEFS_ALL_SIZES(float,                f)
EIGEN_MAKE_TYPEDEFS_ALL_SIZES(double,               d)
EIGEN_MAKE_TYPEDEFS_ALL_SIZES(std::complex<float>,  cf)
EIGEN_MAKE_TYPEDEFS_ALL_SIZES(std::complex<double>, cd)
```
