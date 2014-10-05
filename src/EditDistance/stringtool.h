#include <vector>
#include <string>


bool  getStrsFromFile(
        std::vector<std::string>& words,
		const char * FileName, 
		const char * splitor,
        bool lower = true,
		bool cleanBlank = true);
		
