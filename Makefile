crawl:crawl.cpp
	g++  crawl.cpp -o crawl.o

run1: crawl.o
	./crawl.o
run2: crawl.o
	./crawl.o ${DIR}
