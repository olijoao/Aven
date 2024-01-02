#include <aven/volumeOperations/OperationTool.h>



namespace aven {
	OperationTool::OperationTool(Tool* tool, ivec3 size, clamped<int, 1, 255> opacity, BlendMode blendMode)
	:tool(tool), volumeData(size), opacity(opacity), blendMode(blendMode)
	{
		assert(tool);
	}
}
