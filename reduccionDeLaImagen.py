import numpy as np

def reduccionDeLaImagen(dataSet):
	imagen = np.zeros((28,28))
	reduccion = np.zeros((14,14))

	for r in range(len(dataSet)): 
		lista = dataSet[r]
		k = 0
		for i in range(28):
			for j in range(28):
				imagen[i][j] = lista[k]
				k = k + 1

		#Reduce la imagen a 14x14
		for x in range(0, 26, 2):
			for y in range(0, 26, 2):
				imagen[x][y] += imagen[x][y+1]+imagen[x+1][y]+imagen[x+1][y+1]
				reduccion[x/2][y/2] = imagen[x][y]/4
		
		lista = range(196)
		dataSetNew = []
		for x in range(14):
			for y in range(14):
				lista[x*14+y] = reduccion[x][y]
		dataSetNew.append(lista)

		if (int(float(r)/float(len(dataSet))*100.0) != int(float(r-1)/float(len(dataSet))*100.0)):
			print 'pre-procesamiento imagen : ' + str(int(float(r)/float(len(dataSet))*100.0)) + ' %'
	dataSet = dataSetNew
