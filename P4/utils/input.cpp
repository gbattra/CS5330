// Greg Attra
// 03/09/2021

/**
 * Implementation of the input namespace defined in utils.h
 */

#include "utils/utils.h"

/**
 * Helper function for getting user input from the console.
 * 
 * @param prompt the string to display as a prompt for the user
 * 
 * @return T the expected return type
 */
template <typename T>
T getInput(std::string prompt)
{
    T x;
    std::cout << prompt;
    std::cin >> x;
    std::cout << std::endl;
    return x;
}