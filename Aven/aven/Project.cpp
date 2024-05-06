
#include <aven/Project.h>


namespace aven {

	Project::Project(c_ivec3<1, Volume::MAX_VOLUME_LENGTH> const size)
		:history(Scene(size)),
		camera(vec3(0, size.getValue().y/2, std::max(size.getValue().x, std::max(size.getValue().y,size.getValue().z)/2 + 100)), vec3(0, size.getValue().y/2, 0), 90.0f)
	{

	}

	Project::Project(ViewPortCamera&& camera, Scene&& scene) 
		:history(std::move(scene)),
		camera(std::move(camera))
	{
		//...	
	}
	

	void Project::saveToDisk(std::string const& filename, Project const& project) {
		std::ofstream out(filename, std::ios_base::binary);
		if (!out.is_open())
			throw std::runtime_error("could not open file stream \"" + filename + "\"");

		// magic number
		char magic[] = "aven volume";
		out.write((char*) &magic[0], sizeof(magic));

		// version
		int version = 0;
		out.write((char*)&version, sizeof(version));

		// camera 
		ViewPortCamera::serialize(out, project.camera);

		// Scene
		auto scene = project.history.getCurrent();
		Scene::serialize(out, scene);
	}


	std::expected<Project, std::string> Project::loadFromDisk(std::string const& filename) {
		std::ifstream in(filename, std::ios_base::binary);
		if (!in.is_open())
			return std::unexpected("could not open file stream \"" + filename + "\"");

		const std::string error = "error occured while deserializing Project.";

		// magic number
		char magic[] = "aven volume";
		char idIn[sizeof(magic)];
		if (!in.read(idIn, sizeof(idIn)))
			return std::unexpected(error);
		if (strcmp(magic, idIn) != 0)
			return std::unexpected("Could not find file identifier \"aven volume\" at start of file.");

		// version
		int version;
		if (!in.read((char*)&version, sizeof(version)))
			return std::unexpected(error);
		if (version != 0)
			return std::unexpected("File version not supported");
		
		auto cam = ViewPortCamera::deserialize(in);
		if (!cam.has_value())
			return std::unexpected(cam.error());

		auto scene = Scene::deserialize(in);
		if (!scene.has_value())
			return std::unexpected(scene.error());

		return Project(std::move(cam.value()), std::move(scene.value()));
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


	void Project::commit() {
		assert(operation == Operation::None);
		history.commit();
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
		history.commit();
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



	void Project::startOperation(Tool_Brush* tool, vec2 const& mousePos_01) {
		assert(tool);
		assert(operation == Operation::None);
		assert(currentToolOperation == nullptr);
	
		operation = Operation::Tool;

		currentToolOperation = std::move(tool->start(getScene()));
		tool->apply(getScene(), mousePos_01);
		renderer.resetIterations();
	}



	void Project::continueToolOperation(vec2 const& mousePos_01) {
		assert(operation == Operation::Tool);
		assert(currentToolOperation);

		if (currentToolOperation) {
			currentToolOperation->tool->apply(getScene(), mousePos_01);
			renderer.resetIterations();
		}
	}


	void Project::endToolOperation(vec2 const& mousePos_01) {
		assert(operation == Operation::Tool);
		assert(currentToolOperation);

		if (currentToolOperation) {
			currentToolOperation->tool->end(getScene(), mousePos_01, std::move(currentToolOperation));
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
			sptr_volume->transformation,
			sptr_volume->density,
			sptr_volume->stepSize,
			sptr_volume->isRendering_BondingBox,
			sptr_volume->renderingMode_Hybrid,
			std::make_shared<VolumeData>(std::move(data))
		);

		renderer.resetIterations();
	} 


	Scene& Project::getScene()	{
		return history.getCurrent(); 
	}

}