#include "ArchivoNeighbors.h"
#include "config.h"
#include "Imagen.h"
#include <ctime>

ArchivoNeighbors::ArchivoNeighbors(ArchivoTrain& t):train(t){	
	cargarImagenes();
	cout<<"Termine de cargar las imagenes"<<endl;
	conectarImagenes();
	cout<<"Termine de conectar las imagenes"<<endl;
}

void ArchivoNeighbors::conectarImagenes(){
	ifstream iAdyacencias(ARCHIVO_NEIGHBORS);
	while(!iAdyacencias.eof()){
		ulint mi_posicion;
		iAdyacencias >> mi_posicion;
		Imagen* correspondiente = imagenes[mi_posicion];
		
		for(int i = 0; i < TARGET_NEIGHBORS; i++){
			ulint otra_posicion;
			iAdyacencias >> otra_posicion;
			Imagen* otra = imagenes[otra_posicion];
			correspondiente->entrada.posibleTargetNeighbor(otra->entrada,5);//la distancia_entre no tiene sentido en este contexto, se recalcula todo el tiempo.
		}
	}
}
void ArchivoNeighbors::cargarImagenes(){
	train.cargarImagenes(imagenes);
}

void ArchivoNeighbors::agregarActiveSets(ConjuntoActiveSets& sets){
	map<ulint,Imagen*>::iterator it_dato;
	map<ulint,Imagen*>::iterator it_vecino;
	cout<<"Iniciando búsqueda de active sets:"<<endl;
	ulint iteracion = 0;
	ulint largo = imagenes.size();
	time_t inicial = clock();
	time_t actual;
	for(it_dato = imagenes.begin(); it_dato != imagenes.end(); ++it_dato){
		actual = clock();
		iteracion += 1;
		float proporcion = (float)iteracion/(float)largo;
		float tiempo_paso = (float)(actual-inicial)/CLOCKS_PER_SEC;
		float s_totales = tiempo_paso/proporcion;
		if(iteracion%50==1){
			cout<<"Voy :"<<proporcion*100<<"%, son en total: "<<s_totales<<"segundos, o "<<s_totales/3600<<" horas"<<endl;
		}
		
		Imagen* dato = it_dato->second;
		map<Entrada*,double>& targets = dato->entrada.targets;
		map<Entrada*,double>::iterator it_target;
		for(it_target = targets.begin(); it_target != targets.end(); ++it_target){
			Imagen* target = imagenes[it_target->first->posicion];
			double distancia_target = dato->euclideanaCuadrada(*target);
			
			for(it_vecino = imagenes.begin(); it_vecino != imagenes.end(); ++it_vecino){
				Imagen* vecino = it_vecino->second;
				double distancia_otro = dato->euclideanaCuadrada(*vecino);
				bool cumplen_distancias = distancia_otro<=distancia_target+1;
				bool labels_distintos = (dato->entrada.label != vecino->entrada.label);
				if(labels_distintos && cumplen_distancias){//definición de intruso del papaer!!!
				
					ActiveSet nuevo;//=new ActiveSet;
					nuevo.dato = dato;
					nuevo.targetNeighbor = target;
					nuevo.impostor = vecino;
					sets.insert(nuevo);
					//cout<<"Encontré un intruso!"<<vecino->entrada.posicion<<endl;
				/*
					ActiveSet* nuevo=new ActiveSet;
					nuevo->dato = dato;
					nuevo->targetNeighbor = target;
					nuevo->impostor = vecino;
					sets.insert(nuevo);
				*/
				}
			}
		}
	}
	cout<<"Terminó el cálculo de active sets"<<endl;
}

void ArchivoNeighbors::agregarActiveSets(ConjuntoActiveSets& sets, Matriz& m){
	map<ulint,Imagen*>::iterator it_dato;
	map<ulint,Imagen*>::iterator it_vecino;
	cout<<"Iniciando búsqueda de active sets:"<<endl;
	ulint iteracion = 0;
	ulint largo = imagenes.size();
	time_t inicial = clock();
	time_t actual;
	for(it_dato = imagenes.begin(); it_dato != imagenes.end(); ++it_dato){
		actual = clock();
		iteracion += 1;
		float proporcion = (float)iteracion/(float)largo;
		float tiempo_paso = (float)(actual-inicial)/CLOCKS_PER_SEC;
		float s_totales = tiempo_paso/proporcion;
		if(iteracion%50==1){
			cout<<"Voy :"<<proporcion*100<<"%, son en total: "<<s_totales<<"segundos, o "<<s_totales/3600<<" horas"<<endl;
		}
		
		Imagen* dato = it_dato->second;
		map<Entrada*,double>& targets = dato->entrada.targets;
		map<Entrada*,double>::iterator it_target;
		for(it_target = targets.begin(); it_target != targets.end(); ++it_target){
			Imagen* target = imagenes[it_target->first->posicion];
			double distancia_target = dato->distanciaMahalanobis(*target,m);
			
			for(it_vecino = imagenes.begin(); it_vecino != imagenes.end(); ++it_vecino){
				Imagen* vecino = it_vecino->second;
				double distancia_otro = dato->distanciaMahalanobis(*vecino,m);
				bool cumplen_distancias = distancia_otro<=distancia_target+1;
				bool labels_distintos = (dato->entrada.label != vecino->entrada.label);
				if(labels_distintos && cumplen_distancias){//definición de intruso del papaer!!!
				
					ActiveSet nuevo;//=new ActiveSet;
					nuevo.dato = dato;
					nuevo.targetNeighbor = target;
					nuevo.impostor = vecino;
					sets.insert(nuevo);
					//cout<<"Encontré un intruso!"<<vecino->entrada.posicion<<endl;
				/*
					ActiveSet* nuevo=new ActiveSet;
					nuevo->dato = dato;
					nuevo->targetNeighbor = target;
					nuevo->impostor = vecino;
					sets.insert(nuevo);
				*/
				}
			}
		}
	}
	cout<<"Terminó el cálculo de active sets"<<endl;
}


// 1-mu por sumatoria de (Cij)
void ArchivoNeighbors::calcularG0(Matriz& g0){
	cout<<"Iniciando búsqueda de G0:"<<endl;
	ulint iteracion = 0;
	ulint largo = imagenes.size();
	time_t inicial = clock();
	time_t actual;
	
	map<ulint,Imagen*>::iterator it;
	for(it = imagenes.begin(); it!=imagenes.end(); ++it){
		actual = clock();
		iteracion += 1;
		float proporcion = (float)iteracion/(float)largo;
		float tiempo_paso = (float)(actual-inicial)/CLOCKS_PER_SEC;
		float s_totales = tiempo_paso/proporcion;
		if(iteracion%50==1){
			cout<<"Voy :"<<proporcion*100<<"%, son en total: "<<s_totales<<"segundos, o "<<s_totales/3600<<" horas"<<endl;
		}
		
		Imagen& img = *(it->second);
		
		map<Entrada*,double>& targets = img.entrada.targets;
		map<Entrada*,double>::iterator it;
		for(it = targets.begin(); it!= targets.end(); ++it){
			Entrada& correspondiente = *(it->first);
			Imagen& img_t = *imagenes[correspondiente.posicion];
			img.sumar_productoT(img_t,g0);
		}
	}
	
	g0*=0.5;//porque mu=0.5 y entonces 1 - mu = 0.5
	
	cout<<"Terminó el cálculo de G0"<<endl;
}

// mu por sumatoria de (Cij-Cil)
void ArchivoNeighbors::calcularSegundoTerminoGradiente(Matriz& g,ConjuntoActiveSets& activos){
	cout<<"Iniciando búsqueda de SegundoTerminoGradiente:"<<endl;
	ulint iteracion = 0;
	ulint largo = activos.size();
	time_t inicial = clock();
	time_t actual;
	
	ConjuntoActiveSets::iterator it;
	for(it=activos.begin(); it!=activos.end(); ++it){
		
		actual = clock();
		iteracion += 1;
		float proporcion = (float)iteracion/(float)largo;
		float tiempo_paso = (float)(actual-inicial)/CLOCKS_PER_SEC;
		float s_totales = tiempo_paso/proporcion;
		if(iteracion%50==1){
			cout<<"Voy :"<<proporcion*100<<"%, son en total: "<<s_totales<<"segundos, o "<<s_totales/3600<<" horas"<<endl;
		}
		
		
		it->dato->sumar_productoT(*(it->targetNeighbor),g);
		it->dato->sumar_productoT(*(it->impostor),g);
		
		/*
		(*it)->dato->sumar_productoT(*((*it)->targetNeighbor),g);
		(*it)->dato->sumar_productoT(*((*it)->impostor),g);
		*/
	}
	g*=0.5;//porque mu=0.5
	
	cout<<"Terminó el cálculo de SegundoTerminoGradiente"<<endl;
}

void ArchivoNeighbors::filtrarActivos(ConjuntoActiveSets& sets, ConjuntoActiveSets& filtrados, Matriz& m){
	cout<<"Iniciando filtrado de activos:"<<endl;
	ulint iteracion = 0;
	ulint largo = sets.size();
	time_t inicial = clock();
	time_t actual;
	
	ConjuntoActiveSets::iterator it;
	for(it = sets.begin(); it != sets.end(); ++it){
		
		actual = clock();
		iteracion += 1;
		float proporcion = (float)iteracion/(float)largo;
		float tiempo_paso = (float)(actual-inicial)/CLOCKS_PER_SEC;
		float s_totales = tiempo_paso/proporcion;
		if(iteracion%50==1){
			cout<<"Voy :"<<proporcion*100<<"%, son en total: "<<s_totales<<"segundos, o "<<s_totales/3600<<" horas"<<endl;
		}
		
		
		
		/*
		Imagen* dato = (*it)->dato;
		Imagen* target = (*it)->targetNeighbor;
		Imagen* impostor = (*it)->impostor;
		*/
		
		Imagen* dato = it->dato;
		Imagen* target = it->targetNeighbor;
		Imagen* impostor = it->impostor;
		
		double distancia_target = dato->distanciaMahalanobis(*target,m);
		double distancia_otro = dato->distanciaMahalanobis(*impostor,m);
		bool cumplen_distancias = distancia_otro<=distancia_target+1;
		
		if(cumplen_distancias){
			filtrados.insert(*it);
		}
	}
	
	cout<<"Terminó el cálculo de sets activos"<<endl;
}
