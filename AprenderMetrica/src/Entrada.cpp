#include "Entrada.h"
#include "config.h"
Entrada::Entrada(streampos posicion, string desde){
	this->posicion = posicion;
	
	char numero[4];//con uno o 2 me alcanzaría...
	istringstream stream(desde);
	stream.getline(numero,4,',');
	istringstream stream_numero(numero);
	int obtenido;
	stream_numero>>obtenido;
	label = (char) obtenido;
}

Entrada* Entrada::entradaMasLejana(){
	double mayorDistancia = -1;
	Entrada* masLejana = NULL;
	map<Entrada*,double>::iterator it;
	for(it = targets.begin(); it!=targets.end(); ++it){
		if(mayorDistancia == -1 || mayorDistancia < it->second){
			mayorDistancia = it->second;
			masLejana = it->first;
		}
	}
	return masLejana;
}

void Entrada::posibleTargetNeighbor(Entrada& otra,double distancia_entre){
	Entrada* masLejana = entradaMasLejana();
	if(otra.posicion!=posicion){
		if(targets.size() < TARGET_NEIGHBORS){
			targets[&otra]=distancia_entre;
			//cout<<"Agrego a "<<posicion<<" el "<<otra.posicion<<endl;
		}else{
			double distancia_mas_lejana = targets[masLejana];
			if(distancia_mas_lejana > distancia_entre){
				targets.erase(masLejana);
				targets[&otra]=distancia_entre;
			//	cout<<"Agrego a "<<posicion<<" el "<<otra.posicion<<endl;
			}
			
		}
	}
	
}
void Entrada::agregarEn(ofstream& en){
	cout<<"----largo: "<<targets.size()<<"soy un "<<(int)label<<endl;
	en<<posicion<<" ";
	cout<<posicion<<" ";
	map<Entrada*,double>::iterator it;
	for(it = targets.begin(); it != targets.end(); ++it){
		en<<(it->first)->posicion<<" ";
		cout<<(it->first)->posicion<<" ";
	}
	en<<endl;
	cout<<endl;
}
