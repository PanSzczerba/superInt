CCPP=g++
MAIN=./src/main
TEST=./src/tests
CPPFLAGS= -I$(MAIN) -std=c++11

%.o: %.cpp
	$(CCPP) -c -o $@ $< $(CPPFLAGS)

all: $(MAIN)/superInt.a \
     $(TEST)/test

$(MAIN)/superInt.a: $(MAIN)/superInt.o
	ar rvs $@ $<
	
$(TEST)/test: $(TEST)/superInt_test.cpp $(MAIN)/superInt.a
	$(CCPP) -o $@ $^ $(CPPFLAGS)
