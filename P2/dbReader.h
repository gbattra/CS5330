// Greg Attra
// 02/09/2021

/**
 * Header file for functions to read directory information.
 */

#include <vector>
#include <string>

namespace db
{
    /**
     * Reads the file names at the specified path into a vector of strings.
     * 
     * @param db_path a string pointer to the pathname of the directory
     * 
     * @return the list of filenames in the directory
     */
    std::vector<std::string> list(std::string *db_path);
}