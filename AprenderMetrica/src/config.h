#ifndef CONFIG
#define CONFIG

#define TARGET_NEIGHBORS 5
#define ARCHIVO_ENTRADA "tran.csv"
#define ARCHIVO_NEIGHBORS "TargetNeighbours.dat"
//#define DIMENSIONES 784
#define DIMENSIONES 164
#include <Eigen/Core>
typedef Eigen::Matrix<float,DIMENSIONES,DIMENSIONES> Matriz;
typedef Eigen::Matrix<float,DIMENSIONES,1> Vector;
typedef Eigen::Matrix<float,1,DIMENSIONES> Vectort;
typedef unsigned long int ulint;

#endif
