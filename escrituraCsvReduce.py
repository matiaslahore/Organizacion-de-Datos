import csv
import pandas as pd
from reduccionDeLaImagen import reduccionDeLaImagen

dataset = pd.read_csv("train.csv")
target = dataset[[0]].values.ravel()
train = dataset.iloc[:,1:].values
test = pd.read_csv("test.csv").values

trainReducido = reduccionDeLaImagen(train)

testReducido = reduccionDeLaImagen(test)

h = range(49)
h = map(lambda x: "Pixel"+str(x), h)
header = ["Label"] + h

with open('train7x7.csv', 'wb') as csvfile:
	spamwriter = csv.writer(csvfile, delimiter=',')
	spamwriter.writerow(header)
	for indice, linea in enumerate(trainReducido):
		linea = map(lambda x: int(x),linea)
		lista = []
		lista.append(target[indice])
		lista = lista + linea
		spamwriter.writerow(lista)

header = range(49)
header = map(lambda x: "Pixel"+str(x), header)

with open('test7x7.csv', 'wb') as csvfile:
	spamwriter = csv.writer(csvfile, delimiter=',')
	spamwriter.writerow(header)
	for indice, linea in enumerate(testReducido):
		linea = map(lambda x: int(x),linea)
		spamwriter.writerow(linea)
