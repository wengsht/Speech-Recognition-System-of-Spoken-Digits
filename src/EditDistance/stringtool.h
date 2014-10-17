#include <vector>
#include <string>

void saveStrsToFile(const char *FileName,
						const std::vector<int> ind,
						const std::vector<std::string>& v,
						char splitor = ' ');

bool  getStrsFromFile(
        std::vector<std::string>& words,
		const char * FileName, 
		const char * splitor,
        bool lower = true,
		bool cleanBlank = true);
		
