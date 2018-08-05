# Sophus

# Sophus 是什么？
Sophus是一个李群和李代数的库。

# 主要数据类型
# 类型列表
该库主要有以下几种数据类型：  
特殊正交群: SO2 SO3  
特殊欧式群: SE2 SE3
相似变换群: Sim2 Sim3  
"旋转+缩放"变换群(用于构造上面的相似变换群): SE2 SE3  

上面类型的大部分接口都是通用的，因此我们将以SE3为例，展示这个李群/李代数库的类的构成：
```cpp
// 特殊欧式群SE(3)
template <class Scalar_, int Options>
class SE3 : public SE3Base<SE3<Scalar_, Options>> {
  static int constexpr DoF = 6;
  static int constexpr num_parameters = 7;
  static int constexpr N = 4;
  using Transformation = Matrix<Scalar, N, N>;
  using Point = Vector3<Scalar>;
  using Line = ParametrizedLine3<Scalar>;
  using Tangent = Vector<Scalar, DoF>;
  using Adjoint = Matrix<Scalar, DoF, DoF>;

  // 伴随矩阵，使得hat(Ad_A * x) = A * hat(x) A^{-1}
  Adjoint Adj();

  // 类型转换
  SE3<NewScalarType> cast();

  // 返回 this * exp(x) 在 x=0 处的导数
  // 返回值为7x6矩阵，矩阵元素是[x y z w dx dy dz]关于[dx dy dz a1 a2 a3]的导数
  // 其中 x y z w 是四元数  a1 a2 a3 是轴角
  Matrix<Scalar, num_parameters, DoF> Dx_this_mul_exp_x_at_0();

  // 返回 exp(x) 在 x 处的导数
  Sophus::Matrix<Scalar, num_parameters, DoF> Dx_exp_x(
      Tangent const& upsilon_omega);

  // 返回 exp(x) 在 x=0 处的导数
  Sophus::Matrix<Scalar, num_parameters, DoF>
  Dx_exp_x_at_0();
  
  // 获得 exp(x).matrix() 在 x=0 处的导数
  // 返回4x4矩阵
  Transformation Dxi_exp_x_matrix_at_0(int i);

  // 逆
  SE3<Scalar> inverse();

  // 指数映射
  SE3<Scalar> exp(Tangent const& a);

  // 对数映射，返回一个6维向量，前三维平移，后三维旋转
  Tangent log();

  // 返回一个4x4的变换矩阵，T=[R t;0 1]
  Transformation matrix();

  // 返回一个7维向量，数据是 [x y z w dx dy dz]
  Sophus::Vector<Scalar, num_parameters> params();

  // 求反对称矩阵
  Transformation hat(Tangent const& a);

  // 李括号运算，[A,B] := AB-BA
  Tangent lieBracket(Tangent const& a, Tangent const& b);

  // hat的逆运算
  Tangent vee(Transformation const& Omega);
}
```
