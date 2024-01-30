
#include <aven/Aven.h>
#include <aven/gui/Gui.h>
#include <aven/objects/BrickPool.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <aven/volumeOperations/FilterManager.h>
#include <aven/volumeOperations/OperationTool.h>
#include <aven/volumeOperations/ToolManager.h>
#include <fstream>
#include <memory>


namespace aven{
	std::unique_ptr<Project> project;
	vec3 foregroundColor = vec3(1,1,1);
}


namespace aven {
	void init() {
		brickPool::init();
		volumeOps::init();
		filterManager::init();
		toolManager::init();
		newProject( ivec3( 128, 128, 128));
	}


	void destroy() {
		project.reset();
		toolManager::destroy();
		filterManager::destroy();
		volumeOps::destroy();
		brickPool::destroy();
	}


	Project& getProject() {
		return *project;
	}


	void newProject(clamped<ivec3, 1, 256> size) {
		// dtor before creating a new project in order to free memory
		project = nullptr;
		project = std::make_unique<Project>(size);
	}

	void aven::saveProject(std::string const& filename) {
		if(project)
			Project::saveToDisk(filename, *project);
	}
	
	void aven::loadProject(std::string const& filename) {
		project = nullptr;
		project = std::make_unique<Project>(Project::loadFromDisk(filename));
	}

	vec3 getForegroundColor() {
		return foregroundColor;
	}


	void setForegroundColor(vec3 color) {
		foregroundColor = color;
	}

}