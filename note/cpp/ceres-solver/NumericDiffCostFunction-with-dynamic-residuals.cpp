#include <ceres/ceres.h>
#include <vector>

using ceres::NumericDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;
using std::vector;

struct ExponentialResidual {
  ExponentialResidual(vector<double> data) : data_(data) {}

  bool operator()(const double* const transform, double* residual) const {
    double dx = transform[0];
    double dy = transform[1];
    double s = sin(transform[2]);
    double c = cos(transform[2]);
    double x1 = (data_[0]);
    double y1 = (data_[1]);
    double x2 = (data_[2]);
    double y2 = (data_[3]);
    residual[0] = x1 * c + y1 * s + dx - x2;
    residual[1] = y1 * c - x1 * s + dy - y2;
    return true;
  }

 private:
  const vector<double> data_;
};

int main(int argc, char** argv) {
  vector<vector<double> > data = {{0, 0, 0, 1}, {1, 0, 0, 2.1}, {1, 1, -1, 2}};
  double transform[] = {0, 0, 0};

  Problem problem;
  for (auto item : data) {
    problem.AddResidualBlock(
        new NumericDiffCostFunction<ExponentialResidual,
                                    ceres::NumericDiffMethodType::CENTRAL,
                                    ceres::DYNAMIC, 3>(
            new ExponentialResidual(item), ceres::Ownership::TAKE_OWNERSHIP, 2),
        NULL, transform);
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
