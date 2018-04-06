genera_train: interpolation.o cost.o kitti.o matrix.o main.o DSI.o census.o disparity.o SGM.o
	g++ -o genera_train *.o `pkg-config opencv --cflags --libs` -fopenmp -g

main.o: genera_train.cpp
	g++ -c genera_train.cpp `pkg-config opencv --cflags --libs`  -fopenmp -g

interpolation.o: interpolation.c
	g++ -c interpolation.c `pkg-config opencv --cflags --libs`  

cost.o: cost.c
	g++ -c cost.c `pkg-config opencv --cflags --libs`  

kitti.o: kitti.c
	g++ -c kitti.c `pkg-config opencv --cflags --libs`  

matrix.o: matrix.c
	g++ -c matrix.c `pkg-config opencv --cflags --libs`  

DSI.o: DSI.c
	g++ -c DSI.c `pkg-config opencv --cflags --libs`  -g

SGM.o: SGM.c
	g++ -c SGM.c `pkg-config opencv --cflags --libs`  -fopenmp -g

census.o: census.c
	g++ -c census.c `pkg-config opencv --cflags --libs`  

disparity.o: disparity.c
	g++ -c disparity.c `pkg-config opencv --cflags --libs`  

