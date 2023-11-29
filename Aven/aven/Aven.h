#pragma once

#include <aven/Project.h>

namespace aven {
	void init();
	void destroy();

	Project&	getProject();
	void		newProject(clamped<ivec3, 1, 256> size);

	vec3 getForegroundColor();
	void setForegroundColor(vec3 color);
}