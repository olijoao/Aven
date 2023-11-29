#include <aven/volumeOperations/OperationTool.h>



namespace aven {
	OperationTool::OperationTool(Tool* tool, ivec3 size, vec3 color, clamped<int, 1, 255> opacity, BlendMode blendMode)
	:tool(tool), texture_mask(size), color(color), opacity(opacity), blendMode(blendMode)
	{
		assert(tool);
	}
}
