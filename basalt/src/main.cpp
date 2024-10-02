#include <filesystem>
#include <iostream>
#include "library_test.h"

// Uncomment this line to enable tests
// #define RUN_TESTS

int main() {
	const std::filesystem::path baseResourcePath = std::filesystem::current_path() / ".." / ".." / ".." / "resources";
    
#ifdef RUN_TESTS
        runLibraryTests(baseResourcePath.string());
#endif

    return 0;
}