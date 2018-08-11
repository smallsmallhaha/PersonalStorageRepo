# Ceres Solver
## Problem 类中的重要成员和方法
Problem中有两种重要的成员变量：
```cpp
// 参数块，所有待优化量都属于残差块
// 例如，使用函数
// ResidualBlockId AddResidualBlock(CostFunction* cost_function,
//                                  LossFunction* loss_function,
//                                  double* x0, double* x1, double* x2);
// 时，待优化量 x0 x1 x2 分别属于不同的参数块

std::map<double*, ParameterBlock*> parameter_block_map_;

// 残差块，由价值函数、损失函数和参数块构造
// 价值函数的功能是由参数块给出残差和雅可比矩阵（可选）
ResidualBlockSet residual_block_set_;

// 实际上，参数块残差块都在另一个成员中定义，不过我们不用关系内部细节
```
重要的方法如下：
```cpp
typedef internal::ResidualBlock* ResidualBlockId;

// 添加残差块，返回残差块的指针
// x0 x1 是两组参数，在函数内部将会对应添加两个ParameterBlock
ResidualBlockId AddResidualBlock(CostFunction* cost_function,
                                LossFunction* loss_function,
                                double* x0, double* x1);

// 添加一个参数块，默认加运算是最简单的线性加
void AddParameterBlock(double* values, int size);
// 添加一个自定义加运算和雅克比矩阵计算的参数块
void AddParameterBlock(double* values,
                        int size,
                        LocalParameterization* local_parameterization);

// 将参数块设为常量块
void SetParameterBlockConstant(double* values);
// 讲参数块设为变量块
void SetParameterBlockVariable(double* values);

// 设置参数块参数的上下界
void SetParameterLowerBound(double* values, int index, double lower_bound);
void SetParameterUpperBound(double* values, int index, double upper_bound);
```

### 注意
在Ceres中有些参数块的更新需要自己定义。例如，若一个参数块表示四元数，那么对它的更新不能直接使用简单的加法，用户可以继承 ceres::LocalParameterization 类，实现其中的Plus()和ComputeJacobian()函数。  
详见 LocalParameterization.md。

## ParameterBlock类
```cpp
// 参数块大小
int size_;
// 是否常量块
bool is_constant_;

// 自定义参数化指针
LocalParameterization* local_parameterization_;
// 自定义参数雅克比矩阵
mutable scoped_array<double> local_parameterization_jacobian_;

// 参数块中的每个参数上界
scoped_array<double> upper_bounds_;
// 参数块中的每个参数下界
scoped_array<double> lower_bounds_;

// 参数块对应的残差块集，用于处理依赖，删除残差块的时候用
scoped_ptr<ResidualBlockSet> residual_blocks_;

// 设置参数化，更新local_parameterization_和local_parameterization_jacobian_
void SetParameterization(LocalParameterization* new_parameterization);

void SetUpperBound(int index, double upper_bound);
void SetLowerBound(int index, double lower_bound);

bool Plus(const double *x, const double* delta, double* x_plus_delta);

// 保持与残差块的独立（将residual_blocks_置空）
void EnableResidualBlockDependencies();
// 添加残差块至residual_blocks_
void AddResidualBlock(ResidualBlock* residual_block);
```

## 例子

