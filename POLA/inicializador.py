import csv
import numpy
import random
import sys

def cols(matriz):
	return matriz.shape[1]
def fils(matriz):
	return matriz.shape[0]

def obtenerMenorAvaYSuAve(matriz):
	w,v = numpy.linalg.eig(matriz)
	menor = 0
	for i in range(fils(w)):
		if(w[i]<w[menor]):
			menor = i
	return w[menor],v[:,menor]

class Imagen:
	def __init__(self,label,representacion):
		self.label = label
		self.representacion = representacion
	def distanciaA(self,otra,matrizPI):
		diferencia = self.representacion - otra.representacion
		return numpy.transpose(diferencia).dot(matrizPI.dot(diferencia))
		
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


archivo = Archivo()
identidad = numpy.identity(archivo.dimensiones())
dist = 0
for i in range(100):
	uno=archivo.hacerImagenRandom()
	dos=archivo.hacerImagenRandom()
	dist += uno.distanciaA(dos,identidad)/100

#guardar la matriz
numpy.savetxt('matriz',identidad,delimiter=',')

#guardar b
archivob = open("archivob","w")
archivob.write(str(dist))
