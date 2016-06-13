#ifndef ARCHIVO_TRAIN
#define ARCHIVO_TRAIN
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <Eigen/Geometry>
#include "Entrada.h"
#include "config.h"
class Imagen;
using namespace std;
class ArchivoTrain{
	private:
	ifstream entrada;
	streampos bytes_archivo;
	
	public:
	long unsigned int neighborsEnBloque(Imagen& a, ulint p_inicial, ulint p_final, ifstream& entrada);
	vector<Entrada> entradas;
	void parsearPosicion(streampos pos,vector<float>& en);
	void parsearFila(const string& fila,vector<float>& en);
	ArchivoTrain();
	int dimensiones;
	double distancia(Entrada& e1, Entrada& e2);
	void conectarTargetNeighbors(int k,ulint desde, ulint hasta);
	void guardarTargetNeighbours();
	void guardarTargetNeighbours(ulint desde, ulint hasta);
	void cargarImagenes(map<ulint,Imagen*>& cargadas);
	private:
	int detectarDimensiones(const string& primera_linea);
	void conectarleTargetNeighbors(Entrada& a,int k);
};
#endif
