all:
	gcc -c zlib/*.c
	g++ *.cpp *.o `wx-config --cxxflags --libs` -o factorioBlueprintReader

clean:
	rm *.o
