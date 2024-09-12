#include "Mode.hpp"

#include "Scene.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <chrono>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	void make_mesh(enum block_type type);
	void form_strip(uint8_t n);

	//----- game state -----

	//input tracking:
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, woodleft, woodright, space;

	//local copy of the game scene (so code can change it during gameplay):
	Scene scene;
	//Scene wood;

	//hexapod leg to wobble:
	/*Scene::Transform *hip = nullptr;
	Scene::Transform *upper_leg = nullptr;
	Scene::Transform *lower_leg = nullptr;
	glm::quat hip_base_rotation;
	glm::quat upper_leg_base_rotation;
	glm::quat lower_leg_base_rotation;
	float wobble = 0.0f;*/

	std::chrono::steady_clock::time_point start_time;
	bool timer_running = false;


	Scene::Transform* manTrans = nullptr;

	struct end_dis {
		float posX;
		float posY;
		float negX;
		float negY;
	} end_distance;

	Scene::Transform* woodTrans = nullptr;
	
	//camera:
	Scene::Camera *camera = nullptr;

};
