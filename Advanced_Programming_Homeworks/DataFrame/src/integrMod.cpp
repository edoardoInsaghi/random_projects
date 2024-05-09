#include"integrMod.hpp"

// Function to calculate the max of a function between a and b bounds passed in input.
// In this module it is used to calculate the max of different derivates 
// which is needed to compute the error in Midpoint, Trapezoid and Simpson methods.
// The step variable has a default value but it can be modified if the user wants to.

double integrMod::calculateMax(const std::function<double(double)>& derivative, double a, double b, double step) {

    double max = 0.0;

    for (double x = a; x <= b; x += step) {
        double curr = std::abs(derivative(x));
        max = (curr > max) ? curr : max;
    }
    return max;
}


// Funtion to calculate the number of intervals needed to avoid exceeding the threshold.
// For the Midpoint, Trapezoid and Simpson rule the absolute error |E| is bounded by C / n^x
// C is a constant different for every technique , n is the number of subintervals and 
// x is equal to 2 for the Midpoint and Trapezoid rule and 4 for the Simpson one.
// This function takes in input the constant C, the power of n which is root and the 
// threshold passed by the user.

int integrMod::calculateN(const double &C, const double root, const double &error){

    // If we want the numbers of subintervals required not to exceed the threshold 
    // we have to invert this inequality C / n^2 <= error.
    double dn = std::pow(C/error, root);   
    int n = static_cast<int>(std::ceil(dn)); // rounding the result to the next integer
    return n;
}


// Function to convert a string into a lowercase one.
std::string integrMod::Lower(const std::string& str){

        std::string lowerStr = str;
        // Since std::tolower function takes in input only a char we use std::transform
        // to apply the operation on each character of the string.
        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return lowerStr;
}


// Method for Midpoint integration.
double integrMod::Midpoint(const std::function<double(double)>& func, const double& a,  const double& b, const int& n){

    if (n <= 0) throw std::runtime_error("Number of intervals must be positive.");

    double delta = (b - a)/n; //size of the intervals
    double mid, sum= 0;
    
    for(int i = 1; i <= n; i ++){
        mid = a + (i - 0.5) * delta; //calculate midpoint for each subintervals
        sum += func(mid);
    }
    return sum * delta;
}


// Method to calculate the integral with Midpoint rule passing an error threshold.
// It also takes the step variable in input, it has a default value and is needed to 
// call calculateMax function defined above.
double integrMod::Midpoint(const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step) {

    if (error <= 0 || step <= 0) throw std::runtime_error("Error tolerance and step size must be positive.");

    // Lambda function to calculate the second derivative.
    auto second_derivative = [func](double x) {
        double h = 1e-5; //optimal size for  for double-precision calculations.
        return (func(x + h) - 2 * func(x) + func(x - h)) / (h * h);
    };

    double maxDerivative = calculateMax(second_derivative, a, b, step);
    double C = (std::pow((b - a), 3) * maxDerivative / 24.); //calculation for the C constant in Midpoint rule.
    int Numintervals = calculateN(C, 0.5, error);

    return Midpoint(func, a, b, Numintervals);
}


// Method for Trapezoid integration.
double integrMod::Trapezoid(const std::function<double(double)>& func, const double& a,  const double& b, const int& n){

    if (n <= 0) throw std::runtime_error("Number of intervals must be positive.");

    double delta = (b - a)/n;
    double sum = 0.5 * (func(a) + func(b));

    for (int i = 1; i < n; ++i) {
        sum += func(a + i * delta);
    }

    return sum * delta;
}


// Method to calculate the integral with Trapezoid rule passing an error threshold.
// It also takes the step variable in input, it has a default value and is needed to 
// call calculateMax function defined above
double integrMod::Trapezoid(const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step){

    if (error <= 0 || step <= 0) throw std::runtime_error("Error tolerance and step size must be positive.");

     // Lambda function to calculate the second derivative.
    auto second_derivative = [func](double x) {
        const double h = 1e-5; //optimal size for  for double-precision calculations.
        return (func(x + h) - 2 * func(x) + func(x - h)) / (h * h);
    };

    double maxDerivative = calculateMax(second_derivative, a, b, step);
    double C = (std::pow((b - a), 3)* maxDerivative) / 12.0; //calculation for the C constant in Trapezoid rule.
    int Numintervals = calculateN(C, 0.5, error);

    return Trapezoid(func, a, b, Numintervals);
}


// Method for Simpson integration.
// In this method the input n is not const as in the other methods because the Simpson rule requires an even number
// of subintervals and if the user passes an odd one the variable is increased by one in order not to lose 
// precision in the integral calculation.
double integrMod::Simpson(const std::function<double(double)>& func, const double& a,  const double& b, int n){

    if (n <= 0) throw std::runtime_error("Number of intervals must be positive.");

    n = (n%2 != 0) ? ++n : n; // n increased by one if it's odd.

    double delta = (b - a)/n;
    double sum = (func(a) + func(b));
    
    for(int i = 1; i<n; i++){
        double val = (i%2 == 0) ? 2 : 4;
        sum += (val*func(a + i * delta));
    }
    return sum * delta/3.0;
}


// Method to calculate the integral with Simpson rule passing an error threshold.
// It also takes the step variable in input, it has a default value and is needed to 
// invoke the calculateMax function defined above.
double integrMod::Simpson(const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step){

    if (error <= 0 || step <= 0) throw std::runtime_error("Error tolerance and step size must be positive.");

    // Lambda function to calculate the fourth derivative.
    auto fourth_derivative = [func](double x) {
        const double h = 1e-5; //optimal size for for FLOP.
        return (func(x - 2 * h) - 4 * func(x - h) + 6 * func(x) - 4 * func(x + h) + func(x + 2 * h)) / std::pow(h, 4);
    };

    double maxDerivative = calculateMax(fourth_derivative, a, b, step);
    double C =(std::pow((b - a), 5) * maxDerivative) / 2880.0; //calculation for the C constant in Simpson rule.
    int Numintervals = calculateN(C, 0.25, error);
    
    return Simpson(func, a, b, Numintervals);

}


// Since the GNU Scientific Library expects a function pointer in C style this function is used to
// convert a C++ std::function into it.
// It takes in input a double which is the point at which the function will be evaluated and a void*
// which is a pointer to the actual function used inside this function. 
double Wrapper(double x, void* params) {

    // The void* pointer params is cast to a pointer of type std::function<double(double)>
    auto func = static_cast<std::function<double(double)>*>(params);
    // Here the func pointer is dereferenced to get the std::function,
    // and then it's invoked with x as the argument.
    return (*func)(x);
}


// Method for GaussQuadrature integration, using the GNU Scientific Library
double integrMod::GaussQuadrature(const std::function<double(double)>& func, const double& a,  const double& b, const int &n){

    if (n <= 0) throw std::runtime_error("Number of nodes must be positive.");

    gsl_function gslFunc; //declares a GSL function structure.
    // Sets the function pointer to Wrapper function 
    // that adapts the C++ std::function to the format expected by GSL.
    gslFunc.function = &Wrapper; 
    gslFunc.params = const_cast<std::function<double(double)>*>(&func);//passes the original func as a parameter to the Wrapper function.

    double result = 0.0;
    double xi, wi; // nodes and the weights for the formula.

    gsl_integration_glfixed_table *table = gsl_integration_glfixed_table_alloc(n);//This table contains the weights and nodes for the quadrature.
    if (!table) {
        throw std::runtime_error("Failed to allocate Gauss-Legendre table.");
    }

    for (size_t i = 0; i < n; ++i) {
        // With this function we obtain the i-th node and weight.
        gsl_integration_glfixed_point(a, b, i, &xi, &wi, table);
        // Adding to result the function calculated in xi and weighted with wi.
        result += wi * gslFunc.function(xi, gslFunc.params);
    }

    gsl_integration_glfixed_table_free(table); // This function frees the memory associated with table.

    return result;
}


// Method for adaptive integration with GNU Scientif Library.
// The functions used inside implement a simple adaptive integration procedure which means that 
// it adjusts the number of intervals dynamically based on the function's behavior.
// There is the possbility to pass the asbsolute and relative error, by default they are set to 1e-7.
double integrMod::Adaptive(const std::function<double(double)>& func, const double& a,  const double& b, const double abserr, const double relerr){
    
    if (abserr <= 0 || relerr <= 0) {
    throw std::runtime_error("Error tolerances must be positive.");
    }

    gsl_function gslFunc; //declares a GSL function structure.
    // Sets the function pointer to Wrapper a function 
    // that adapts the C++ std::function to the format expected by GSL.
    gslFunc.function = &Wrapper;
    gslFunc.params = const_cast<void*>(reinterpret_cast<const void*>(&func)); //passes the original func as a parameter to the Wrapper function.

    double result, error;
    //This table contains the max number of subintervals used for the integration.
    gsl_integration_workspace* workspace = gsl_integration_workspace_alloc(1000);

    // Adaptive integration of the function between the bounds a and b with specified errors tolerence.
    gsl_integration_qags(&gslFunc, a, b, abserr, relerr, 1000, workspace, &result, &error);
    
    gsl_integration_workspace_free(workspace);//this function frees the memory associated with workspace.

    return result;
}



// This is the general method for integration with a fixed amount of subintervals. It takes in input a string
// which has to specify the integration rule, if the string doesn't match any rule it throws an error.
double integrMod::Integrate(const std::string rule, const std::function<double(double)>& func, const double& a,  const double& b, int n){

    // Function to convert a string into a lowercase one.
    std::string lowerstr = Lower(rule);

    if (rule.empty()) {
        throw std::runtime_error("Integration rule is not specified.");
    }

    if (lowerstr == "midpoint") return Midpoint(func, a, b, n);
    else if (lowerstr == "trapezoid") return Trapezoid(func, a, b, n);
    else if (lowerstr == "simpson") return Simpson(func, a, b, n);
    else if (lowerstr == "gaussquadrature") return GaussQuadrature(func, a, b, n);
    else throw std::runtime_error("No matching integration rule.");
}


// This is the general method for integration with an error tolerence. It takes in input a string
// which has to specify the integration rule, if the string doesn't match any rule it throws an error.
double integrMod::Integrate(const std::string rule, const std::function<double(double)>& func, const double& a,  const double& b, const double& error, const double step){

    // Function to convert a string into a lowercase one.
    std::string lowerstr = Lower(rule);

    double result;

    if (rule.empty()) {
        throw std::runtime_error("Integration rule is not specified.");
    }

    if (lowerstr == "midpoint") return Midpoint(func, a, b, error, step);
    else if (lowerstr == "trapezoid") return Trapezoid(func, a, b, error, step);
    else if (lowerstr == "simpson") return Simpson(func, a, b, error, step);
    else if (lowerstr == "adaptive") {
        if(step == 1e-3) return Adaptive(func, a, b, error);
        else throw std::runtime_error("Cannot use adaptive integration setting the step input.");
    }
    else throw std::runtime_error("No matching integration rule");
}