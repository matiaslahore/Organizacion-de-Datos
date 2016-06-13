#include "ArchivoTrain.h"
#include "Imagen.h"
#include <cstdlib>
#include <ctime>
void ArchivoTrain::parsearFila(const string& fila,vector<float>& en){
	if(fila.size()==0){
		throw string("Tamanio invalido de fila");
	}
	istringstream stream(fila);
	char numero[50];
	stream.getline(numero,50,',');//el primer número es el label
	
	while(!stream.eof() && en.size()<DIMENSIONES){//esto corta la cola
		stream.getline(numero,50,',');
		istringstream stream_numero(numero);
		float obtenido;
		stream_numero>>obtenido;
		en.push_back(obtenido);
	}
}
void ArchivoTrain::parsearPosicion(streampos pos,vector<float>& en){
	entrada.open("entrada.csv");//así gunciona seekg (???????)
	char fila[2048];
	entrada.seekg(pos,ios_base::beg).getline(fila,2048);
	//cout<<"Desde posicon: "<<pos<<endl;
	//cout<<"esta es:"<<entrada.seekg(pos,ios_base::beg).tellg()<<endl;
	parsearFila(fila,en);
	entrada.close();
}


int ArchivoTrain::detectarDimensiones(const string& primera_linea){
	istringstream stream(primera_linea);
	int dimensiones = 0;
	char dummy[50];
	while(!stream.eof()){
		stream.getline(dummy,50,',');
		dimensiones+=1;
	}
	return dimensiones-1;//la primera columna es "label!!"
}

ArchivoTrain::ArchivoTrain(){
	entrada.open("entrada.csv");
	//----------inicializar la cantidad de dimensiones-------//
	string primera_linea;
	getline(entrada,primera_linea);
	dimensiones = detectarDimensiones(primera_linea);
	cout<<"Dimensiones: "<<dimensiones<<endl;
	//----------generar las entradas
	
	string linea_entrada;
	while(!entrada.eof()){
		streampos posicion = entrada.tellg();
		//std::cout<<posicion<<endl;
		getline(entrada, linea_entrada);
		Entrada nueva(posicion, linea_entrada);
		entradas.push_back(nueva);
	}
	entrada.close();//así gunciona seekg (???????)
	
}

double ArchivoTrain::distancia(Entrada& e1, Entrada& e2){
	//cout<<"a"<<endl;
	Imagen img1(*this,e1), img2(*this,e2);
	return img1.euclideanaCuadrada(img2);
}

void ArchivoTrain::conectarTargetNeighbors(int k,ulint desde, ulint hasta){
	time_t tiempo_inicio = clock();
	vector<Entrada>::iterator it;
	long int procesadas = 0;
	ulint total = hasta-desde;
	//for(it = entradas.begin(); it!=entradas.end(); ++it){
	for(ulint i =desde; i<hasta; ++i){
		conectarleTargetNeighbors(entradas[i],k);
		time_t tiempo_actual = clock();
		float segundos = (float)(tiempo_actual - tiempo_inicio) / CLOCKS_PER_SEC;
		float proporcion = (float)(i-desde)/(float)total;
		float estimacion = segundos/proporcion;
		cout<<"Procesadas: "<<proporcion*100<<"%"<<"Segundos estimados:"<<estimacion<<"Horas: "<<estimacion/3600<<endl;
	}
}

void ArchivoTrain::conectarleTargetNeighbors(Entrada& a,int k){
	ifstream entrada;
	a.setCantidadTargetNeighbors(k);
	Imagen img_a(*this,a);
	vector<Entrada>::iterator it;
	entrada.open("entrada.csv");
	long unsigned int pos_final = 0;
	entrada.seekg(entradas[0].posicion);
	
	while(pos_final<entradas.size()-1){
		//cout<<pos_final<<endl;
		long unsigned int siguiente = pos_final +100;//constante mágica, 30...60 son óptimos
		if(siguiente>=entradas.size())siguiente=entradas.size()-1;
		//neighborsEnBloque(img_a,pos_final,siguiente);
		//pos_final = siguiente;
		pos_final = neighborsEnBloque(img_a,pos_final,siguiente,entrada);
	}
	//cout<<"LLegue hasta: "<<pos_final<<" No es menor que: "<<entradas.size()-1<<endl;
	
	//neighborsEnBloque(img_a,0,80);
	//cout<<"Entradas sze: "<<entradas.size()<<" pos final: "<<pos_final<<endl;
	entrada.close();
}

long unsigned int ArchivoTrain::neighborsEnBloque(Imagen& a, ulint p_inicial, ulint p_final,ifstream& entrada){
	
	//cout<<"Pos inicial: " <<entrada.tellg()<<endl;
	//cargar el bloque en el char*
	streampos tamanio_buffer =entradas[p_final].posicion - entradas[p_inicial].posicion;
	char* buf = (char*) malloc((tamanio_buffer)*sizeof(char));
	entrada.get(buf,tamanio_buffer,'j');//no hay jotas
	//me desplazo un caracter más
	char dummy;
	entrada.get(dummy);
	//estadísticas
	/*
	cout<<"Pos final: " <<entrada.tellg()<<endl;
	string s(buf),h;
	//cout<<buf<<s<<"  "<<tamanio_buffer<<"  "<<s.size()<<endl;
	cout<<tamanio_buffer<<"  "<<s.size()<<endl;
	cout<<"Arranqué en:" <<entradas[p_inicial].posicion<<" termino en: "<<entradas[p_final].posicion;
	cin>>h;
	*/
	
	istringstream oss(buf);
	string fila;
	for(long unsigned int i = p_inicial; i<p_final;++i){
		getline(oss,fila);
		
		if(fila.size()!=0){
			//cout<<(int)entradas[i].label<<" "<<(int)a.entrada.label<<endl;
			if(entradas[i].label == a.entrada.label){
				Imagen img_leida(*this,fila,entradas[i]);
				
				double distancia_entre = a.euclideanaCuadrada(img_leida);
				a.entrada.posibleTargetNeighbor(entradas[i],distancia_entre);
			}
		}
	}
	free(buf);
	
	return p_final;
}

void ArchivoTrain::guardarTargetNeighbours(ulint desde, ulint hasta){
	ofstream salida;
	salida.open("TargetNeighbours.dat");
	/*
	vector<Entrada>::iterator it;
	//for(it=entradas.begin(); it!=entradas.end(); ++it){
	for(it = entradas.begin(); (&(*it))!=&entradas[100]; ++it){
		it->agregarEn(salida);
	}
	* */
	for(ulint i = desde; i<hasta; ++i){
		entradas[i].agregarEn(salida);
	}
}

void ArchivoTrain::cargarImagenes(map<ulint,Imagen*>& cargadas){
	vector<Entrada>::iterator it;
	for(it = entradas.begin(); it!=entradas.end(); ++it){
		try{
			cargadas[it->posicion]=new Imagen(*this,*it);
			
		}catch(string& error){
			cout<<"Se ignora la fila: "<<it->posicion;
		//la última línea se lee mal y no debe ser parseada, ni idea	
		}
		
	}
}
