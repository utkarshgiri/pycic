all:
	g++ -O3 -Wall -shared -march=native -ffp-contract=fast -DARMA_NO_DEBUG -std=c++11 -fPIC `python3 -m pybind11 --includes` pycic/pycic.cpp -o pycic`python3-config --extension-suffix`

install:
	cp *.so ${HOME}/anaconda3/lib/python3.7/site-packages
