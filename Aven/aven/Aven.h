#pragma once

#include <aven/Project.h>

namespace aven {
	void init();
	void destroy();

	Project&	getProject();
	void		newProject(c_ivec3<1, Volume::MAX_VOLUME_LENGTH> size);
	void		saveProject(std::string const& filename);
	void		loadProject(std::string const& filename);

	vec3 getForegroundColor();
	void setForegroundColor(vec3 color);
}