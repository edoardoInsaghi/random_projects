#ifndef INTEGRMOD__
#define INTEGRMOD__

#include<iostream>
#include<string>
#include<gsl/gsl_integration.h>
#include <functional>


// The integrMod class provides a collection of methods for numerical integration. All of them are static
// since within the class we don't store anything and we thought it was a good idea to call the method without instantiating
// an object of the class. Every integration method has in common the fact that it takes in input a function (user defined or lambda),
// the left and the right limit of the integration.

class integrMod{
public:

    // Midpoint, Trapezoid and Simpson rule have two methods each because we wanted to be more flexible
    // and give the user the possibility to pass either the number of subintervals that he wanted to use
    // for the integration or the error tolerance. In the second case the number of subintervals is calculated
    // internally in order to not exceed the threshold.
    static double Midpoint(const std::function<double(double)>& func, const double& a,  const double& b, const int& n);
    static double Midpoint(const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step = 1e-3);

    static double Trapezoid(const std::function<double(double)>& func, const double& a,  const double& b, const int& n);
    static double Trapezoid(const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step = 1e-3);
    
    static double Simpson(const std::function<double(double)>& func, const double& a,  const double& b, int n);
    static double Simpson(const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step = 1e-3);
    

    // GaussQuadrature rule has been implemented using the GNU Scientific Library, takes in input,
    // besides the other common input, the number of points used for the integration.
    // It has only one method because the function from the Scientific Library used inside 
    // does not accept absolute or relative error bounds.
    static double GaussQuadrature(const std::function<double(double)>& func, const double& a,  const double& b, const int &n);


    //An adaptive integration technique also implemented with the help of the GNU Scientific Library.
    static double Adaptive(const std::function<double(double)>& func, const double& a,  const double& b, const double abserr = 1e-7, const double relerr = 1e-7);


    //Method to give the possibility to the user to just invoke one method and then specify the rule with a string.
    static double Integrate(const std::string rule, const std::function<double(double)>& func, const double& a,  const double& b, int n);
    static double Integrate(const std::string rule, const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step = 1e-3);


private:

    // Utilities functions
    static double calculateMax(const std::function<double(double)>& derivative, double a, double b, double step);
    static int calculateN(const double &C, const double root, const double &error);
    static std::string Lower(const std::string& str);
};

#endif