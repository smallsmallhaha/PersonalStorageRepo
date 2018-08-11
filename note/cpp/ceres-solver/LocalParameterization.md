```cpp
// Purpose: Sometimes parameter blocks x can overparameterize a problem
//
//   min f(x)
//    x
//
// In that case it is desirable to choose a parameterization for the
// block itself to remove the null directions of the cost. More
// generally, if x lies on a manifold of a smaller dimension than the
// ambient space that it is embedded in, then it is numerically and
// computationally more effective to optimize it using a
// parameterization that lives in the tangent space of that manifold
// at each point.
//
// For example, a sphere in three dimensions is a 2 dimensional
// manifold, embedded in a three dimensional space. At each point on
// the sphere, the plane tangent to it defines a two dimensional
// tangent space. For a cost function defined on this sphere, given a
// point x, moving in the direction normal to the sphere at that point
// is not useful. Thus a better way to do a local optimization is to
// optimize over two dimensional vector delta in the tangent space at
// that point and then "move" to the point x + delta, where the move
// operation involves projecting back onto the sphere. Doing so
// removes a redundent dimension from the optimization, making it
// numerically more robust and efficient.
//
// More generally we can define a function
//
//   x_plus_delta = Plus(x, delta),
//
// where x_plus_delta has the same size as x, and delta is of size
// less than or equal to x. The function Plus, generalizes the
// definition of vector addition. Thus it satisfies the identify
//
//   Plus(x, 0) = x, for all x.
//
// A trivial version of Plus is when delta is of the same size as x
// and
//
//   Plus(x, delta) = x + delta
//
// A more interesting case if x is two dimensional vector, and the
// user wishes to hold the first coordinate constant. Then, delta is a
// scalar and Plus is defined as
//
//   Plus(x, delta) = x + [0] * delta
//                        [1]
//
// An example that occurs commonly in Structure from Motion problems
// is when camera rotations are parameterized using Quaternion. There,
// it is useful only make updates orthogonal to that 4-vector defining
// the quaternion. One way to do this is to let delta be a 3
// dimensional vector and define Plus to be
//
//   Plus(x, delta) = [cos(|delta|), sin(|delta|) delta / |delta|] * x
//
// The multiplication between the two 4-vectors on the RHS is the
// standard quaternion product.
//
// Given g and a point x, optimizing f can now be restated as
//
//     min  f(Plus(x, delta))
//    delta
//
// Given a solution delta to this problem, the optimal value is then
// given by
//
//   x* = Plus(x, delta)
//
// The class LocalParameterization defines the function Plus and its
// Jacobian which is needed to compute the Jacobian of f w.r.t delta.

// 自定义参数化类
// 抽象类，Plus ComputeJacobian GlobalSize LocalSize 必须被子类实现
class CERES_EXPORT LocalParameterization {
 public:
  virtual ~LocalParameterization();

  // 自定义Plus函数
  //   x_plus_delta = Plus(x, delta)
  // 条件: Plus(x, 0) = x
  // 用途: 已知 x delta，计算 x_plus_delta
  virtual bool Plus(const double* x,
                    const double* delta,
                    double* x_plus_delta) const = 0;

  // 计算 Plus(x, delta) 在 delta=0 处的雅克比矩阵
  // 雅克比矩阵行优先，大小为 GlobalSize x LocalSize
  // 用途: 已知 delta_x 的情况下计算 delta，公式为 delta = delta_x * jacobian，
  //      delta 是由非线性优化方法计算得到的。
  virtual bool ComputeJacobian(const double* x, double* jacobian) const = 0;

  // local_matrix = global_matrix * jacobian
  //
  // global_matrix 行优先，大小为 num_rows x GlobalSize
  // local_matrix 行优先，大小为 num_rows x LocalSize
  // jacobian(x) 由于 ComputeJacobian 计算得到
  //
  // 该函数只会被 GradientProblem 使用，大部分时候使用默认的实现即可
  virtual bool MultiplyByJacobian(const double* x,
                                  const int num_rows,
                                  const double* global_matrix,
                                  double* local_matrix) const;

  // x 的元素个数
  virtual int GlobalSize() const = 0;

  // delta 的元素个数
  virtual int LocalSize() const = 0;
};

// Some basic parameterizations

// Plus(x, delta) = x + delta
class CERES_EXPORT IdentityParameterization : public LocalParameterization {
 public:
  explicit IdentityParameterization(int size);
  virtual ~IdentityParameterization() {}
  virtual bool Plus(const double* x,
                    const double* delta,
                    double* x_plus_delta) const;
  virtual bool ComputeJacobian(const double* x,
                               double* jacobian) const;
  virtual bool MultiplyByJacobian(const double* x,
                                  const int num_cols,
                                  const double* global_matrix,
                                  double* local_matrix) const;
  virtual int GlobalSize() const { return size_; }
  virtual int LocalSize() const { return size_; }

 private:
  const int size_;
};

// 让子集中的一部分为常量
// 常量的下标储存在 constancy_mask_ 中
class CERES_EXPORT SubsetParameterization : public LocalParameterization {
 public:
  explicit SubsetParameterization(int size,
                                  const std::vector<int>& constant_parameters);
  virtual ~SubsetParameterization() {}
  virtual bool Plus(const double* x,
                    const double* delta,
                    double* x_plus_delta) const;
  virtual bool ComputeJacobian(const double* x,
                               double* jacobian) const;
  virtual bool MultiplyByJacobian(const double* x,
                                  const int num_cols,
                                  const double* global_matrix,
                                  double* local_matrix) const;
  virtual int GlobalSize() const {
    return static_cast<int>(constancy_mask_.size());
  }
  virtual int LocalSize() const { return local_size_; }

 private:
  const int local_size_;
  std::vector<char> constancy_mask_;
};

// Plus(x, delta) = [cos(|delta|), sin(|delta|) delta / |delta|] * x
// 四元数，顺序为 w x y z，Plus()中的delta是轴角的二分之一
class CERES_EXPORT QuaternionParameterization : public LocalParameterization {
 public:
  virtual ~QuaternionParameterization() {}
  virtual bool Plus(const double* x,
                    const double* delta,
                    double* x_plus_delta) const;
  virtual bool ComputeJacobian(const double* x,
                               double* jacobian) const;
  virtual int GlobalSize() const { return 4; }
  virtual int LocalSize() const { return 3; }
};

// Plus(x, delta) = [sin(|delta|) delta / |delta|, cos(|delta|)] * x
// Eigen库中的四元数，顺序为 x y z w，Plus()中的delta是轴角的二分之一
class CERES_EXPORT EigenQuaternionParameterization
    : public ceres::LocalParameterization {
 public:
  virtual ~EigenQuaternionParameterization() {}
  virtual bool Plus(const double* x,
                    const double* delta,
                    double* x_plus_delta) const;
  virtual bool ComputeJacobian(const double* x, double* jacobian) const;
  virtual int GlobalSize() const { return 4; }
  virtual int LocalSize() const { return 3; }
};

// This provides a parameterization for homogeneous vectors which are commonly
// used in Structure for Motion problems.  One example where they are used is
// in representing points whose triangulation is ill-conditioned. Here
// it is advantageous to use an over-parameterization since homogeneous vectors
// can represent points at infinity.
//
// The plus operator is defined as
// Plus(x, delta) =
//    [sin(0.5 * |delta|) * delta / |delta|, cos(0.5 * |delta|)] * x
// with * defined as an operator which applies the update orthogonal to x to
// remain on the sphere. We assume that the last element of x is the scalar
// component. The size of the homogeneous vector is required to be greater than
// 1.
class CERES_EXPORT HomogeneousVectorParameterization :
      public LocalParameterization {
 public:
  explicit HomogeneousVectorParameterization(int size);
  virtual ~HomogeneousVectorParameterization() {}
  virtual bool Plus(const double* x,
                    const double* delta,
                    double* x_plus_delta) const;
  virtual bool ComputeJacobian(const double* x,
                               double* jacobian) const;
  virtual int GlobalSize() const { return size_; }
  virtual int LocalSize() const { return size_ - 1; }

 private:
  const int size_;
};

// Construct a local parameterization by taking the Cartesian product
// of a number of other local parameterizations. This is useful, when
// a parameter block is the cartesian product of two or more
// manifolds. For example the parameters of a camera consist of a
// rotation and a translation, i.e., SO(3) x R^3.
//
// Currently this class supports taking the cartesian product of up to
// four local parameterizations.
//
// Example usage:
//
// ProductParameterization product_param(new QuaterionionParameterization(),
//                                       new IdentityParameterization(3));
//
// is the local parameterization for a rigid transformation, where the
// rotation is represented using a quaternion.
class CERES_EXPORT ProductParameterization : public LocalParameterization {
 public:
  //
  // NOTE: All the constructors take ownership of the input local
  // parameterizations.
  //
  ProductParameterization(LocalParameterization* local_param1,
                          LocalParameterization* local_param2);

  ProductParameterization(LocalParameterization* local_param1,
                          LocalParameterization* local_param2,
                          LocalParameterization* local_param3);

  ProductParameterization(LocalParameterization* local_param1,
                          LocalParameterization* local_param2,
                          LocalParameterization* local_param3,
                          LocalParameterization* local_param4);

  virtual ~ProductParameterization();
  virtual bool Plus(const double* x,
                    const double* delta,
                    double* x_plus_delta) const;
  virtual bool ComputeJacobian(const double* x,
                               double* jacobian) const;
  virtual int GlobalSize() const { return global_size_; }
  virtual int LocalSize() const { return local_size_; }

 private:
  void Init();

  std::vector<LocalParameterization*> local_params_;
  int local_size_;
  int global_size_;
  int buffer_size_;
};
```
