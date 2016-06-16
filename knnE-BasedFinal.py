import numpy as np
from sklearn.neighbors import BallTree
import pandas as pd
import random
import operator
from scipy import spatial
import csv
import sys

archivoM = open("/home/nicolas/Organizacion-de-Datos/matrices/mejor matriz POLA 14x14","rb")
reader = csv.reader(archivoM,delimiter=',')
x = list(reader)
M = np.array(x).astype('float')
archivoM.close()

def mahalanobisDistance(x,y):
	return spatial.distance.mahalanobis(x, y, M)

def obtenerPrediccionknnEB(train, targetTrain, puntoTest,ind):
	labelVecinos=[]
	vecinos=[]
	for i in ind:
		vecinos.append(train[i])
		labelVecinos.append(targetTrain[i])
	vecinos.append(puntoTest)		
	labels = list(set(labelVecinos))
	perdidas = []
	for label in labels:
		labelVecinos.append(label)
		indices=range(len(vecinos))
		targetNeighbours=[]
		for i in indices:
	  		for j in indices:
	  			if(i!=j and labelVecinos[i] == labelVecinos[j]):
	  				targetNeighbours.append((i,j))		      	
		triplas = []
		for dupla in targetNeighbours:
			for l in indices:
				if(mahalanobisDistance(vecinos[dupla[0]],vecinos[dupla[1]]) <= mahalanobisDistance(vecinos[dupla[0]],vecinos[l]) + 1313486.48934) and (labelVecinos[dupla[0]]!=labelVecinos[l] and labelVecinos[dupla[1]] != labelVecinos[l]):
					triplas.append((dupla[0], dupla[1], l))
		ePull = 0
		for dupla in targetNeighbours:
	  		ePull += mahalanobisDistance(vecinos[dupla[0]],vecinos[dupla[1]])

		ePush = 0
		for tripla in triplas:
	  		ePush += 1313486.48934 + mahalanobisDistance(vecinos[tripla[0]],vecinos[tripla[1]]) - mahalanobisDistance(vecinos[tripla[0]],vecinos[tripla[2]])
	  
		perdida = 0.5 *ePull + 0.5 *ePush
		
		perdidas.append((label,perdida))

		labelVecinos.pop()

	return min(perdidas,key=operator.itemgetter(1))[0]	

def guardarPrediccionesParaKaggle(predictions):
	print "escribiendo predicciones"
	with open('predicciones.csv', 'w') as csvfile:
	    fieldnames = ['ImageId', 'Label']
	    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
	    writer.writeheader()
	    for i in range(len(predictions)):
	    	writer.writerow({'ImageId': str(i+1), 'Label': str(predictions[i])})

def progreso(parcial,total):
	return str(int((float(parcial)/float(total) * 100.0))) + '%'

def cargarDatosParaKaggle(set_ampliado):

	dataset = pd.read_csv("train14x14.csv")
	target = dataset[[0]].values.ravel()
	trainSet = dataset.iloc[:,1:].values
	if set_ampliado:
		dataset = pd.read_csv("/home/nicolas/Escritorio/trainAmpliado14x14.csv")
		trainSet = np.concatenate((trainSet,dataset.iloc[:,1:].values))
		target = np.concatenate((target,dataset[[0]].values.ravel()))
	
	testSet = pd.read_csv("test14x14.csv").values

	#solo lo hago por un warning que tiraba para las pruebas de kaggle
	train = []
	targetTrain=[]
	test = []

	for x in range(len(trainSet)):
		train.append(trainSet[x])
		targetTrain.append(target[x])
    
	for x in range(len(testSet)):
		test.append(testSet[x])

	train = np.matrix(train)
	targetTrain=np.array(targetTrain)
	test=np.matrix(test)

	return train,targetTrain,test

def cargarDatosPruebaLocal(set_ampliado,separacionSet):
	dataset = pd.read_csv("train14x14.csv")
	target = dataset[[0]].values.ravel()
	trainSet = dataset.iloc[:,1:].values

	if set_ampliado:
		dataset = pd.read_csv("/home/nicolas/Escritorio/trainAmpliado14x14.csv")
		trainSet = np.concatenate((trainSet,dataset.iloc[:,1:].values))
		target = np.concatenate((target,dataset[[0]].values.ravel()))

	train = []
	targetTrain=[]
	targetTest=[]
	test = []

	for x in range(len(trainSet)):
	    if (random.random() < 0.66):
	    	train.append(trainSet[x])
	    	targetTrain.append(target[x])
	    else:
	        test.append(trainSet[x])
	        targetTest.append(target[x])

	train = np.matrix(train)

	targetTrain=np.array(targetTrain)
	targetTest=np.array(targetTest)
	test=np.matrix(test)

	return train, targetTrain, test,targetTest	

def correrPruebaParaKaggle(set_ampliado):
	print "corriendo pruebas para kaggle"

	train,targetTrain,test = cargarDatosParaKaggle(set_ampliado)


	tree = BallTree(train,leaf_size=30) 
	predictions=[]
	for x in range(len(test)):
		dist, ind = tree.query(test[x], k=4)
		resultado = obtenerPrediccionknnEB(train,targetTrain,test[x],ind.ravel())
		predictions.append(resultado)
		print progreso(x,len(test))

	guardarPrediccionesParaKaggle(predictions)


def correrPruebaLocal(set_ampliado):
	
	print "corriendo prueba local"

	train,targetTrain,test,targetTest = cargarDatosPruebaLocal(set_ampliado,0.66)

	tree = BallTree(train,leaf_size=30) 
	predictions=[]
	correctas=0
	incorrectas=0
	for x in range(len(test)):
		dist, ind = tree.query(test[x], k=4)
		resultado = obtenerPrediccionknnEB(train,targetTrain,test[x],ind.ravel())
		predictions.append(resultado)
		print progreso(x,len(test))	
		if resultado==targetTest[x]: 
			correctas+=1
		else:
			incorrectas+=1
		print "Predicciones -->  Correctas: " + str(correctas) + "Incorrectas: " + str(incorrectas)+ "Total: "+ str(len(test))
		print('> predicted=' + repr(resultado) + ', actual=' + repr(targetTest[x]) + ' ' + progreso(x,len(test)) )
	print "precision total"
	correct = 0
	for x in range(len(test)):
		if targetTest[x] == predictions[x]:
			correct += 1
	print (float(correct)/float(len(test))) * 100.0


def main():

	if (sys.argv<2) or (sys.argv[1] not in ["local","kaggle"]):
		print "ingresar argumento local o kaggle"
	
	if sys.argv[1]== "local":
		correrPruebaLocal(set_ampliado=True)
	else:	
		correrPruebaParaKaggle(set_ampliado=True)

main()
