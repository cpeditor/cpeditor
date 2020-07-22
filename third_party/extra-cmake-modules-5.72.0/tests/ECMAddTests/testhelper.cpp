#include "testhelper.h"
#include <fstream>
#include <string>

void make_test_file(const char *filename)
{
    std::ofstream(filename) << "test" << std::endl;
}
