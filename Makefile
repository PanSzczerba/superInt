CXX=g++
MAIN=./src/main
TEST=./src/tests
INCLUDE= -I$(MAIN)
CPPFLAGS= -Wall -std=c++11
OPT=

%.o: %.cpp
	$(CXX) -c -o $@ $(INCLUDE) $< $(CPPFLAGS) $(OPT)

all: $(MAIN)/superInt.a \
     $(TEST)/test \
	 $(TEST)/factorial

$(MAIN)/superInt.a: $(MAIN)/superInt.o
	ar rvs $@ $<
	
$(TEST)/test: $(TEST)/superInt_test.cpp $(MAIN)/superInt.a
	$(CXX) -o $@ $(INCLUDE) $^ $(CPPFLAGS) $(OPT)

$(TEST)/factorial: $(TEST)/factorial.cpp $(MAIN)/superInt.a
	$(CXX) -o $@ $(INCLUDE) $^ $(CPPFLAGS) $(OPT)

clean: 
	rm -f $(MAIN)/*.o
	rm -f $(TEST)/*.o
	rm -f $(MAIN)/superInt.a
	rm -f $(TEST)/test
