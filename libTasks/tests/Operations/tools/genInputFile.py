#encoding: utf-8

import numpy as np

buffFile="buffer.txt"

outFileName="inputFile.dat"


N=int(5.0E+05)
#N=500

shape=(N,2)
dtype=np.float32
mode='w+'

arr=np.memmap(buffFile, dtype=dtype, shape=shape, mode=mode)

arr= np.random.uniform(-10000,10000,(N,2)).astype(dtype)

f=open(outFileName, 'ab')
np.savetxt(outFileName,arr)
f.close()
