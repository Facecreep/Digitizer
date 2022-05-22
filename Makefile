## -*- Makefile -*-
##
## User: mikhailr
## Time: Aug 17, 2018 11:42:36 AM
## Makefile created by Oracle Developer Studio.
##
## This file is generated automatically.
##


OUTPUT=analysis_time
ROOTCONFIG=root-config --cflags --glibs

SOURCES=main.cpp BmnTOF1Conteiner.cxx Dict_DRS.cxx
DICTIONARY=Dict_DRS.cxx
HEDERSFORDICT=BmnTOF1Conteiner.h 

#PATHTOROOT=~/soft/bmnroot/build/config.sh

CFLAGS=-c -Wall
#CFLAGS +=$(shell $(ROOTCONFIG))


all: $(OUTPUT) $(DICTIONARY)

$(DICTIONARY): $(HEDERSFORDICT)
	rootcling -f $(DICTIONARY) -c $(HEDERSFORDICT)
	
$(OUTPUT): $(SOURCES)	
	g++ -o $(OUTPUT) $(SOURCES) `root-config --cflags --glibs `

clean:
	rm -rf $(OUTPUT) Dict_DRS*

# 	g++ -c calibV.cpp `root-config --cflags --glibs`
# g++ -o calibV calibV.o event_reader.o event_calibrator.o `root-config --libs`
