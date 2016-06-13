#ifndef TRABAJADOR
#define TRABAJADOR
#include "Thread.h"
class Trabajador: public Thread{
	ArchivoTrain& archivo;
	unsigned long int desde;
	unsigned long int hasta;
	public:
	Trabajador(ArchivoTrain& archivo,unsigned long int desde, unsigned long int hasta):archivo(archivo){
		this->desde = desde;
		this->hasta = hasta;
	}
	virtual void run(){
		archivo.conectarTargetNeighbors(5,desde,hasta);
	};
	virtual void end(){};
};
#endif
