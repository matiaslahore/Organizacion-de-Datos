import pandas as pd
from reduccionDeLaImagen import reduccionDeLaImagen

dataset = pd.read_csv("train.csv")
target = dataset[[0]].values.ravel()
train = dataset.iloc[:,1:].values
test = pd.read_csv("test.csv").values

#imprime 784, imagen 28x28
print len(train[0])

trainReducido = reduccionDeLaImagen(train)

#imprime 196, imagen 14x14
print len(trainReducido[0])

#desomentar para reducir el set de test tambien
#testReducido= reduccionDeLaImagen(test)

