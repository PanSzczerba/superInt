CCPP=g++
CPPFLAGS= -I./main -std=c++11
MAIN=./main
TEST=./tests

%.o: %.cpp
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

all: $(MAIN)/superInt.a \
     $(TEST)/test

$(MAIN)/superInt.a: $(MAIN)/superInt.o
	ar rvs $@ $<
	
$(TEST)/test: $(TEST)/superInt_test.cpp $(MAIN)/superInt.a
	$(CCPP) -o $@ $^ $(CPPFLAGS)
