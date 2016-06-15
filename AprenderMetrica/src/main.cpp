#include <iostream>
#include "ArchivoTrain.h"
#include "Imagen.h"
#include "Trabajador.h"
#include "config.h"
#include "ArchivoNeighbors.h"
#include <set>
#include "ActiveSet.h"
#include <fstream>
#include <Eigen/Eigenvalues>
#include <algorithm>
#include <iterator>

int TARGET_NEIGHBORS = 5;
std::string ARCHIVO_ENTRADA("train.csv");
std::string  ARCHIVO_NEIGHBORS("TargetNeighbours.dat");

/**
 * Necesito que el archivo venga en 
 * "exactamente" el mismo formato que 
 * el train de kaggle, se detecta la
 * cantidad de dimensiones
 * */
//weinberger 09 - páginas 33 a 35
using namespace std;

void filtrarNegativos(Matriz& m){
	for(int i = 0; i < m.rows(); i++){
		if(m(i,i)<0){
			m(i,i)=0;
		}
	}
}

void obtenerSalieron(	ConjuntoActiveSets& salieron,
						ConjuntoActiveSets& activos, 
						ConjuntoActiveSets& anterior){
	//salieron = anterior - activos
	set_difference(	anterior.begin(),anterior.end(),
					activos.begin(),activos.end(), 
					inserter(salieron,salieron.end()),
					CompararActiveSet()
	);
}
void obtenerEntraron(	ConjuntoActiveSets& entraron,
						ConjuntoActiveSets& activos, 
						ConjuntoActiveSets& anterior){
	//entraron = activos - anterior
	set_difference(	activos.begin(),activos.end(),
					anterior.begin(),anterior.end(), 
					inserter(entraron,entraron.end()),
					CompararActiveSet()
	);
	//notese que los argumentos están invertidos
}

int main(int argc, char* argv[]){
	cout<<"DIMENSIONES A LEER: "<<DIMENSIONES<<endl;
	ulint cantidad;
	ifstream conf("config.txt");
	conf>>TARGET_NEIGHBORS;
	cout<<TARGET_NEIGHBORS<<endl;
	
	conf>>ARCHIVO_ENTRADA;
	cout<<ARCHIVO_ENTRADA<<endl;
	
	conf>>ARCHIVO_NEIGHBORS;
	cout<<ARCHIVO_NEIGHBORS<<endl;
	
	conf>>cantidad;
	cout<<cantidad<<endl;
	
	if(argc==1){
		cout<<"Y los argumentos?"<<endl;
	}else{
		if(argv[1][0]=='n'){
			cout<<"Recalculando target neighbours"<<endl;
			ArchivoTrain archivo;
			archivo.conectarTargetNeighbors(5,0,cantidad);
			archivo.guardarTargetNeighbours(0,cantidad);
		}else if(argv[1][0]=='m'){
			ofstream off("M.dat");
			cout<<"Voy a calcular la matriz M"<<endl;
			ArchivoTrain archivo;
			ArchivoNeighbors neighbors(archivo);
			
			Matriz m,g;
			m.setIdentity();//initialize with the identity matrix
			ConjuntoActiveSets posibles_activos, activos, anteriores;//initialize active sets
			neighbors.calcularG0(g);//initialize gradient
			
			
			
			for(int i = 0; i<10000; i++){//while not converged do
			
				cout<<"Iniciando nueva iteracion:"<<i<<endl;
				if(i==0){//if mod(i,10)==0 || casi convergencia then
					posibles_activos.clear();
					neighbors.agregarActiveSets(posibles_activos);//compute Nt+1 exactly
					//(es inviable hacer la búsqueda de active sets con la distancia Mahalanobis)
					activos = posibles_activos;//update active set
				}else if(i%100==0){
					posibles_activos.clear();
					neighbors.agregarActiveSets(posibles_activos,m);//compute Nt+1 exactly
					//(es inviable hacer la búsqueda de active sets con la distancia Mahalanobis)
					activos = posibles_activos;//update active set
				}else{
					anteriores = activos;
					neighbors.filtrarActivos(posibles_activos, activos,m);
					//compute Nt+1 only search active set
				}
				cout<<"Posibles activos:"<<activos.size()<<endl;
				
				cout<<"Voy a calcular la G nueva"<<endl;
				
				ConjuntoActiveSets salieron, entraron;
				obtenerSalieron(salieron,activos,anteriores);
				obtenerEntraron(entraron,activos,anteriores);
				cout<<"Ya tengo los que salientraron"<<endl;
				
				Matriz gResta,gSuma;
				neighbors.calcularSegundoTerminoGradiente(gResta,salieron);
				neighbors.calcularSegundoTerminoGradiente(gSuma,entraron);
				g+=g-gResta+gSuma;//sumaresta de gradientes
				
				cout<<"Descmponiendo las matrices"<<endl;
				
				Matriz m_def = m-0.1*g;//Mt-alfa*Gt+1
				Eigen::EigenSolver<Matriz> solver(m_def);
				Matriz delta = solver.pseudoEigenvalueMatrix();
				Matriz v = solver.pseudoEigenvectors();
				filtrarNegativos(delta);//proyección sobre semidefinidas positivas
				m=v*delta*v.transpose();//take gradient step
				
				//guardo la iteracion:
				ostringstream oss;
				oss<<"matrices/"<<DIMENSIONES<<"M"<<i<<".matriz";
				ofstream off(oss.str().c_str());
				off<<m;
				cout<<"Guardo la matriz como "<<oss.str()<<endl;
			}
			
			off<<m;//output Mt
			
		}else if(argv[1][0]=='g'){
			ofstream off("G0.dat");
			cout<<"Voy a calcular la matriz G0"<<endl;
			ArchivoTrain archivo;
			ArchivoNeighbors neighbors(archivo);
			Matriz g0;
			neighbors.calcularG0(g0);
			cout<<"G0 es "<<g0<<endl;
			off<<g0;
		}else if(argv[1][0]=='a'){
			ofstream off("G0.dat");
			cout<<"Voy a calcular los ActiveSets"<<endl;
			Matriz m,g0;
			m.setIdentity();
			ArchivoTrain archivo;
			ArchivoNeighbors neighbors(archivo);
			
			ConjuntoActiveSets caca;
			neighbors.agregarActiveSets(caca);
			cout<<"Activesets: "<<caca.size()<<endl;
		}
	}
	return 0;
}

