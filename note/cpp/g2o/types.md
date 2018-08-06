# G2O主要数据类型

g2o全称是General (Hyper) Graph Optimization，使用图表示优化问题。
g2o中的主要数据类型有顶点(vertex)和边(edge)两种，顶点继承于g2o::OptimizableGraph::Vertex，边继承于g2o::OptimizableGraph::Edge。

## 边
```cpp
// 超图的边
class HyperGraph::Edge {
  VertexContainer& vertices ();
}


// 优化图的边，可以看成抽象类
class OptimizableGraph::Edge : public HyperGraph::Edge, public HyperGraph::DataContainer {
  // 误差向量和信息矩阵的维度
  int _dimension;
  int _level;
  // 核函数，用于提高优化的鲁棒性
  RobustKernel* _robustKernel;
  // 参数类型
  std::vector<std::string> _parameterTypes;
  // 参数
  std::vector<Parameter**> _parameters;
  std::vector<int> _parameterIds;

  // 下面是一些重要的虚函数
  // 设置观测值
  virtual bool setMeasurementData(const number_t* m);
  // 设置和函数
  void setRobustKernel(RobustKernel* ptr);
  // 方差
  virtual number_t chi2() const = 0;
  // ?
  virtual void linearizeOplus(JacobianWorkspace& jacobianWorkspace) = 0;
  // ?
  virtual void initialEstimate(const OptimizableGraph::VertexSet& from, OptimizableGraph::Vertex* to) = 0;
}


// 基本优化图的边
// D : 边的维度
// E : 边的观测值类型
template <int D, typename E>
class BaseEdge : public OptimizableGraph::Edge {
  typedef E Measurement;
  typedef Eigen::Matrix<double, D, 1, Eigen::ColMajor> ErrorVector;
  typedef Eigen::Matrix<double, D, D, Eigen::ColMajor> InformationType;
  // 观测值，类型为E，标量
  Measurement _measurement;
  // 信息矩阵，类型为D维方阵
  InformationType _information;
  // 误差向量，类型为D维列向量
  ErrorVector _error;
}

// 下面的一些类都是OptimizableGraph::BaseEdge模板类的特化
class BaseEdge < D, Eigen::Vector2d >;
class BaseEdge < D, SE3Quat >;
```

## 顶点
```cpp
// 超图的顶点
class HyperGraph::Vertex {
  EdgeSet& edges ();
}

// 优化图的顶点
class OptimizableGraph::Vertex : public HyperGraph::Vertex, public HyperGraph::DataContainer {
  // 优化图指针
  OptimizableGraph* _graph;
  // 用户数据
  Data* _userData;
  // 顶点在海塞矩阵中的索引
  int _hessianIndex;
  // 顶点数据是否固定
  bool _fixed;
  // 是否边缘化
  bool _marginalized;
  // 海塞方阵和_b向量的维度
  int _dimension;
  // 顶点在海塞矩阵中的列索引
  int _colInHessian;

  // 获得海塞矩阵数据
  virtual number_t* hessianData() = 0;
  // ?
  virtual number_t solveDirect(number_t lambda=0) = 0;
  // ?
  virtual void setToOriginImpl() = 0;
  // ?
  virtual bool setEstimateDataImpl(const number_t* );
  // ?
  virtual bool setMinimalEstimateDataImpl(const number_t* );
  // 加运算实现
  virtual void oplusImpl(const number_t* v) = 0;  
}


template <int D, typename T>
class BaseVertex : public OptimizableGraph::Vertex {
  typedef T EstimateType;
  typedef std::stack<EstimateType, std::vector<EstimateType, Eigen::aligned_allocator<EstimateType> > > BackupStackType;
  typedef Eigen::Map<Eigen::Matrix<double, D, D, Eigen::ColMajor>, Eigen::Matrix<double, D, D, Eigen::ColMajor>::Flags & Eigen::AlignedBit ? Eigen::Aligned : Eigen::Unaligned > HessianBlockType;
  // 海塞矩阵，类型为D维方阵
  HessianBlockType _hessian;
  // _b矩阵，类型为D维列向量
  Eigen::Matrix<number_t, D, 1, Eigen::ColMajor> _b;
  // 估计值，类型为T
  EstimateType _estimate;
};
```

## 应用

### SBA
```cpp
// 相机内参顶点，含有fx,fy,cx,cy,baseline五个参数
class VertexIntrinsics : public BaseVertex<4, Eigen::Matrix<number_t, 5, 1, Eigen::ColMajor> >
// 相机位姿顶点，含有x,z,z,qx,qy,qz六个值(qw默认为正)
class VertexCam : public BaseVertex<6, SBACam>
// 空间点位姿顶点，含有x,y,z三个值
class VertexSBAPointXYZ : public BaseVertex<3, Vector3>
// 单目相机中空间点到像素的投影方程边
class EdgeProjectP2MC : public  BaseBinaryEdge<2, Vector2, VertexSBAPointXYZ, VertexCam> 
// 立体相机中空间点到像素的投影方程边
class EdgeProjectP2SC : public  BaseBinaryEdge<3, Vector3, VertexSBAPointXYZ, VertexCam>
// 带有标定参数的单目相机中空间点到像素的投影方程边 ?
class EdgeProjectP2MC_Intrinsics : public  BaseMultiEdge<2, Vector2> 

// 相机间的位姿约束边
class EdgeSBACam : public BaseBinaryEdge<6, SE3Quat, VertexCam, VertexCam>
// 相机间的尺度约束边
class EdgeSBAScale : public BaseBinaryEdge<1, number_t, VertexCam, VertexCam>

// 相机参数，含有焦距、像主点、基线
class CameraParameters : public g2o::Parameter
// 李代数表示的三维位姿顶点
class VertexSE3Expmap : public BaseVertex<6, SE3Quat>
// 李代数表示的三维位姿边
class EdgeSE3Expmap : public BaseBinaryEdge<6, SE3Quat, VertexSE3Expmap, VertexSE3Expmap>
// 三维空间点在李代数表示的相机位姿下的投影方程边
class EdgeProjectXYZ2UV : public  BaseBinaryEdge<2, Vector2, VertexSBAPointXYZ, VertexSE3Expmap>
// 已知相机内参的三维空间点在李代数表示的相机位姿下的投影方程边（高博在书中提到的方程）
class EdgeSE3ProjectXYZ : public BaseBinaryEdge<2, Vector2, VertexSBAPointXYZ, VertexSE3Expmap>

// 仅优化相机位姿
class EdgeSE3ProjectXYZOnlyPose : public BaseUnaryEdge<2, Vector2, VertexSE3Expmap>

```
