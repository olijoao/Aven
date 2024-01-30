
#include <aven/Project.h>


namespace aven {

	Project::Project(clamped<ivec3, 1, 256> const size)
		:history(Scene(size)), 
		renderer(std::max(size.getValue().x, std::max(size.getValue().y,size.getValue().z)/2 + 100))
	{
	
	}

	Project::Project(clamped<ivec3, 1, 256> const size, Scene&& scene) 
		:history(std::move(scene)),
		renderer(std::max(size.getValue().x, std::max(size.getValue().y, size.getValue().z) / 2 + 100))
	{
			
	}
	

	void Project::saveToDisk(std::string const& filename, Project const& project) {
		std::ofstream out(filename, std::ios_base::binary);
		if (!out)
			throw std::runtime_error("could not open file stream \"" + filename + "\"");
		
		auto scene = project.history.getCurrent();
		Scene::serialize(out, scene);
	}


	Project Project::loadFromDisk(std::string const& filename) {
		std::ifstream in(filename, std::ios_base::binary);
		if (!in)
			throw std::runtime_error("could not open file stream \"" + filename + "\"");

		Scene scene = Scene::deserialize(in);
		return Project(scene.volume->getSize(), std::move(scene));
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



	void Project::startOperation(Tool_Brush* tool, MouseInput const& mouseInput) {
		assert(tool);
		assert(operation == Operation::None);
		assert(currentToolOperation == nullptr);
	
		operation = Operation::Tool;

		currentToolOperation = std::move(tool->start(getScene()));
		tool->apply(getScene(), mouseInput);
		renderer.resetIterations();
	}



	void Project::continueToolOperation(MouseInput const& mouseInput) {
		assert(operation == Operation::Tool);
		assert(currentToolOperation);

		if (currentToolOperation) {
			currentToolOperation->tool->apply(getScene(), mouseInput);
			renderer.resetIterations();
		}
	}


	void Project::endToolOperation(MouseInput const& mouseInput) {
		assert(operation == Operation::Tool);
		assert(currentToolOperation);

		if (currentToolOperation) {
			currentToolOperation->tool->end(getScene(), mouseInput, std::move(currentToolOperation));
			renderer.resetIterations();
		}
		commitOperation();
	}



	void Project::executeFilterOperation() {
		assert(op_filter && operation == Operation::Filter);
		history.cancelCurrent();

		auto data = op_filter->operator()(getScene().volume->getVolumeData());
		auto sptr_volume = getScene().volume;
		getScene().volume = std::make_shared<Volume>(
			sptr_volume->getVolumeData().getSize(),
			sptr_volume->pos,
			sptr_volume->sigma_t,
			sptr_volume->density,
			sptr_volume->stepSize,
			sptr_volume->isRendering_BondingBox,
			sptr_volume->renderingMode_Hybrid,
			std::make_shared<VolumeData>(std::move(data))
		);

		renderer.resetIterations();
	} 


	Renderer&		Project::getRenderer()	{ return renderer; }
	History<Scene>& Project::getHistory()	{ return history; }
	Scene&			Project::getScene()		{ return history.getCurrent(); }

}