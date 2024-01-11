#pragma once

#include <aven/objects/Scene.h>
#include <aven/render/Renderer.h>
#include <aven/util/History.h>
#include <aven/volumeOperations/Filter.h>
#include <aven/volumeOperations/OperationTool.h>
#include <fstream>

namespace aven {

	class Project {
	public:
		Project(clamped<ivec3, 1, 256> const size);

		Renderer&		getRenderer();
		History<Scene>& getHistory();
		Scene&			getScene() ;

		// operations, state machine
		enum class Operation { None, Filter, ValueEdit, Tool };
		Operation	getCurrentOperation();
		void		commitOperation();
		void		cancelOperation();
		void		startOperation(Filter* filter);
		void		executeFilterOperation();
		Filter*		getFilterOperation();			//returns nullptr if getCurrentOperation() != Filter

		void		startOperation(Tool_Brush* tool, MouseInput const&);
		void		continueToolOperation(MouseInput const&);
		void		endToolOperation(MouseInput const&);
		Tool_Brush*	getToolOperation();				//returns nullptr if getCurrentOperation() != Tool
		std::unique_ptr<OperationTool>& getCurrentToolOperation();

	private:
		Renderer renderer;
		History<Scene> history;

		//current operation
		Operation	operation	= Operation::None;
		Filter*		op_filter	= nullptr;	// points to filter in case operation = Filter, otherwise nullptr	
		Tool_Brush*	op_tool		= nullptr;	// points to tool in case operation = Tool, otherwise nullptr	
		std::unique_ptr<OperationTool> currentToolOperation{ nullptr };
	};

}