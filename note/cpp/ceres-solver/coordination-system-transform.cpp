#include "ceres/ceres.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

const int kNumObservations = 3;
double data[kNumObservations][4] = {
    {0, 0, 0, 1}, {1, 0, 0, 2.1}, {1, 1, -1, 2}};

struct ExponentialResidual {
  ExponentialResidual(double* data) : data_(data) {}

  template <typename T>
  bool operator()(const T* const transform, T* residual) const {
    T dx = transform[0];
    T dy = transform[1];
    T s = sin(transform[2]);
    T c = cos(transform[2]);
    T x1 = T(data_[0]);
    T y1 = T(data_[1]);
    T x2 = T(data_[2]);
    T y2 = T(data_[3]);
    residual[0] = x1 * c + y1 * s + dx - x2;
    residual[1] = y1 * c - x1 * s + dy - y2;
    return true;
  }

 private:
  const double* data_;
};

int main(int argc, char** argv) {
  double transform[3] = {0, 0, 0};

  Problem problem;
  for (int i = 0; i < kNumObservations; ++i) {
    problem.AddResidualBlock(
        new AutoDiffCostFunction<ExponentialResidual, 2, 3>(
            new ExponentialResidual(data[i])),
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
  std::cout << "error: "
            << sqrt(summary.final_cost * 2 / (kNumObservations - 1))
            << std::endl;
  return 0;
}
