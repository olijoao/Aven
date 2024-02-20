#pragma once

#include <aven/objects/Scene.h>
#include <aven/objects/ViewPortCamera.h>
#include <aven/render/Renderer.h>
#include <aven/util/History.h>
#include <aven/volumeOperations/Filter.h>
#include <aven/volumeOperations/OperationTool.h>
#include <expected>


namespace aven {

	class Project {
	public:
		Project(c_ivec3<1, Volume::MAX_VOLUME_LENGTH> const size);
		Project(ViewPortCamera&& cam, Scene&&);

		static void saveToDisk(std::string const& filename, Project const&);
		static std::expected<Project, std::string> loadFromDisk(std::string const& filename);
		
		Scene&			getScene() ;

		// operations, state machine
		enum class Operation { None, Filter, ValueEdit, Tool, RotateCamera};

		Operation	getCurrentOperation();
		void		commit();
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

	public:
		ViewPortCamera camera;
		Renderer renderer;
		History<Scene> history;

	private:
		//current operation
		Filter*		op_filter	= nullptr;	// points to filter in case operation = Filter, otherwise nullptr	
	};

}