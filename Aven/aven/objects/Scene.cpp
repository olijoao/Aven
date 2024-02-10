

#include <aven/objects/Scene.h>
#include <aven/objects/Volume.h>
#include <fstream>
  
namespace aven {

	Scene::Scene(c_ivec3<1, Volume::MAX_VOLUME_LENGTH> size)
		: volume(std::make_shared<Volume const>(Volume(size.getValue(), {0,0,0}, 1, 1, 1, true, false))),
		renderSettings(std::make_shared<RenderSettings const>())
	{
		//..	
	}
	

	Scene::Scene(RenderSettings&& rs, Volume&& vol)
		:volume(std::make_shared<Volume>(vol)),
		renderSettings(std::make_shared<RenderSettings>(rs)){
		//..	
	}


	void Scene::serialize(std::ofstream& out, Scene const& scene) {
		RenderSettings::serialize(out, *scene.renderSettings);
		Volume::serialize(out, *scene.volume);
	}


	std::expected<Scene, std::string> Scene::deserialize(std::ifstream& in ) {	

		auto rs = RenderSettings::deserialize(in);
		if (!rs.has_value())
			return std::unexpected(rs.error());

		auto vol = Volume::deserialize(in);
		if (!vol.has_value())
			return std::unexpected(vol.error());

		return Scene(std::move(rs.value()), std::move(vol.value()));
	}
	
}




