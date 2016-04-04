# Mon Apr  4 19:14:33 JST 2016
# kk@ise.chuo-u.ac.jp
#
TARGETS=ex1ad ex2ad checker
LIBS=-lglpk
CXXFLAGS=-std=c++11

all: $(TARGETS)
	make example1
	make example2
	echo end

example1:
	./ex1ad

example2:
	./ex2ad
	time ./checker < ex2-output.dat | gzip > ex2-check.out.gz
	zcat ex2-check.out.gz | fgrep print | wc
	zcat ex2-check.out.gz | fgrep nofeasible | wc


% : %.cc
	$(CXX) $(CXXFLAGS) $< -o $@ $(LIBS)

clean:
	/bin/rm -f $(TARGETS)
	/bin/rm -f ex2-check.out.gz
	/bin/rm -f ex2-output.dat
	/bin/rm -f ex2-output.m
