#include "../src/statsMod.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>


namespace py = pybind11;
typedef std::map<std::string, std::vector<std::optional<std::variant<double, std::string>>>> MAP;

PYBIND11_MODULE(libstatsModPy, m){
    py::class_<statsMod>(m, "statsMod")

        // Constructors
        .def(py::init<const std::string&>())
        .def(py::init<MAP&>())

        // Operator()
        .def("__call__", py::overload_cast<int, int>(&statsMod::operator()))
        .def("__call__", py::overload_cast<std::string>(&statsMod::operator()))
        .def("__call__", py::overload_cast<std::vector<std::string>>(&statsMod::operator()))

        // Methods
        .def("find", &statsMod::find)
        .def("mean", &statsMod::mean)
        .def("var", &statsMod::var)
        .def("sd", &statsMod::sd)     
        .def("median", &statsMod::median)      
        .def("corr", &statsMod::corr, py::arg("vars"), py::arg("start") = 0, py::arg("end") = 0)    
        .def("freq", &statsMod::freq)     
        .def("colnames", &statsMod::colnames)       
        .def("isVar", &statsMod::isVar)      
        .def("dropNA", &statsMod::dropNA)    
        .def("summary", &statsMod::summary)    
        .def("head", &statsMod::head, py::arg("arg0") = 5)
        .def("printRow", &statsMod::printRow)   
        .def("writeToFile", &statsMod::writeToFile)

        .def("__repr__", [](const statsMod& self) { 
            std::ostringstream oss;
            oss << "A statsMod object with " << self.nrow << "rows and " << self.ncol << "columns.";
            return oss.str();})

        .def("__iter__", [](statsMod& self) { return py::make_iterator(self.begin(), self.end()); }, 
            py::keep_alive<0, 1>())

        // Attributes
        .def_readwrite("data", &statsMod::data)
        .def_readwrite("columns", &statsMod::columns)
        .def_readwrite("numCols", &statsMod::numCols)
        .def_readwrite("catCols", &statsMod::catCols)
        .def_readwrite("ncol", &statsMod::ncol)
        .def_readwrite("nrow", &statsMod::nrow);

    // Iterator Class
    py::class_<statsMod::df_iterator>(m, "df_iterator")
        .def("__iter__", [](statsMod::df_iterator &it) { return &it; })
        .def("__next__", [](statsMod::df_iterator &it) {
            if (it != it.df.end()) {
                auto value = *it;
                ++it;
                return value;
            }
            throw py::stop_iteration();
    });
}