all: Mosaics

Mosaics: mosaics.cpp
	g++ -o mosaics mosaics.cpp `pkg-config opencv --cflags --libs`
