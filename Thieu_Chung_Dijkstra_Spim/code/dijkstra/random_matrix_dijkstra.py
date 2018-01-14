from random import randint
from numpy import random, arange
import sys
MAX = 1000000
MIN = 0
def randomint(begin, end):
    return randint(begin, end)
def createfile(filepath, numvertical):

    a = [[0 for x in range(numvertical)] for y in range(numvertical)]
    for i in range(0,numvertical):
        for j in range(0, numvertical):
            if i != j:               
                if numvertical < 200:   #  64, 128
                    if random.choice(arange(0,2), p=[0.30, 0.70]) == 0:
                        a[i][j] = MAX
                    else:
                        a[i][j] = randomint(1,999)
                elif numvertical < 800: # 256, 512
                    if random.choice(arange(0,2), p=[0.40, 0.60]) == 0:
                        a[i][j] = MAX
                    else:
                        a[i][j] = randomint(1,999)
                elif numvertical < 2500: # 1024, 2048
                    if random.choice(arange(0,2), p=[0.50, 0.50]) == 0:
                        a[i][j] = MAX
                    else:
                        a[i][j] = randomint(1,999)
                elif numvertical < 10000:   # 4096, 8192
                    if random.choice(arange(0,2), p=[0.30, 0.70]) == 0:
                        a[i][j] = MAX
                    else:
                        a[i][j] = randomint(1,9999)
                else:   # 16384
                    if random.choice(arange(0,2), p=[0.35, 0.65]) == 0:
                       a[i][j] = MAX
                    else:
                        a[i][j] = randomint(1,9999)
                        
    file = open(filepath,"w")
    file.write(str(numvertical)+"\n")
    
    for i in range(0,numvertical):
        for j in range(0, numvertical):
            file.write(str(a[i][j]))
            file.write(" ")
        file.write("\n")
    file.close()
    del a
if __name__ == '__main__':
    if (len(sys.argv) < 3):
        print ('You need to put the file_path and number_of_node_in_graph.')
    else:
        numvertical = int(sys.argv[2])
        filepath = sys.argv[1]
        createfile(filepath=filepath, numvertical=numvertical)
