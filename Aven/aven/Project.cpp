
#include <aven/Project.h>


namespace aven {

	Project::Project(clamped<ivec3, 1, 256> size)
		:history(Scene(size)), 
		renderer(std::max(size.getValue().x, std::max(size.getValue().y, size.getValue().z)/2 + 100))
	{
		history.commit();
	}


	Project::Operation Project::getCurrentOperation() {
		return operation;
	}

	void Project::startOperation(Filter* filter) {
		assert(filter);	
		assert(operation == Operation::None);

		operation = Operation::Filter;
		op_filter = filter;

		executeFilterOperation();

		if (!filter->hasProperties()) {
			commitOperation();
			return;
		}
	}


	void Project::commitOperation() {
		switch (operation) {
		case Operation::Filter:
			op_filter = nullptr;
			break;
		case Operation::Tool:
			op_tool = nullptr;
			break;
		default:
			assert(false);
		}

		operation = Operation::None;
		getHistory().commit();
	}



	void Project::cancelOperation() {
		history.cancelCurrent();

		switch (operation) {
		case Operation::Filter:
			op_filter = nullptr;
			break;
		default:
			assert(false);
		}

		operation = Operation::None;
		renderer.resetIterations();
	}


	Filter* Project::getFilterOperation() {
		return op_filter;
	}



	void Project::startOperation(Tool* tool, MouseInput const& mouseInput) {
		assert(tool);
		assert(operation == Operation::None);

		operation = Operation::Tool;
		op_tool = tool;

		tool->start(getScene(), mouseInput);
		renderer.resetIterations();
	}



	void Project::continueToolOperation(MouseInput const& mouseInput) {
		assert(operation == Operation::Tool);
		assert(op_tool);

		if (op_tool) {
			op_tool->apply(getScene(), mouseInput);
			renderer.resetIterations();
		}
	}


	void Project::endToolOperation(MouseInput const& mouseInput) {
		assert(operation == Operation::Tool);
		assert(op_tool);

		if (op_tool) {
			op_tool->end(getScene(), mouseInput);
			renderer.resetIterations();
		}
		commitOperation();
	}


	Tool* Project::getToolOperation() {
		return op_tool;
	}

	std::unique_ptr<OperationTool>& Project::getCurrentToolOperation() {
		return currentToolOperation;
	}



	void Project::executeFilterOperation() {
		assert(op_filter && operation == Operation::Filter);
		history.cancelCurrent();
		getScene().volume = op_filter->operator()(getScene().volume);
		renderer.resetIterations();
	}



	Renderer&		Project::getRenderer()	{ return renderer; }
	History<Scene>& Project::getHistory()	{ return history; }
	Scene&			Project::getScene()		{ return history.getCurrent(); }



}