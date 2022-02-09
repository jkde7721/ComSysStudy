#include "CompilationEngine.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
	string path = argv[1];
	if (path.substr(path.length() - 5).compare(".jack") == 0) {//file
		CompilationEngine engine(path);
	}
	else {//directory
		for (const auto& entry : fs::directory_iterator(path)) {
			if (entry.path().extension() == ".jack") {
				CompilationEngine engine(entry.path());
			}
		}
	}
}//file close 추가하기