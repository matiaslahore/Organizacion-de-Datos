import pandas as pd
import cv2
import numpy as np
from pylab import *
import matplotlib.pyplot as plt
import random
import csv


dataset = pd.read_csv("train.csv")
train = dataset.iloc[:,1:].values
target = dataset[[0]].values.ravel()

def vectorToMatrix(vec):
    return np.reshape(vec,(28,28)).astype(np.uint8)

h = 28
w = 28
center = (w/2.0, h/2.0)

setAmpliado = []

porcentaje = -1

for i in range(len(train)):
    for angulo in [-30,-20,-10,10,20,30]:    
        matrizDatos = vectorToMatrix(train[i])
        M = cv2.getRotationMatrix2D(center,angulo,round(random.uniform(0.6,1.0),2))
        rotated = cv2.warpAffine(matrizDatos,M,(w,h))
        rotated = np.reshape(rotated,(784))
        setAmpliado.append(np.concatenate(([target[i]],rotated)))
        if (int(float(i)/float(len(train))*100.0) != int(float(i-1)/float(len(train))*100.0)):
            print str(int(float(i)/float(len(train))*100.0)) + ' %'
        #plt.matshow(np.reshape(rotated,(28,28)),cmap=plt.cm.gray)
        #plt.show()

h = range(784)
h = map(lambda x: "Pixel"+str(x), h)
header = ["Label"] + h

with open('trainAmpliado.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    spamwriter.writerow(header)
    for vec in setAmpliado:
        spamwriter.writerow(map(lambda x: int(x),vec))

