#ifndef CONFIG
#define CONFIG
/*
#define TARGET_NEIGHBORS 5
#define ARCHIVO_ENTRADA "tran.csv"
#define ARCHIVO_NEIGHBORS "TargetNeighbours.dat"
#define DIMENSIONES 164
*/
#include <Eigen/Core>
#define DIMENSIONES 164
#define RADIO_EXTRA_IMPOSTORES 1
#define FACTOR_AMPLIACION 10000
typedef Eigen::Matrix<float,DIMENSIONES,DIMENSIONES> Matriz;
typedef Eigen::Matrix<float,DIMENSIONES,1> Vector;
typedef Eigen::Matrix<float,1,DIMENSIONES> Vectort;
typedef unsigned long int ulint;

extern int TARGET_NEIGHBORS;
extern std::string ARCHIVO_ENTRADA;
extern std::string  ARCHIVO_NEIGHBORS;

#endif
