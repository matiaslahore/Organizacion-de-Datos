#ifndef ENTRADA
#define ENTRADA
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;
class Entrada{
	public:
	Entrada(streampos posicion, string desde);//ojo que no se guarda el string en memoria por dioh muchachoh(?)
	streampos posicion;
	char label;
	int cantidadTargetNeighbors;
	map<Entrada*,double> targets;
	Entrada* entradaMasLejana();
	void setCantidadTargetNeighbors(int k){
		cantidadTargetNeighbors = k;
	}
	void posibleTargetNeighbor(Entrada& otra,double distancia_entre);
	
	void agregarEn(ofstream& en);
};
#endif
