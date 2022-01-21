#I would strongly recommend NOT changing any lines below except the CC and MYFILE lines.
#Before running this file, run the command:   module load gnu


EXECS=danc_parser

#Replace the g++ with gcc if using C
CC=g++

#Replace with the name of your C or C++ source code file.
MYFILE=Airi_Shimamura_R11652198_Assignment4.cpp


all: ${EXECS}

${EXECS}: ${MYFILE}
	${CC} -o ${EXECS} ${MYFILE}

clean:
	rm -f ${EXECS}