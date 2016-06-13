#ifndef ACTIVESET
#define ACTIVESET
#include "Imagen.h"
class ActiveSet{
	public:
	Imagen* dato;
	Imagen* targetNeighbor;
	Imagen* impostor;
	ActiveSet(){
		dato = NULL;
		targetNeighbor = NULL;
		impostor=NULL;
	}
};

struct CompararActiveSet{
	bool operator()(const ActiveSet a, const ActiveSet b){
		size_t hash_a = (size_t)a.dato*7 + (size_t)a.targetNeighbor *3+ (size_t)a.impostor*11;
		size_t hash_b = (size_t)b.dato*7 + (size_t)b.targetNeighbor *3+ (size_t)b.impostor*11;
		return hash_a<hash_b;
	}
};

#endif
