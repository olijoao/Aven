
#include <aven/volumeOperations/Tool.h>

namespace aven {

	Tool::Tool(std::string name) 
		:name(name) 
	{
		
	}

	std::string const& Tool::getName() const {
		return name;
	}

}