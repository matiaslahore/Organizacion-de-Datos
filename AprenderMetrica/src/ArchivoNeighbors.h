#ifndef ARCHIVONEIGHBORS
#define ARCHIVONEIGHBORS
#include "ArchivoTrain.h"
#include <set>
#include "ActiveSet.h"
#include "config.h"
typedef set<ActiveSet,CompararActiveSet> ConjuntoActiveSets;
//typedef set<ActiveSet*> ConjuntoActiveSets;
class ArchivoNeighbors{
	private:
	ArchivoTrain& train;
	
	public:
	map<ulint,Imagen*> imagenes;
	/**
	 * "Compute Nt+1 exactly"
	 * */
	void agregarActiveSets(ConjuntoActiveSets& sets);
	void agregarActiveSets(ConjuntoActiveSets& sets,Matriz& m);
	
	/**
	 * "Nt+1 ~ Nt+1 interseccion N(t)" (only search active set)
	 * */
	void filtrarActivos(ConjuntoActiveSets& sets, ConjuntoActiveSets& filtrados, Matriz& m);
	
	ArchivoNeighbors(ArchivoTrain& train);
	void conectarImagenes();
	void cargarImagenes();
	
	void calcularG0(Matriz& g0);
	
	void calcularSegundoTerminoGradiente(Matriz& g,ConjuntoActiveSets& activos);
};
#endif
