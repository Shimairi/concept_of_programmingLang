# Airi Shimamura R11652198
# Final Project - Cellular Simulation
# Create a program to execute the first 100 setps of a midified celluar life simulator 
# then save the result in an output file 
from multiprocessing import Pool
import os
import sys
import argparse
import os.path
import itertools
import time


def main():
    start = time.perf_counter()
    # command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', dest='in_file', required=True, type= str)
    parser.add_argument('-o', dest='out_file', required=True, type= str)
    parser.add_argument("-t", type = int, dest = 'numThreads', default = 1)
    args = parser.parse_args()
    inputFile= args.in_file
    outFile= args.out_file
    numThreads = args.numThreads
    directoryName =  os.path.dirname(args.out_file)

    if not os.path.exists(directoryName) and directoryName:
        exit("Directory not exist")
    # when the threadnumber is less than 0
    if numThreads <= 0:
        exit("Error: number of threads must be a positive integer")
    # when input file is not exit  
    if  not os.path.exists(args.in_file):
        exit("File does not exist")
    
    matrix = list()
    # reading the inputfile line by line 
    with open(inputFile, 'r') as rw:
        line = rw.readlines()
    # save the matrix into a list 
    for row in line:
        matrix.append(row.rstrip())    

    # count rown number of the matrix
    rowNum = len(matrix) 
    # count column number for a row 
    colNum = len(matrix[0]) 
    print("Project :: R11652198\n")

    row = [x for x in range(rowNum)]
    col = [x for x in range(colNum)]

    # creating new matrix to save the result 
    processPool = Pool(processes=args.numThreads)
    taskzSize = int((rowNum*colNum)/ args.numThreads)

    for x in range(100):

        data = [(matrix, i, j) for i in row for j in col]
        finalData = processPool.map(pool_handler, data, chunksize=taskzSize)
        
        del(data)
        matrix = [finalData[i:i+colNum] for i in range(0, len(finalData), colNum)]  

    wf = open(outFile,'w')
    for x in range(rowNum):
        wf.write(''.join(matrix[x]) + '\n')

    end = time.perf_counter()
    print(round(end-start,2))

def pool_handler(val):
        return count_alive(val[0], val[1], val[2])

    #  counting alive cells and apply to the rules 
def count_alive(matrix, x, y):
    rownum = len(matrix)
    colnum = len(matrix[0])
    cnt_alive = 0
     # check if there is no invalid character in the matrix 
    if not matrix[x][y] == '.' and  not matrix[x][y] == 'O' and not matrix[x][y] == '\n':
        exit("Error: invalid character")
    # count alive cells around the current position
    for row in range(-1, 2):
        for col in range(-1, 2):
            # when it reaches to the current position 
            if row == 0 and col == 0:
                continue
            # counts alive cells for neighbors
            elif matrix[(x + row) % rownum][(y + col) % colnum] == 'O':
                cnt_alive += 1
    # check if the position needs to be changed 
    if matrix[x][y] == 'O' and (cnt_alive == 2 or cnt_alive == 3 or cnt_alive == 4):
        return 'O'
    if matrix[x][y] == '.' and (cnt_alive % 2 == 0 and cnt_alive > 0):
        return 'O' 
    else:
        return '.'  

if __name__ == '__main__':
    main()
