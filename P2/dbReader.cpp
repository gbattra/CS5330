// Greg Attra
// 02/09/2021

#include "dbReader.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>

// code taken from sample provided by prof:
// https://northeastern.instructure.com/courses/76659/files/7911128?wrap=1

namespace db
{
    /**
     * Reads the file names at the specified path into a vector of strings.
     * 
     * @param db_path a string pointer to the pathname of the directory
     * @return the list of filenames in the directory
     */
    std::vector<std::string> list(std::string *db_path)
    {
        FILE *f;
        DIR *dir;
        struct dirent *dirent;
        int i;

        dir = opendir(db_path->c_str());
        if (dir == NULL)
        {
            printf("Invalid db directory.\n");
            return std::vector<std::string>(0);
        }

        std::vector<std::string> image_files;
        while ((dirent = readdir(dir)) != NULL)
        {
            if (
                strstr(dirent->d_name, ".jpg")
                || strstr(dirent->d_name, ".png")
                || strstr(dirent->d_name, ".ppm")
                || strstr(dirent->d_name, ".tif"))
            {
                char buffer[256];
                strcpy(buffer, db_path->c_str());
                if (db_path->back() != '/')
                {
                    strcat(buffer, "/");
                }
                strcat(buffer, dirent->d_name);
                image_files.push_back(std::string(buffer));
            }
        }

        return image_files;
    }
}