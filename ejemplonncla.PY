from pylab import *
import cPickle #Permite guardar y abrir archivos rapidamente
from numpy import *
from reduccionDeLaImagen import reduccionDeLaImagen
import pandas as pd

class nn:
    def sigmoid(self,x):
        return tanh(x)
    # derivada de la funcion sigmoid
    def dsigmoid(self,y):
        #d/dx(tanh(x))=1 - (tanh(x))^2 
        return 1.0-y*y

    def __init__(self, ni, nh, no):
        # numero de nodos entrada, oculta y salidas
        self.ni = ni +1 # +1 para los bias
        self.nh = nh
        self.no = no
       
        # activacion de los nodos
        self.ai = ones((self.ni))
        self.a1 = ones((self.nh))
        self.ao = ones((self.no))
        
        # crear pesos
        self.w1 = random.uniform(-2.0,2.0,(self.ni, self.nh))
        self.w2 = random.uniform(-2.0,2.0,(self.nh, self.no))
        
    def guardar(self,filename):
         W = [self.w1,self.w2]
         cPickle.dump(W,open(filename,'w'))

    def cargar(self,filename):
         W = cPickle.load(open(filename,'r'))
         self.w1=W[0]
         self.w2=W[1]

    def evaluar(self, inputs):
        if len(inputs) != self.ni-1:
            raise ValueError, 'numero erroneo de entradas'

        # activaciones entradas
        self.ai[0:self.ni-1]=inputs

        # acticaciones capa oculta
        # a1=f1(W1*p)
        self.n1 = dot(transpose(self.w1),self.ai)
        self.a1= self.sigmoid(self.n1)

        # activaciones salidas
        # a2=f2(W2* a1)
        self.n2 = dot(transpose(self.w2),self.a1)
        self.ao = self.sigmoid(self.n2)

        return self.ao

    def backPropagate(self, targets, N):
        if len(targets) != self.no:
            raise ValueError, 'numero de objetivos incorrectos'

        #Propagar errores hacia atras        
        # Calcular errores a la salida
        d2=targets-self.ao
        
        # Calcular errores en la capa oculta
        d1 = dot(self.w2,d2)
      
        # Acutalizar pesos de la salida
        #Wnuevo += Wviejo+n*(delta2*f2')*a1
        d2fp= self.dsigmoid(self.ao) * d2
        change = d2fp * reshape(self.a1,(self.a1.shape[0],1))
        self.w2 = self.w2 + N * change

        #Actualizar pesos de las entradas
        #Wnuevo += Wviejo+n*(delta1*f1')*a0
        d1fp =self.dsigmoid(self.a1) * d1
        change = d1fp * reshape(self.ai,(self.ai.shape[0],1))
        self.w1 = self.w1 + N * change

        # calcular error
        error = sum(0.5* (targets-self.ao)**2)
        
        return error

    def test(self, entrada):
        #Imprime la entrada y su salida de la red neuronal
        for p in range(size(entrada,axis=0)):
            print entrada[p,:], '->', self.evaluar(entrada[p,:])

    def singletrain(self,inputs,targets):
        #Realiza una iteracion del entrenamiento backpropagation
        self.evaluar(inputs)
        return self.backPropagate(targets,0.5)
       
    def train(self, entrada, salida, iterations=1000, N=0.5):
        #Realiza entrenamiento backpropagation        
        # N: factor de aprendizaje
        for i in xrange(iterations):
            error = 0.0
            for p in range(size(entrada,axis=0)):
                inputs = entrada[p,:]
                targets = salida[p,:]
                self.evaluar(inputs)
                error = error + self.backPropagate(targets, N)
            if i % 100 == 0 and i!=0:
                print 'error ' + str(error)

def preprocesoIris(infile,outfile):

    stext1 = 'Iris-setosa'
    stext2 = 'Iris-versicolor'
    stext3 = 'Iris-virginica'
    rtext1 = '0'
    rtext2 = '1'
    rtext3 = '2'

    fid = open(infile,"r")
    oid = open(outfile,"w")

    for s in fid:
        if s.find(stext1)>-1:
            oid.write(s.replace(stext1, rtext1))
        elif s.find(stext2)>-1:
            oid.write(s.replace(stext2, rtext2))
        elif s.find(stext3)>-1:
            oid.write(s.replace(stext3, rtext3))
    fid.close()
    oid.close()


#Preprosesar datos
#preprocesoIris('iris.data','iris_proc.data')
#Cargar datos
datos = loadtxt('train14x14.csv',delimiter=',')
# Definion de entradas y salidas
#numero de datos
ndatos=size(datos,axis=0)
#En este caso, las primeras 4 columnas son las entradas
x=datos[:,0:195]
#la salida es la 5 columna
tent=datos[:,195]
tent=transpose(tent)
#Generar salidas individuales
t=zeros((size(tent,0),255))
#10710000
for k in range(255):
    indices = where(tent==k) 
    t[indices,k] = 1
#Crear red neuronal
n=nn(195,10,255)
print "Entrenando"
n.train(x,t,1000,0.05)
print "evaluando la red"
#Evaluar red
y=zeros(shape(tent))
for k in range(ndatos):
    y[k]=argmax(n.evaluar(x[k,:]))
#Calcular porcentaje de acierto
print 'Porcentaje de acierto es: ',float(sum(y==tent))/ndatos

# Plot the data
plot(t,'o')
plot(y,'or')
xlabel('procentaje de acierto')
ylabel('imagenes')

show()