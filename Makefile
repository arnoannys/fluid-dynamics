#Makefile voor examenopdracht
# Arno Annys 
# arno.annys@student.uantwerpen.be
#

COMPILER = g++ -Wall
LIBDIR = `root-config --glibs --cflags`

#aanroepen met make all
all : main_all main_test systeem plot state


main_all: main_all.cpp systeem state plot 
	$(COMPILER)  -o $@ main_all.cpp systeem.o state.o plot.o $(LIBDIR) 
main_test: main_test.cpp systeem state plot 
	$(COMPILER)  -o $@ main_test.cpp systeem.o state.o plot.o $(LIBDIR) 
state: state.cpp
	$(COMPILER)  -c state.cpp $(LIBDIR)  
systeem: systeem.cpp
	$(COMPILER)  -c systeem.cpp $(LIBDIR)  
plot: plot.cpp
	$(COMPILER)  -c plot.cpp $(LIBDIR)

#aanroepen met make clean
clean:
	rm -f main_all     
	rm -f main_test
	rm -f systeem
	rm -f plot
	rm -f state