prevedi: interpolation.o cost.o filter.o filterUtilities.o kitti.o matrix.o main.o DSI.o census.o disparity.o SGM.o main.o genera_8_mappe.o read_data_csv.o confronta.o
	g++ -o prevedi *.o -g `pkg-config opencv --cflags --libs` -fopenmp -fpermissive

main.o: prevedi_mappa.cpp
	g++ -c prevedi_mappa.cpp `pkg-config opencv --cflags --libs` -g -fopenmp -fpermissive

genera_8_mappe.o: genera_8_mappe.cpp
	g++ -c genera_8_mappe.cpp `pkg-config opencv --cflags --libs` -g -fopenmp

confronta.o: confronta.cpp
	g++ -c confronta.cpp `pkg-config opencv --cflags --libs` -g -fopenmp

read_data_csv.o: read_data_csv.cpp
	g++ -c read_data_csv.cpp `pkg-config opencv --cflags --libs` -g -fopenmp

cost.o: cost.c
	g++ -c cost.c `pkg-config opencv --cflags --libs`  -g

interpolation.o: interpolation.c
	g++ -c interpolation.c `pkg-config opencv --cflags --libs`  

filter.o: filter.c
	g++ -c filter.c `pkg-config opencv --cflags --libs`  

filterUtilities.o: filterUtilities.c
	g++ -c filterUtilities.c `pkg-config opencv --cflags --libs`  

kitti.o: kitti.c
	g++ -c kitti.c `pkg-config opencv --cflags --libs`  

matrix.o: matrix.c
	g++ -c matrix.c `pkg-config opencv --cflags --libs` -g

DSI.o: DSI.c
	g++ -c DSI.c `pkg-config opencv --cflags --libs`  -g

SGM.o: SGM.c
	g++ -c SGM.c `pkg-config opencv --cflags --libs` -g -fopenmp

census.o: census.c
	g++ -c census.c `pkg-config opencv --cflags --libs` -g 

disparity.o: disparity.c
	g++ -c disparity.c `pkg-config opencv --cflags --libs` -g

