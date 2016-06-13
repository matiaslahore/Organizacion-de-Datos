#ifndef IMAGEN
#define IMAGEN
#include <iostream>
#include "ArchivoTrain.h"
#include "Entrada.h"
#include "config.h"
class ArchivoNeighbors;
using namespace std;
class Imagen{
	public:
	Imagen(ArchivoTrain& archivo, Entrada& entrada);
	Imagen(ArchivoTrain& archivo, const string& linea, Entrada& entrada);
	vector<float> pixeles;
	Entrada& entrada;
	double euclideanaCuadrada(Imagen& otra);
	double distanciaMahalanobis(Imagen& otra,Matriz& m);
	//bool esIntruso(Imagen& posible, ArchivoNeighbors& neighbors);
	double dTargetMasDistante(ArchivoNeighbors& neighbors);
	
	void sumar_productoT(Imagen& img ,Matriz& en);
	
};
#endif
