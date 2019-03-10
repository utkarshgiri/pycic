#include "armadillo"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<double> cic(py::array_t<double> position, py::array_t<double> weight, int boxsize, int gridsize)
{

    arma::mat pos;
    arma::vec wei;
    arma::cube result(gridsize, gridsize, gridsize);
    
    py::buffer_info positioninfo = position.request();
    py::buffer_info weightinfo = weight.request();

    pos = arma::mat(reinterpret_cast<double*>(positioninfo.ptr),positioninfo.shape[0], positioninfo.shape[1]);
    wei = arma::vec(reinterpret_cast<double*>(weightinfo.ptr),weightinfo.shape[0]);

    arma::inplace_trans(pos);
    if(boxsize == 0) boxsize = ceil(pos.max());
    if(gridsize == 0) gridsize = boxsize;

    double w;
    int xindex, yindex, zindex;
    double xcoord, ycoord, zcoord, dx, dy, dz, tx, ty, tz;
    
    int n = gridsize;
    int num_particles = pos.n_cols;
    double spacing = double(boxsize) / double(gridsize);


    for(int i=0; i<num_particles; i++)
    {
        w = wei(i);
	
	xcoord = pos(0,i) / spacing; xindex = int(xcoord);
        ycoord = pos(1,i) / spacing; yindex = int(ycoord);
        zcoord = pos(2,i) / spacing; zindex = int(zcoord);

	dx = xcoord - xindex; tx = 1 - dx;
        dy = ycoord - yindex; ty = 1 - dy;
        dz = zcoord - zindex; tz = 1 - dz;

        result(xindex%n, yindex%n, zindex%n) += tx*ty*tz*w;
        result(xindex%n, (yindex+1)%n, zindex%n) += tx*dy*tz*w;
        result(xindex%n, yindex%n, (zindex+1)%n) += tx*ty*dz*w;
        result(xindex%n, (yindex+1)%n, (zindex+1)%n) += tx*dy*dz*w;
        result((xindex+1)%n, yindex%n, (zindex+1)%n) += dx*ty*dz*w;
        result((xindex+1)%n, (yindex+1)%n, (zindex+1)%n) += dx*dy*dz*w;
        result((xindex+1)%n, (yindex+1)%n, zindex%n) += dx*dy*tz*w;
        result((xindex+1)%n, yindex%n, zindex%n) += dx*ty*tz*w;


    }


    py::buffer_info buffer( result.memptr(), 
                            sizeof(double),
                            py::format_descriptor<double>::format(),
                            3,
                            py::detail::any_container<ssize_t> ({result.n_rows, result.n_cols, result.n_slices}),
                            py::detail::any_container<ssize_t> ({ sizeof(double) , result.n_cols * sizeof(double), sizeof(double) * result.n_rows * result.n_cols }));

    return py::array_t<double>(buffer);
}

PYBIND11_MODULE(pycic, m)
{
    m.doc() = " ";
    m.def("cic", &cic);

}
