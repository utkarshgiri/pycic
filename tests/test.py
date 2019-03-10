import pycic
import numpy

size = 128
position = numpy.random.random_sample((size**3,3))*size
weight = numpy.ones(len(position))

grid = pycic.cic(position, weight, size, size)

assert numpy.isclose(numpy.mean(grid), 1.)
