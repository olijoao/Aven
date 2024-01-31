#pragma once

#include <aven/objects/Scene.h>
#include <aven/render/Renderer.h>
#include <aven/util/History.h>
#include <aven/volumeOperations/Filter.h>
#include <aven/volumeOperations/OperationTool.h>

namespace aven {

	class Project {
	public:
		Project(clamped<ivec3, 1, 256> const size);
		Project(clamped<ivec3, 1, 256> const size, Scene &&);

		static void saveToDisk(std::string const& filename, Project const&);
		static Project loadFromDisk(std::string const& filename);
		
		Renderer&		getRenderer();
		History<Scene>& getHistory();
		Scene&			getScene() ;

		// operations, state machine
		enum class Operation { None, Filter, ValueEdit, Tool, RotateCamera};

		Operation	getCurrentOperation();
		void		commitOperation();
		void		cancelOperation();
		void		startOperation(Filter* filter);
		void		executeFilterOperation();
		Filter*		getFilterOperation();			//returns nullptr if getCurrentOperation() != Filter

		void		startOperation(Tool_Brush* tool, MouseInput const&);
		void		continueToolOperation(MouseInput const&);
		void		endToolOperation(MouseInput const&);
		
		std::unique_ptr<OperationTool> currentToolOperation { nullptr };

		Operation	operation	= Operation::None;
	private:
		Renderer renderer;
		History<Scene> history;

		//current operation
		Filter*		op_filter	= nullptr;	// points to filter in case operation = Filter, otherwise nullptr	
	};

}