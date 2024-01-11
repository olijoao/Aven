#include <aven/volumeOperations/OperationTool.h>



namespace aven {
	OperationTool::OperationTool(Tool_Brush* tool, ivec3 size, clamped<int, 1, 255> opacity, volumeOps::BlendMode blendMode)
	:tool(tool), volumeData(size), opacity(opacity), blendMode(blendMode)
	{
		assert(tool);
	}
}
