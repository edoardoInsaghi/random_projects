#include "../src/integrMod.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace py::literals;

// Creating Python bindings for the 'integrMod' C++ class within the 'libintegrModPy' module.
PYBIND11_MODULE(libintegrModPy, m) {
    // Documentation for the module.
    m.doc() = "Module for numerical integration";

    // Binding the 'integrMod' class to Python. Although it's possible to only bind methods, 
    // binding the class is necessary here because additional methods will be added to the class in the Python main.
    py::class_<integrMod>(m, "integrMod")

        // Binding methods of the class, we used py::overload_cast<> to disambiguate the overloaded methods.
        .def_static("Midpoint", 
                    py::overload_cast<const std::function<double(double)>&, const double&, const double&, const int&>(&integrMod::Midpoint),
                    "func"_a, "a"_a, "b"_a, "n"_a)
                    
        .def_static("Midpoint", 
                    py::overload_cast<const std::function<double(double)>&, const double&, const double&, const double&, const double>(&integrMod::Midpoint),
                    "func"_a, "a"_a, "b"_a, "error"_a, "step"_a = 1e-3)

        .def_static("Trapezoid", 
                    py::overload_cast<const std::function<double(double)>&, const double&, const double&, const int&>(&integrMod::Trapezoid),
                    "func"_a, "a"_a, "b"_a, "n"_a)

        .def_static("Trapezoid", 
                    py::overload_cast<const std::function<double(double)>&, const double&, const double&, const double&, const double>(&integrMod::Trapezoid),
                    "func"_a, "a"_a, "b"_a, "error"_a, "step"_a = 1e-3)
        .def_static("Simpson", 
                    py::overload_cast<const std::function<double(double)>&, const double&, const double&, int>(&integrMod::Simpson),
                    "func"_a, "a"_a, "b"_a, "n"_a)

        .def_static("Simpson", 
                    py::overload_cast<const std::function<double(double)>&, const double&, const double&, const double&, const double>(&integrMod::Simpson),
                    "func"_a, "a"_a, "b"_a, "error"_a, "step"_a = 1e-3)

        .def_static("GaussQuadrature", 
                    &integrMod::GaussQuadrature,
                    "func"_a, "a"_a, "b"_a, "n"_a)

        .def_static("Adaptive", 
                    &integrMod::Adaptive,
                    "func"_a, "a"_a, "b"_a, "abserr"_a = 1e-7, "relerr"_a = 1e-7)

        .def_static("Integrate", 
                    py::overload_cast<const std::string, const std::function<double(double)>&, const double&, const double&, int>(&integrMod::Integrate),
                    "rule"_a, "func"_a, "a"_a, "b"_a, "n"_a)

        .def_static("Integrate", 
                    py::overload_cast<const std::string, const std::function<double(double)>&, const double&, const double&, const double&, const double>(&integrMod::Integrate),
                    "rule"_a, "func"_a, "a"_a, "b"_a, "error"_a, "step"_a = 1e-3);
}
