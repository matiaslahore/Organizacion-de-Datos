#esto es pola
# http://ai.stanford.edu/~ang/papers/icml04-onlinemetric.pdf
# siempre lee de entrada.csv, siempre escupe a matriz y a archivob
# cuando cambias el set de datos tenes que correr inicializador.py

import csv
import numpy
import random
import time
import sys
correcciones = 0

def cols(matriz):
	return matriz.shape[1]
def fils(matriz):
	return matriz.shape[0]

def obtenerMenorAvaYSuAve(matriz):
	w,v = numpy.linalg.eig(matriz)
	menor = 0
	for i in range(fils(w)):
		if(not isinstance(w[i], complex) and w[i]<w[menor]):
			menor = i
	return w[menor],v[:,menor]

class Imagen:
	def __init__(self,label,representacion):
		self.label = label
		self.representacion = representacion
	def distanciaA(self,otra,matrizPI):
		diferencia = self.representacion - otra.representacion
		return numpy.transpose(diferencia).dot(matrizPI.dot(diferencia))

class Pola:
	def perdida(self,img1,img2,y):
		dist = img1.distanciaA(img2,self.matriz)
		return max(0,y*(dist-self.b)+1)
		
	def __init__(self,dimensiones):
		#cargar matriz
		archivoM = open("matriz","rb")
		reader = csv.reader(archivoM,delimiter=',')
		x = list(reader)
		self.matriz = numpy.array(x).astype('float')
		archivoM.close()
		#cargar b
		archivob = open("archivob","rb")
		self.b = float(archivob.readline())
		archivob.close()
	def introducir(self,img1,img2):
		v = img1.representacion - img2.representacion
		if numpy.linalg.norm(v)==0 : return
		y = 1 if img1.label == img2.label else -1
		perdida = self.perdida(img1,img2,y)
		if perdida>0 :
			global correcciones
			correcciones += 1
			
			alfa = perdida/(1+numpy.linalg.norm(v)**4)
			
			aTecho = self.matriz - y * alfa * numpy.outer(v,v)
			bTecho = self.b + y * alfa

			if y==1 :
				self.b = bTecho
				lambda_n, u_n = obtenerMenorAvaYSuAve(self.matriz)
				
				if lambda_n<0:
					self.matriz = self.matriz - lambda_n*numpy.outer(u_n,u_n)
				else:
					self.matriz = aTecho
			else:
				self.b = max(bTecho,1)
				self.matriz = aTecho
		
class Archivo:
	def __init__(self):
		reader = csv.reader(open("entrada.csv","rb"),delimiter=',')
		x = list(reader)
		archivo = numpy.array(x[1:]).astype('float')
		self.labels=archivo[:,0]
		self.imagenes=archivo[:,1:]
	def cantidad(self):
		return fils(self.imagenes)
	def hacerImagen(self,posicion):
		return Imagen(self.labels[posicion],self.imagenes[posicion,:])
	def posRandom(self):
		return random.randint(0,self.cantidad()-1)
	def hacerImagenRandom(self):
		return self.hacerImagen(self.posRandom())
	def dimensiones(self):
		return cols(self.imagenes)

#print(numpy.outer([2,1],[1,1]))
if(len(sys.argv)<2):
	print("el argumento es el numero de segundos que queres que compute")
else:
	correcciones = 0
	print("-------------------------------")
	print(sys.argv[0])
	print(sys.argv[1])

	archivo = Archivo()
	pola = Pola(archivo.dimensiones())
	inicio = time.clock()
	ultimo = inicio
	vueltas = 0

	while time.clock() - inicio < int(sys.argv[1]):
		vueltas += 1
		uno=archivo.hacerImagenRandom()
		dos=archivo.hacerImagenRandom()
		pola.introducir(uno,dos)
		if(time.clock() - ultimo>5):
			global correcciones
			ultimo = time.clock()
			print("van "+str(ultimo-inicio)+" de "+sys.argv[1])
			print("correcciones hasta ahora: "+str(correcciones))

			#guardar la matriz
			numpy.savetxt('matriz',pola.matriz,delimiter=',')

			#guardar b
			archivob = open("archivob","w")
			archivob.write(str(pola.b))
			archivob.close()

	#guardar la matriz
	numpy.savetxt('matriz',pola.matriz,delimiter=',')

	#guardar b
	archivob = open("archivob","w")
	archivob.write(str(pola.b))
	archivob.close()
