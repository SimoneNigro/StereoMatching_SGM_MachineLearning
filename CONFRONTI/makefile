prevedi:  main.o confronta.o
	g++ -o prevedi *.o -g `pkg-config opencv --cflags --libs` -fopenmp -fpermissive

main.o: prevedi_mappa.cpp
	g++ -c prevedi_mappa.cpp `pkg-config opencv --cflags --libs` -g -fopenmp -fpermissive

confronta.o: confronta.cpp
	g++ -c confronta.cpp `pkg-config opencv --cflags --libs` -g -fopenmp

