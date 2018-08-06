/*
问题: 已知两个坐标系下面一些对应点的坐标，求坐标变换参数。
函数关系如下:
x * cos(theta) + y * sin(theta) + dx - x_ = 0;
y * cos(theta) - x * sin(theta) + dy - y_ = 0;
其中，(x,y)和(x_,y_)为同一个点在两个坐标系下的坐标，
待求参数有平移量dx、dy，旋转量theta。

令
f1 = x * cos(theta) + y * sin(theta) + dx - x_
f2 = y * cos(theta) - x * sin(theta) + dy - y_
手动求导，得到大小为 3 x 2 的雅克比矩阵:
[ D_dx(f1)   D_dy(f1)   D_theta(f1)
  D_dx(f2)   D_dy(f2)   D_theta(f2) ]
*/
#include <ceres/ceres.h>
#include <vector>

using ceres::NumericDiffCostFunction;
using ceres::SizedCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;
using std::vector;

// 模板参数说明:
// 2: 残差个数，对应于 residuals[0] residuals[1]
// 2: 参数块1中的参数个数，对应于 parameters[0][0] parameters[0][1]
// 1: 参数块2中的参数个数，对应于  parameters[1][0]
// 上面三个模板参数决定了雅克比矩阵的书写形式
class CustomSizedCostFunction : public SizedCostFunction<2, 2, 1> {
 public:
  CustomSizedCostFunction(const double x, const double y, const double x1,
                          const double y1)
      : x_(x), y_(y), x1_(x1), y1_(y1) {}
  virtual ~CustomSizedCostFunction() {}
  // 计算残差和雅克比矩阵
  virtual bool Evaluate(double const* const* parameters, double* residuals,
                        double** jacobians) const {
    const double dx = parameters[0][0];
    const double dy = parameters[0][1];
    const double theta = parameters[0][2];

    residuals[0] = +x_ * cos(theta) + y_ * sin(theta) + dx - x1_;
    residuals[1] = +y_ * cos(theta) - x_ * sin(theta) + dy - y1_;

    if (!jacobians || !jacobians[0]) return true;

    // 参数块1，参数个数为2
    // 该Jacobian矩阵表示为一维向量，依次为：
    // 残差1对各参数的导数、残差2对各参数的导数、...
    // D_dx(f1)
    jacobians[0][0] = 1;
    // D_dy(f1)
    jacobians[0][1] = 0;
    // D_dx(f2)
    jacobians[0][2] = 0;
    // D_dy(f2)
    jacobians[0][3] = 1;

    // 参数块2，参数个数为1
    // 该Jacobian矩阵表示为一维向量，依次为：
    // 残差1对各参数的导数、...
    // D_theta(f1)
    jacobians[1][0] = -x_ * sin(theta) + y_ * cos(theta);
    // D_theta(f2)
    jacobians[1][1] = -y_ * sin(theta) - x_ * cos(theta);

    return true;
  }

 private:
  const double x_;
  const double y_;
  const double x1_;
  const double y1_;
};

int main(int argc, char** argv) {
  vector<vector<double> > data = {{0, 0, 0, 1}, {1, 0, 0, 2.1}, {1, 1, -1, 2}};
  double transform[] = {0, 0, 0};

  Problem problem;
  for (auto item : data) {
    CostFunction* func =
        new CustomSizedCostFunction(item[0], item[1], item[2], item[3]);
    problem.AddResidualBlock(func, NULL, transform, transform + 2);
  }

  Solver::Options options;
  options.max_num_iterations = 25;
  options.minimizer_progress_to_stdout = true;
  Solver::Summary summary;
  Solve(options, &problem, &summary);
  std::cout << summary.FullReport() << "\n";
  std::cout << "dx = " << transform[0] << ", dy = " << transform[1]
            << ", theta = " << transform[2] << std::endl;
  std::cout << "error: " << sqrt(summary.final_cost * 2 / (data.size() - 1))
            << std::endl;
  return 0;
}
