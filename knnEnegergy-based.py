import numpy as np
from sklearn.neighbors import BallTree
import pandas as pd
import random
import operator
from scipy import spatial
import csv


archivoM = open("/home/nicolas/Organizacion-de-Datos/matrices/mejor matriz POLA 14x14","rb")
reader = csv.reader(archivoM,delimiter=',')
x = list(reader)
M = np.array(x).astype('float')
archivoM.close()

def getResponse(neighbors):
	classVotes = {}
	for x in range(len(neighbors)):
		response = neighbors[x]
		if response in classVotes:
			classVotes[response] += 1
		else:
			classVotes[response] = 1
	sortedVotes = sorted(classVotes.iteritems(), key=operator.itemgetter(1), reverse=True)
	return sortedVotes[0][0]

def mahalanobisDistance(x,y):
	return spatial.distance.mahalanobis(x, y, M)
	#return np.linalg.norm(x-y)
def obtenerPrediccion(indices,query):
	distancias=[]
	for i in indices:
		distancias.append((targetTrain[i],mahalanobisDistance(train[i],query)))
	distOrdenadas = sorted(distancias, key = operator.itemgetter(0))        
	return distOrdenadas[0][0]
'''	
dataset = pd.read_csv("train14x14.csv")
targetTrain = dataset[[0]].values.ravel()
train = dataset.iloc[:,1:].values
test = pd.read_csv("test14x14.csv").values
'''

dataset = pd.read_csv("/home/nicolas/Escritorio/train14x14.csv")
targetSet = dataset[[0]].values.ravel()
trainingSet = dataset.iloc[:,1:].values
dataset = pd.read_csv("/home/nicolas/Escritorio/trainAmpliado14x14.csv")
train = np.concatenate((trainingSet,dataset.iloc[:,1:].values))
targetTrain = np.concatenate((targetSet,dataset[[0]].values.ravel()))
test = pd.read_csv("test14x14.csv").values


print test.shape
print test[0]
'''
# PARA HACER PRUEBA LOCAL
train = []
targetTrain=[]
targetTest=[]
test = []

for x in range(len(trainingSet)):
    if (random.random() < 0.66):
    	train.append(trainingSet[x])
    	targetTrain.append(targetSet[x])
    else:
        test.append(trainingSet[x])
        targetTest.append(targetSet[x])

train = np.matrix(train)

targetTrain=np.array(targetTrain)
targetTest=np.array(targetTest)
test=np.matrix(test)

#train = train[:,:181]
#test = test[:,:181]
'''
tree = BallTree(train,leaf_size=30) 
predictions=[]
correctas = 0
for x in range(len(test)):
	dist, ind = tree.query(test[x], k=4)
	labelVecinos=[]
	vecinos=[]
	for i in ind.ravel():
		#if targetTrain[i] not in labelVecinos:
		vecinos.append(train[i])
		labelVecinos.append(targetTrain[i])
	vecinos.append(test[x])		
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

	result = min(perdidas,key=operator.itemgetter(1))[0]
	#result = obtenerPrediccion(ind.ravel(),test[x])
	predictions.append(result)
	print str(int((float(x)/float(len(test))) * 100.0)) + '%'
'''	
	if result==targetTest[x]: correctas+=1
	print "Predicciones -->  Correctas: " + str(correctas) + "Incorrectas: " + str(len(test) - correctas)+ "Total: "+ str(len(test))
	print('> predicted=' + repr(result) + ', actual=' + repr(targetTest[x]) + ' ' + str(int((float(x)/float(len(test))) * 100.0)) + '%' )
#accuracy = getAccuracy(testSet, predictions)
print "precision total"
correct = 0
for x in range(len(test)):
	if targetTest[x] == predictions[x]:
		correct += 1
print (float(correct)/float(len(test))) * 100.0
#print('Accuracy: ' + repr(accuracy) + '%')
'''

print "escribiendo predicciones"

with open('predicciones.csv', 'w') as csvfile:
    fieldnames = ['ImageId', 'Label']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()
    for i in range(len(predictions)):
    	writer.writerow({'ImageId': str(i+1), 'Label': str(predictions[i])})