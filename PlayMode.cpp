#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <string>
#include <sstream>   // For std::stringstream
#include <iomanip>   // For std::setprecision and std::fixed


enum block_type
{
	wood,
	bomb,
	grass,
	man,
	metal,
	water
};


GLuint wood_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > wood_meshes(LoadTagDefault, []() -> MeshBuffer const* {
	MeshBuffer const* ret1 = new MeshBuffer(data_path("wood_test.pnct"));
	wood_meshes_for_lit_color_texture_program = ret1->make_vao_for_program(lit_color_texture_program->program);
	return ret1;
});

GLuint bomb_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > bomb_meshes(LoadTagDefault, []() -> MeshBuffer const* {
	MeshBuffer const* ret = new MeshBuffer(data_path("bomb.pnct"));
	bomb_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
	});

GLuint grass_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > grass_meshes(LoadTagDefault, []() -> MeshBuffer const* {
	MeshBuffer const* ret = new MeshBuffer(data_path("grassland.pnct"));
	grass_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
	});

GLuint man_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > man_meshes(LoadTagDefault, []() -> MeshBuffer const* {
	MeshBuffer const* ret = new MeshBuffer(data_path("man.pnct"));
	man_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
	});

GLuint metal_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > metal_meshes(LoadTagDefault, []() -> MeshBuffer const* {
	MeshBuffer const* ret = new MeshBuffer(data_path("metal.pnct"));
	metal_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
	});

GLuint water_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > water_meshes(LoadTagDefault, []() -> MeshBuffer const* {
	MeshBuffer const* ret = new MeshBuffer(data_path("water.pnct"));
	water_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
	});


Load< Scene > wood_scene(LoadTagDefault, []() -> Scene const* {
	return new Scene(data_path("wood_test.scene"), [&](Scene& wood, Scene::Transform* transform, std::string const& mesh_name1) {
		//Mesh const& mesh = wood_meshes->lookup(mesh_name1);

		/*wood.drawables.emplace_back(transform);
		Scene::Drawable& drawable = wood.drawables.back();*/
		
		//drawable.pipeline = lit_color_texture/*_program_pipeline;

		//drawable.pipeline.vao = wood_meshes_for_lit_color_texture_program;
		//drawable.pipeline.type = mesh.type;
		//drawable.pipeline.start = mesh.start;
		//drawable.pipeline.count = mesh.count;*/

		});
	});

void PlayMode::make_mesh(enum block_type type) {
	Mesh dummy;
	Mesh& mesh = dummy;

	switch (type)
	{
	case wood:
		mesh = wood_meshes->lookup("Cube.001");
		break;
	case bomb:
		mesh = bomb_meshes->lookup("bomb"); 
		break;
	case grass:
		mesh = grass_meshes->lookup("Cube");
		break;
	case man:
		mesh = man_meshes->lookup("Cylinder.004");
		break;
	case metal:
		mesh = metal_meshes->lookup("Cube.001");
		break;
	case water:
		mesh = water_meshes->lookup("Cube");
		break;
	default:
		return;
	}

	auto newTrans = new Scene::Transform();
	scene.drawables.emplace_back(newTrans);
	Scene::Drawable& drawable = scene.drawables.back();

	drawable.pipeline = lit_color_texture_program_pipeline;

	switch (type)
	{
	case wood:
		drawable.pipeline.vao = wood_meshes_for_lit_color_texture_program;
		break;
	case bomb:
		drawable.pipeline.vao = bomb_meshes_for_lit_color_texture_program;
		break;
	case grass:
		drawable.pipeline.vao = grass_meshes_for_lit_color_texture_program;
		break;
	case man:
		drawable.pipeline.vao = man_meshes_for_lit_color_texture_program;
		break;
	case metal:
		drawable.pipeline.vao = metal_meshes_for_lit_color_texture_program;
		break;
	case water:
		drawable.pipeline.vao = water_meshes_for_lit_color_texture_program;
		break;
	default:
		return;
	}
	drawable.pipeline.type = mesh.type;
	drawable.pipeline.start = mesh.start;
	drawable.pipeline.count = mesh.count;
}

// keep track of how many strips are there
uint16_t distance_unit = 0;
// form "strip", where one strip contains 2 grassland and n waters
void PlayMode::form_strip(uint8_t n) {

	if (distance_unit == 0) {
		end_distance.negX = 1.0f;
		end_distance.negY = -3.255f;
		end_distance.posX = 7.0f;
	}
	
	make_mesh(grass);
	Scene::Drawable& temp = scene.drawables.back();
	temp.transform->rotation *= glm::angleAxis(
		glm::radians(90.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));
	temp.transform->position = glm::vec3(0.0, distance_unit * 5.5f, 0.0f);

	glm::vec3 temp_pos = temp.transform->position;
	for (int i = 0; i < n; i++) {
		make_mesh(water);
		temp = scene.drawables.back();
		temp.transform->rotation *= glm::angleAxis(
			glm::radians(0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		temp.transform->position = glm::vec3(temp_pos[0] + 2.0f, temp_pos[1], 0.0f);
		temp_pos = temp.transform->position;
	}

	make_mesh(grass);
	temp = scene.drawables.back();
	temp.transform->rotation *= glm::angleAxis(
		glm::radians(270.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));
	temp.transform->position = glm::vec3(temp_pos[0] + 4.0f, temp_pos[1], 0.0f);

	make_mesh(grass);
	temp = scene.drawables.back();
	temp.transform->rotation *= glm::angleAxis(
		glm::radians(90.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));
	temp.transform->position = glm::vec3(temp_pos[0] - 8.0f, temp_pos[1], 0.0f);

	distance_unit++;
	end_distance.posY = temp_pos[1] + 2.0f;
}

PlayMode::PlayMode() : /*scene(*hexapod_scene), */scene(*wood_scene) {
	//get pointers to leg for convenience:
	//for (auto &transform : scene.transforms) {
	//	if (transform.name == "Hip.FL") hip = &transform;
	//	else if (transform.name == "UpperLeg.FL") upper_leg = &transform;
	//	else if (transform.name == "LowerLeg.FL") lower_leg = &transform;
	//}
	//if (hip == nullptr) throw std::runtime_error("Hip not found.");
	//if (upper_leg == nullptr) throw std::runtime_error("Upper leg not found.");
	//if (lower_leg == nullptr) throw std::runtime_error("Lower leg not found.");

	const uint8_t stream = 3;

	form_strip(stream);
	form_strip(stream);
	form_strip(stream);
	form_strip(stream);

	make_mesh(man);
	manTrans = scene.drawables.back().transform;
	manTrans->position = glm::vec3(4.0f, -3.25f, 1.0f);
	manTrans->rotation *= glm::angleAxis(
		glm::radians(180.0f),
		glm::vec3(0.0f, 0.0f, 1.0f));

	make_mesh(wood);
	Scene::Transform* tempMesh = scene.drawables.back().transform;
	tempMesh->position = glm::vec3(4.0f, -3.25f, 0.5f);


	/*hip_base_rotation = hip->rotation;
	upper_leg_base_rotation = upper_leg->rotation;
	lower_leg_base_rotation = lower_leg->rotation;*/

	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();
	camera->transform->rotation = glm::vec3(1.20102f, 0.002f, 0.0f);
	camera->transform->position = glm::vec3(3.98946f, -11.086f, 5.4959f);
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_KEYDOWN) {
		switch (evt.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		case SDLK_w:
			up.downs += 1;
			up.pressed = true;
			return true;
		case SDLK_a:
			left.downs += 1;
			left.pressed = true;
			return true;
		case SDLK_s:
			down.downs += 1;
			down.pressed = true;
			return true;
		case SDLK_d:
			right.downs += 1;
			right.pressed = true;
			return true;
		case SDLK_q:
			woodleft.downs++;
			woodleft.pressed = true;
			return true;
		case SDLK_e:
			woodright.downs++;
			woodright.pressed = true;
			return true;
		case SDLK_SPACE:
			space.downs++;
			space.pressed = true;
			timer_running = true;
			start_time = std::chrono::steady_clock::now();
		default:
			break;
		}
	} else if (evt.type == SDL_KEYUP) {
		switch (evt.key.keysym.sym)
		{
		case SDLK_w:
			up.pressed = false;
			return true;
		case SDLK_a:
			left.pressed = false;
			return true;
		case SDLK_s:
			down.pressed = false;
			return true;
		case SDLK_d:
			right.pressed = false;
			return true;
		case SDLK_q:
			woodleft.pressed = false;
			return true;
		case SDLK_e:
			woodright.pressed = false;
			return true;
		default:
			break;
		}
	} else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			glm::vec2 motion = glm::vec2(
				evt.motion.xrel / float(window_size.y),
				-evt.motion.yrel / float(window_size.y)
			);
			/*camera->transform->rotation = glm::normalize(
				camera->transform->rotation
				* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
			);*/
			//std::cout << camera->transform->position[0] << " " << camera->transform->position[1] << " " << camera->transform->position[2] << " " << std::endl;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {

	//slowly rotates through [0,1):
	/*wobble += elapsed / 10.0f;
	wobble -= std::floor(wobble);

	hip->rotation = hip_base_rotation * glm::angleAxis(
		glm::radians(5.0f * std::sin(wobble * 2.0f * float(M_PI))),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	upper_leg->rotation = upper_leg_base_rotation * glm::angleAxis(
		glm::radians(7.0f * std::sin(wobble * 2.0f * 2.0f * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	lower_leg->rotation = lower_leg_base_rotation * glm::angleAxis(
		glm::radians(10.0f * std::sin(wobble * 3.0f * 2.0f * float(M_PI))),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);*/

	//move camera:
	{

		//combine inputs into a move:
		constexpr float PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed) move.x =-1.0f;
		if (!left.pressed && right.pressed) move.x = 1.0f;
		if (down.pressed && !up.pressed) move.y =-1.0f;
		if (!down.pressed && up.pressed) move.y = 1.0f;

		//make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f)) move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::mat4x3 frame = camera->transform->make_local_to_parent();
		glm::vec3 frame_right = frame[0];
		//glm::vec3 up = frame[1];
		glm::vec3 frame_forward = -frame[2];

		glm::vec3 manTemp = manTrans->position;
		if ((end_distance.negX < manTemp[0] + move.x) && (manTemp[0] + move.x < end_distance.posX) &&
			(end_distance.negY < manTemp[1] + move.y) && (manTemp[1] + move.y < end_distance.posY))
			manTrans->position = glm::vec3(manTemp[0] + move.x, manTemp[1] + move.y, 1.0f);

	}

	//reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	woodleft.downs = 0;
	woodright.downs = 0;
	space.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	//set up light type and position for lit_color_texture_program:
	// TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f,-1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); //1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); //this is the default depth comparison function, but FYI you can change it.

	GL_ERRORS(); //print any errors produced by this setup code

	//scene.draw(*camera);
	scene.draw(*camera);

	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float H = 0.09f;

		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("See how fast can you reach the end",
			glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.90, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		if (timer_running) {
			// time processing code adapted from ChatGPT
			auto curr_time = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsed_seconds = curr_time - start_time;
			std::stringstream ss;
			ss << std::fixed << std::setprecision(3);  // Set the precision to 3 decimal places
			ss << elapsed_seconds.count();
			std::string message = ss.str();
			lines.draw_text("Time: " + message + " s",
				glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.80, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		}
		else {
			lines.draw_text("Press SPACE to start timer",
				glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.80, 0.0),
				glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
				glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		}
		lines.draw_text("WASD move you; QE moves closest block/bomb",
			glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.70, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		lines.draw_text("Wood/metal build platform, bomb destroy",
			glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.60, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
		lines.draw_text("You lose if platform you stand on is destoyed",
			glm::vec3(-aspect + 0.1f * H + ofs, /*-1.0 + 0.1f * H + ofs*/ 0.50, 0.0),
			glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
			glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
}


/* Temp place to store commands
node Maekfile.js && cd dist && game.exe && cd ..
*/