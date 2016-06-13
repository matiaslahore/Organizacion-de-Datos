#include "Imagen.h"
#include "ArchivoNeighbors.h"
#include <cmath>
Imagen::Imagen(ArchivoTrain& archivo, const string& linea, Entrada& en):entrada(en){
	archivo.parsearFila(linea,pixeles);
	if(pixeles.size()==0){
		throw "hay 0 pixeles, imagen ivalida";
	}
}
Imagen::Imagen(ArchivoTrain& archivo, Entrada& en):entrada(en){
	archivo.parsearPosicion(en.posicion,pixeles);
	//cout<<"-----------"<<endl;
	vector<float>::iterator it;
	/*
	cout<<endl;
	for(it = pixeles.begin(); it!=pixeles.end();++it){
		cout<<*it<<" ";
	}
	cout<<endl;
	* */
}

double Imagen::euclideanaCuadrada(Imagen& otra){
	double suma = 0;
	for(unsigned int i = 0; i<pixeles.size(); ++i){
		float dif = pixeles[i]-otra.pixeles[i];
		suma += dif*dif;
	}
	return sqrt(suma);
}
/*
bool Imagen::esIntruso(Imagen& posible, ArchivoNeighbors& neighbors){
	double mayor_distancia = dTargetMasDistante(neighbors);
	double distancia_otro = euclideanaCuadrada(posible);
	return (distancia_otro<=mayor_distancia+1);//ojo con las raíces cuadradaas!!!!
}
*/
double Imagen::dTargetMasDistante(ArchivoNeighbors& neighbors){
	map<Entrada*,double>::iterator it;
	map<Entrada*,double>& targets = entrada.targets;
	double mayor = 0;
	for(it = targets.begin(); it!=targets.end(); ++it){
		Entrada* entrada = it->first;
		Imagen& img = *neighbors.imagenes[entrada->posicion];
		double calculada = euclideanaCuadrada(img);
		if(mayor<calculada){
			mayor = calculada;
		}
	}
	return mayor;
}

void Imagen::sumar_productoT(Imagen& img ,Matriz& en){
	Eigen::Map<Vector> vec(pixeles.data());
	Eigen::Map<Vector> vec2(img.pixeles.data());
	Vector  resta  = vec2-vec;
	Vectort restat = resta.transpose();
	en += resta * restat;
}

double Imagen::distanciaMahalanobis(Imagen& otra,Matriz& m){
	//return sqrt(euclideanaCuadrada(otra));

	Eigen::Map<Vector> vec(pixeles.data());
	Eigen::Map<Vector> vec2(otra.pixeles.data());
	Vector  resta  = vec2-vec;
	Vectort restat = resta.transpose();
	double distancia_cuadrada = resta.transpose() * m * resta;
	return distancia_cuadrada;
	
	//return sqrt(euclideanaCuadrada(otra));
	
	
}
