from random import randint
from numpy import random, arange, array
import numpy as np
import sys
MIN = 0
def randomint(begin, end):
    return randint(begin, end)
def createfile(filepath, numvertical):
    a = [[0 for x in range(numvertical)] for y in range(numvertical)]
    for i in range(0,numvertical):
        for j in range(0, numvertical):
            if i != j:
                if numvertical < 200:   #  64, 128
                    if random.choice(arange(0,2), p=[0.50, 0.50]) == 0:
                        a[i][j] = randomint(1,999)
                elif numvertical < 800: # 256, 512
                    if random.choice(arange(0,2), p=[0.40, 0.60]) == 0:
                        a[i][j] = randomint(1,999)
                elif numvertical < 2500: # 1024, 2048
                    if random.choice(arange(0,2), p=[0.20, 0.80]) == 0:
                        a[i][j] = randomint(1,999)
                elif numvertical < 10000:    # 4096, 8192
                    if random.choice(arange(0,2), p=[0.15, 0.85]) == 0:
                        a[i][j] = randomint(1,999)
                else:   # 16384
                    if random.choice(arange(0,2), p=[0.10, 0.90]) == 0:
                        a[i][j] = randomint(1,999)

    a = np.array(a)
    a_sym = np.tril(a) + np.tril(a, -1).T

    count_number_dif_zero = np.count_nonzero(a_sym)
    number_of_edges = count_number_dif_zero / 2

    file1 = open(filepath + ".txt","w")
    file1.write(str(numvertical)+"\n")
    for i in range(0,numvertical):
        for j in range(0, numvertical):
            file1.write(str(a_sym[i][j]))
            file1.write(" ")
        file1.write("\n")
    file1.close()

    file2 = open(filepath + "_mpi.txt", "w")
    file2.write(str(numvertical) + " " + str(number_of_edges) + "\n")
    for i in range(numvertical):
        for j in range(i+1, numvertical):
            if a_sym[i][j] != 0:
                file2.write(str(i) + " " + str(j) + " " + str(a_sym[i][j]) + "\n")
    file2.close()

    del a
    del a_sym
if __name__ == '__main__':
    if (len(sys.argv) < 3):
        print ('You need to put the file_path and number_of_node_in_graph.')
    else:
        numvertical = int(sys.argv[2])
        filepath = sys.argv[1]
        createfile(filepath=filepath, numvertical=numvertical)