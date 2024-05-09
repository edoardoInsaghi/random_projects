#include"../src/statsMod.hpp"
#include"../src/integrMod.hpp"
#include <cxxopts.hpp>
#include <chrono>
#include <set>

// Function to calculate and show accuracy, efficency and order of convergence for the methods in our class.
// This function, which is the one for the methods in the class that calculate the integral with a chosen
// number of subintervals, takes in input a method with that specific signature, a function (the one we want to integrate),
// the left and right limit of the integration and the exact value of that integral.
// It calculates, for different numbers of intervals, the value of the integral, time taken by the method, relative error
// and the order of convergence.  
void printInfoN(std::function<double(const std::function<double(double)>&, const double, const double, const int)> method,
               const std::function<double(double)>& func, const double& a,  const double& b,  const double &real_integral){

    // Initialization of the vectors where we will store the absolute error, step size and the order of convergence 
    // for each iteration. 
    std::vector<double> errors, stepSize, result;

    // Fixed number of intervals where we want to see how the method performs.
    std::vector<int> intervals = {10, 100, 1000};

    std::cout << "\n--------------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(25) << "Intervals"
              << std::setw(25) << "Time taken (ms)"
              << std::setw(25) << "Result"
              << std::setw(25) << "Relative error"
              << "\n--------------------------------------------------------------------------------------------\n";

    double approxIntegral;

    // For to iterate on intervals vector.
    for(const auto &element : intervals){
        // Here we initialize a set where we will save the time to invoke a method.
        // We invoke it 10 times and then take the median.
        std::set<double> t;
        for(int i=0; i<10; i++){

            auto start = std::chrono::high_resolution_clock::now(); // Starting the timer.
            approxIntegral = method(func, a, b, element); // Invoking the method.
            auto stop = std::chrono::high_resolution_clock::now(); // Stopping the timer.
            std::chrono::duration<double, std::milli> duration = stop - start;
            t.insert(duration.count());
        }
        // Calculation for the median of the t set.
        auto n = t.size();
        auto mid = std::next(t.begin(), n / 2);
        auto time = (n%2 == 0) ? ((*mid + *std::prev(mid))/2) : *mid; 
        
        double absError = (real_integral - approxIntegral);

        std::cout << std::left << std::setw(25) << element
                  << std::setw(25) << time
                  << std::setw(25) << approxIntegral
                  << std::setw(25) << std::abs(absError/real_integral)
                  << std::endl;

        // Step size for the integration at the i-th iteration of the first loop.
        double h = static_cast<double>((b - a))/element;
        errors.push_back(absError);
        stepSize.push_back(h);
    }

    std::cout << "--------------------------------------------------------------------------------------------\n";

    // Loop to calculate the order of convergence.
    // We calculate it for each iteration and then take the mean.
    for(uint i=0 ; i < intervals.size()-1; i++){

        // Checking the error and the step size to see if the calculation for the order of convergence is valid.
        // If it is too small we skip the iteration.
        if (std::abs(errors[i]) < 1e-10 || std::abs(errors[i+1]) < 1e-10
            || std::abs(stepSize[i]) < 1e-10 || std::abs(stepSize[i+1]) < 1e-10) {
            continue;  
        }
        // We calculate the ratio between the errors and between the steps size.
        double error_change = errors[i+1]/errors[i];
        double step_change = stepSize[i+1]/stepSize[i];

        // Check to avoid processing values that are infinite or NaN.
        // If we found one we skip to the next iteration.
        if (!std::isfinite(error_change) || !std::isfinite(step_change)) {
            continue;  
        }

        // Calculation for the order of convergence for each ietarion.
        result.push_back(std::log(error_change)/std::log(step_change));

    }
    // If the result is not empty it means that we calculate the order of convergence for some iteration
    // and so we take the mean. If it is empty it means that the errors or step size were to small to do the calculation
    // so we print an error message.
    if (!result.empty()) {
    double sum = std::accumulate(result.begin(), result.end(), 0.0);
    std::cout << "The order of convergence is: " << sum / result.size() << std::endl;
    } else std::cout << "The order of convergence is not defined." << std::endl;
    
}

// Very similar function to the one above, but in this case we change the error threshold and not the 
// number of subintervals.
// Since the methods are the same but with a different input and the order of convergence has been calulated
// with the function above, here it is omitted.
// The order of convergence for the adaptive method is not calculated beacuse we need the step size or
// the number of intervals but the GNU Scientific Library, for that function, gives only the possibility to pass
// the absolute and relative error.
void printInfoErr(std::function<double(const std::function<double(double)>&, const double, const double, const double)> method,
               const std::function<double(double)>& func, const double& a,  const double& b,  const double &real_integral){

    std::vector<double> threshold = {1e-2, 1e-3, 1e-4};

    std::cout << "\n--------------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(25) << "Threshold"
              << std::setw(25) << "Time taken (ms)"
              << std::setw(25) << "Result"
              << std::setw(25) << "Relative error"
              << "\n--------------------------------------------------------------------------------------------\n";

    double approxIntegral;

    for(const auto &element : threshold){
        std::set<double> t;
        for(int i=0; i<10; i++){
            auto start = std::chrono::high_resolution_clock::now();
            approxIntegral = method(func, a, b, element);
            auto stop = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = stop - start;
            t.insert(duration.count());
        }

        auto n = t.size();
        auto mid = std::next(t.begin(), n / 2);
        auto time = (n%2 == 0) ? ((*mid + *std::prev(mid))/2) : *mid; 
        
        double absError = (real_integral - approxIntegral);
        std::cout << std::left << std::setw(25) << element
                  << std::setw(25) << time
                  << std::setw(25) << approxIntegral
                  << std::setw(25) << std::abs(absError/real_integral)
                  << std::endl;

    }

    std::cout << "--------------------------------------------------------------------------------------------\n";
    
}

int main(int argc, char *argv[]){

    ///////////////////////////////
    // Part 1: Statistics Module //
    ///////////////////////////////

    std::cout << "\n";
    std::cout << "///////////////////////////////" << std::endl;
    std::cout << "// Part 1: Statistics Module //" << std::endl;
    std::cout << "///////////////////////////////" << std::endl;

    // Allowing the user to give a custom input file path.
    // In order to pass a file path the flag -f must precede the user input.
    cxxopts::Options options("Homework2_Alessio_Barrasso_Insaghi", "");
    options.add_options()("i,input", "Input file path", cxxopts::value<std::string>())
                         ("o,output", "Output file path", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    // Default file to be analysed if none is provided by the user.
    std::string inputFile = "../data/shopping_trends_updated.csv";
    if (result.count("input")) {
        inputFile = result["input"].as<std::string>();
    }

    // Default file for output if none is provided by the user.
    std::string outFile = "../data/output.csv";
    if (result.count("output")) {
        outFile = result["output"].as<std::string>();
    }

    // Building the object .
    statsMod df{inputFile};

    // Removing rows with at least one NA value (if any are present).
    df.dropNA();

    // Obtaining the number of columns of the data-set.
    std::cout << "\n";
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;
    std::cout << "Number of columns stored in the object: "<< df.ncol << "\n" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    // Printing the name of the columns.
    std::cout << "Name of the variables: " << std::endl;
    df.colnames();
    std::cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    // The data-set contains 18 columns, which makes it hard to visualize it neatly.
    // In this case, we select a subset of the columns to showcase the functioning
    // of the statistics module. The object lends itself to be sliced both on the columns
    // and the rows (more on that later) by simply calling the operator() with a vector
    // of the columns the user wants to keep.
    std::vector<std::string> sub_vars = {"Age", "Purchase Amount (USD)", "Review Rating", "Previous Purchases", "Payment Method", "Item Purchased"};
    statsMod sub_df = df(sub_vars);

    // Getting a brief description of the data_set"
    std::cout << "Brief summary of the data." << std::endl;
    sub_df.summary();
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    // Each of the previous function can be called specifying any numerical column stored in the 
    // data-set, and two indexes. The function will use only the rows in between said indexes as 
    // inputs. Here we call the mean of Purchase Amount (USD) and Review Rating, for the observation
    // between row 2000 and 2100
    std::vector<std::string> vars_reduced = {"Purchase Amount (USD)", "Review Rating"};
    std::map<std::string, double> means_reduced = df.mean(vars_reduced, 2000, 2100);
    std::cout << "Means of the variables Purchase Amount (USD) and Review Rating for the observation between rows 2000 and 2100: " << std::endl;    
    for(const auto &pair : means_reduced) 
        std::cout << pair.first << ": " << pair.second << std::endl;
    std::cout << std::endl;    
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    // Obtaining the correlation between pairs of numerical variables.
    std::cout << "Correlations: " << std::endl;
    std::vector<std::string> cor_vars = {"Previous Purchases", "Age"};
    std::vector<std::string> cor_vars2 = {"Review Rating", "Purchase Amount (USD)"};
    std::cout << "Corr(Previous Purchases, Age) = " << sub_df.corr(cor_vars) << std::endl;
    std::cout << "Corr(Review Rating, Purchase Amount (USD)) = " << sub_df.corr(cor_vars2) << "\n" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    // Counting the occurrences of every possible value for a categorical variable, the same as in the summary.
    std::map<std::string, uint> payment_counts = sub_df.freq("Payment Method");
    std::cout << "Payment methods: " << std::endl;
    for(const auto &pair : payment_counts) 
        std::cout << pair.first << ": " << pair.second << std::endl;
    std::cout << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    // Obtaining the subset of the rows that contain a specific value in a given column.
    // Obtaining only the observations for the people who paid Cash.
    statsMod cash_df = sub_df.find("Payment Method", "Cash");
    std::cout << "Subset of the people who paid cash. " << std::endl;
    cash_df.head();

    // Overloading the operator() also allows to slice the data-set on the rows, calling
    // df(i, j) returns a statsMod object containing the rows between indexes i and j.
    statsMod sub_df_small = sub_df(100, 199);
    std::cout << "\n";
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;
    std::cout << "Number of rows of the small data-set: " << sub_df_small.nrow << "\n" << std::endl;
    std::cout << "-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;
    
    // This way of slicing the data-set, which returns another instance of the same class 
    // containing the subset of interest of the data, allows for complex instruction via concatenation
    // of simpler ones. With the next line of code we obtain the subset of the observations between 
    // line 100 and line 1100 who paid cash and bought a pair of jeans.
    statsMod special_df = sub_df(100, 1100).find("Payment Method", "Cash").find("Item Purchased", "Jeans");
    std::cout << "Subset of the observations between line 100 and 1100, who bought jeans using cash. " << std::endl;
    special_df.head();
    std::cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;


    // Iterating over the rows of the data-set.
    for(auto it=sub_df.begin(); it.pos<10; ++it){
        std::cout << "Row " << it.pos << ": ";
        sub_df.printRow(*it);
    }
    std::cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    special_df.writeToFile(outFile);
    std::cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------\n" << std::endl;

    std::cout << "\n";



    ////////////////////////////////
    // Part 2: Integration Module //
    ////////////////////////////////

    std::setprecision(10);

    std::cout << "\n";
    std::cout << "////////////////////////////////" << std::endl;
    std::cout << "// Part 2: Integration Module //" << std::endl;
    std::cout << "////////////////////////////////" << std::endl;
    std::cout << "\n" << std::endl;


    // Defining e^x + 3x^2+ 10 as a lambda function.
    auto f = [](double x){return exp(x) + (3*x*x) + 10;};

    // Exact value of the integral.
    double realIntegral = 1099 + exp(10);

    //We decided to integrate the f function in the interval [0,10].

    // Calling the functions to test the performance of Midpoint methods of the class.
    std::cout << "///////////////////" << std::endl;
    std::cout << "// Midpoint rule //" << std::endl;
    std::cout << "///////////////////" << std::endl;

    printInfoErr([&](const std::function<double(double)>& func, const double a, const double b, const double error) {
              return integrMod::Midpoint(func, a, b, error);}, f, 0, 10, realIntegral);
    printInfoN([&](const std::function<double(double)>& func, const double a, const double b, const int n) {
              return integrMod::Midpoint(func, a, b, n);}, f, 0, 10, realIntegral);

    // Calling the functions to test the performance of Trapezoid methods of the class.
    std::cout << "\n\n";
    std::cout << "////////////////////" << std::endl;
    std::cout << "// Trapezoid rule //" << std::endl;
    std::cout << "////////////////////" << std::endl;

    printInfoErr([&](const std::function<double(double)>& func, const double a, const double b, const double error) {
              return integrMod::Trapezoid(func, a, b, error);}, f, 0, 10, realIntegral);
    
    printInfoN([&](const std::function<double(double)>& func, const double a, const double b, const int n) {
              return integrMod::Trapezoid(func, a, b, n);}, f, 0, 10, realIntegral);

     // Calling the functions to test the performance of Simpson methods of the class.
    std::cout << "\n\n";
    std::cout << "////////////////////" << std::endl;
    std::cout << "// Simpson's rule //" << std::endl;
    std::cout << "////////////////////" << std::endl;

    printInfoErr([&](const std::function<double(double)>& func, const double a, const double b, const double error) {
              return integrMod::Simpson(func, a, b, error);}, f, 0, 10, realIntegral);

    printInfoN([&](const std::function<double(double)>& func, const double a, const double b, const int n) {
              return integrMod::Simpson(func, a, b, n);}, f, 0, 10, realIntegral);
    
    // Calling the functions to test the performance of GaussQuadrature methods of the class.
    std::cout << "\n\n";
    std::cout << "///////////////////" << std::endl;
    std::cout << "// Gaussian rule //" << std::endl;
    std::cout << "///////////////////" << std::endl;

    printInfoN([&](const std::function<double(double)>& func, const double a, const double b, const int n) {
              return integrMod::GaussQuadrature(func, a, b, n);}, f, 0, 10, realIntegral);

     // Calling the functions to test the performance of Adaptive methods of the class.
    std::cout << "\n\n";
    std::cout << "///////////////////" << std::endl;
    std::cout << "// Adaptive rule //" << std::endl;
    std::cout << "///////////////////" << std::endl;

    printInfoErr([&](const std::function<double(double)>& func, const double a, const double b, const double error) {
              return integrMod::Adaptive(func, a, b, error);}, f, 0, 10, realIntegral);
    
    // Just an example of how the Integrate method can be used.
    std::cout<<"\nThe result of the integral with Trapezoid rule using the Integrate method is: "
             <<integrMod::Integrate("Trapezoid", f, 0, 10, 1e-4)<<std::endl;
    std::cout << "\n"; 

    return 0;
}

    