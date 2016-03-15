Matrix2 = [[0 for x in range(2)] for x in range(2)]
Matrix = [[0 for x in range(4)] for x in range(4)]
promedio = [0 for x in range(4)]

Matrix[0][0] = 0
Matrix[0][1] = 1
Matrix[0][2] = 2
Matrix[0][3] = 3
Matrix[1][0] = 0
Matrix[1][1] = 1
Matrix[1][2] = 2
Matrix[1][3] = 3
Matrix[2][0] = 0
Matrix[2][1] = 1
Matrix[2][2] = 2
Matrix[2][3] = 3
Matrix[3][0] = 0
Matrix[3][1] = 1
Matrix[3][2] = 2
Matrix[3][3] = 3

print Matrix
print

for i in range(len(Matrix)):
	for j in range(len(Matrix[i])):
		promedio[j] += Matrix[i][j]

z = 0
for i in range(len(Matrix2)):
	for j in range(len(Matrix2[i])):
		Matrix2[i][j] = promedio[z]
		z += 1

print Matrix2	

http://conociendogithub.readthedocs.org/en/latest/data/dinamica-de-uso/