import pandas as pd
import cv2
import numpy as np
from pylab import *

dataset = pd.read_csv("train14x14.csv")
train = dataset.iloc[:,1:].values


def vectorToMatrix(vec):
    matrizDatos = np.arange(196).reshape(14, 14)
    k = 0
    for i in range(14):
        for j in range(14):
            matrizDatos[i][j] = vec[k]
            k = k+1
    matrizDatos = matrizDatos.astype(np.uint8)
    return matrizDatos

def compute_skew(image):
    #image = cv2.bitwise_not(image)
    height = 14
    width = 14

    edges = image
    lines = cv2.HoughLinesP(edges, 1, cv2.cv.CV_PI/180, 100, minLineLength=width / 2.0, maxLineGap=14)
    if(not lines):
        return None
    angle = 0.0
    nlines = lines.size
    for x1, y1, x2, y2 in lines[0]:
        angle += np.arctan2(x2 - x1, y2 - y1)
    return angle / nlines


def deskew(image, angle):
    image = cv2.bitwise_not(image)
    non_zero_pixels = cv2.findNonZero(image)
    center, wh, theta = cv2.minAreaRect(non_zero_pixels)

    root_mat = cv2.getRotationMatrix2D(center, angle, 1)
    cols, rows = image.shape
    rotated = cv2.warpAffine(image, root_mat, (cols, rows), flags=cv2.INTER_CUBIC)

    return cv2.getRectSubPix(rotated, (cols, rows), center)


for i in range(1000):
    matrizDatos = vectorToMatrix(train[i])
    computeSkew = compute_skew(matrizDatos)

    if computeSkew:
        deskewed_image = deskew(matrizDatos.copy(),computeSkew)
        print deskewed_image
    else:
        print "no hay lineas"