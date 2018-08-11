#include <g2o/core/base_unary_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <g2o/types/sba/types_six_dof_expmap.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <random>

using namespace std;

// 产生三维随机高斯向量
Eigen::Vector3d GuassNoiseVector(double sigma) {
  static std::default_random_engine e;           //引擎
  std::normal_distribution<double> n(0, sigma);  //均值, 方差
  return Eigen::Vector3d(n(e), n(e), n(e));
}

// 从轴角转换为旋转矩阵
inline Eigen::Matrix3d Angles2RotationMatrix(const Eigen::Vector3d& v) {
  double norm = sqrt(v(0) * v(0) + v(1) * v(1) + v(2) * v(2));
  return Eigen::AngleAxis<double>(norm, v / norm).toRotationMatrix();
}

// g2o edge
class EdgeProjectXYZRGBDPoseOnly
    : public g2o::BaseUnaryEdge<3, Eigen::Vector3d, g2o::VertexSE3Expmap> {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  EdgeProjectXYZRGBDPoseOnly(const Eigen::Vector3d& point) : _point(point) {}

  // 计算残差
  virtual void computeError() {
    const g2o::VertexSE3Expmap* pose =
        static_cast<const g2o::VertexSE3Expmap*>(_vertices[0]);
    // measurement is p, point is p'
    _error = _measurement - pose->estimate().map(_point);
  }

  // 计算雅克比矩阵
  virtual void linearizeOplus() {
    g2o::VertexSE3Expmap* pose =
        static_cast<g2o::VertexSE3Expmap*>(_vertices[0]);
    g2o::SE3Quat T(pose->estimate());
    Eigen::Vector3d xyz_trans = T.map(_point);
    double x = xyz_trans[0];
    double y = xyz_trans[1];
    double z = xyz_trans[2];

    _jacobianOplusXi(0, 0) = 0;
    _jacobianOplusXi(0, 1) = -z;
    _jacobianOplusXi(0, 2) = y;
    _jacobianOplusXi(0, 3) = -1;
    _jacobianOplusXi(0, 4) = 0;
    _jacobianOplusXi(0, 5) = 0;

    _jacobianOplusXi(1, 0) = z;
    _jacobianOplusXi(1, 1) = 0;
    _jacobianOplusXi(1, 2) = -x;
    _jacobianOplusXi(1, 3) = 0;
    _jacobianOplusXi(1, 4) = -1;
    _jacobianOplusXi(1, 5) = 0;

    _jacobianOplusXi(2, 0) = -y;
    _jacobianOplusXi(2, 1) = x;
    _jacobianOplusXi(2, 2) = 0;
    _jacobianOplusXi(2, 3) = 0;
    _jacobianOplusXi(2, 4) = 0;
    _jacobianOplusXi(2, 5) = -1;
  }

  virtual bool read(istream& in) {}
  virtual bool write(ostream& out) const {}

 protected:
  Eigen::Vector3d _point;
};

void bundleAdjustment(const vector<Eigen::Vector3d>& pts1,
                      const vector<Eigen::Vector3d>& pts2,
                      const Eigen::Vector3d angles_init,
                      const Eigen::Vector3d t_init) {
  // 初始化g2o
  typedef g2o::BlockSolver<g2o::BlockSolverTraits<6, 3>>
      Block;  // pose维度为 6, landmark 维度为 3
  std::unique_ptr<Block::LinearSolverType> linearSolver = g2o::make_unique<
      g2o::LinearSolverDense<Block::PoseMatrixType>>();  // 线性方程求解器
  std::unique_ptr<Block> solver_ptr =
      g2o::make_unique<Block>(std::move(linearSolver));  // 矩阵块求解器
  g2o::OptimizationAlgorithmGaussNewton* solver =
      new g2o::OptimizationAlgorithmGaussNewton(std::move(solver_ptr));
  g2o::SparseOptimizer optimizer;
  optimizer.setAlgorithm(solver);

  // vertex
  // 新建一个顶点，顶点数据为待优化量，用g2o中的李代数表示
  g2o::VertexSE3Expmap* pose = new g2o::VertexSE3Expmap();  // camera pose
  pose->setId(0);
  pose->setEstimate(g2o::SE3Quat(Angles2RotationMatrix(angles_init), t_init));
  // 添加顶点
  optimizer.addVertex(pose);

  // edges
  for (size_t i = 0; i < pts1.size(); i++) {
    // 新建一条边，边的数据为观测量，自定义类型
    EdgeProjectXYZRGBDPoseOnly* edge = new EdgeProjectXYZRGBDPoseOnly(pts2[i]);
    edge->setId(i + 1);
    edge->setVertex(0, dynamic_cast<g2o::VertexSE3Expmap*>(pose));
    edge->setMeasurement(pts1[i]);
    edge->setInformation(Eigen::Matrix3d::Identity() * 1e4);
    // 添加边
    optimizer.addEdge(edge);
  }

  chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
  optimizer.setVerbose(true);
  optimizer.initializeOptimization();
  optimizer.optimize(10);
  chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
  chrono::duration<double> time_used =
      chrono::duration_cast<chrono::duration<double>>(t2 - t1);
  cout << "optimization costs time: " << time_used.count() << " seconds."
       << endl;

  cout << endl << "after optimization:" << endl;
  cout << "T=" << endl << Eigen::Isometry3d(pose->estimate()).matrix() << endl;
}

int G2O_test() {
  Eigen::Vector3d t = {3, 4, 5};
  Eigen::Vector3d angles = {1, 2, 1};
  int N = 100;  // 数据点

  vector<Eigen::Vector3d> pts1;
  vector<Eigen::Vector3d> pts2;

  Eigen::Matrix3d R = Angles2RotationMatrix(angles);
  for (int i = 0; i < N; i++) {
    // don't write like "GuassNoiseVector(10) * 10",
    // for temp object must have a receiver
    // generate pt1 and pt2
    Eigen::Vector3d pt1 = GuassNoiseVector(100);
    cout << pt1.transpose() << endl;
    Eigen::Vector3d pt2 = R * pt1 + t;
    // add guass noise
    pt1 += GuassNoiseVector(0.05);
    pt2 += GuassNoiseVector(0.05);
    pts1.push_back(pt1);
    pts2.push_back(pt2);
  }

  bundleAdjustment(pts2, pts1, angles + GuassNoiseVector(0.2),
                   t + GuassNoiseVector(0.2));

  return 0;
}
