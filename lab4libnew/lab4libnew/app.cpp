#include "app.h"


std::map<Object*, std::vector<EnvErr>> Application::get_env_status() {
	errors = enviroment->get_errors();
	return errors;
}


void Application::kick_ai(std::map<Object*, std::vector<EnvErr>> errs, int a, int b) {
	ai.get_errors(errs, a, b);
	ai.get_all_info();
	for (auto p : errs) {
		p.second.clear();
	}
	errs.clear();
}


std::map<Object*, std::vector<Instructions>> Application::check_ai_status() {
	return ai.give_instructions();
}


void Application::change_enviroment(std::map<Object*, std::vector<Instructions>> inst) {
	enviroment->get_instructions(inst);
	enviroment->move_objs();
	for (auto p : inst) {
		p.second.clear();
	}
	inst.clear();
}


void Application::work() {
	enviroment->print_enviroment();
	while(ai.get_int_num() < enviroment->get_num_interests() && ai.get_timer() < 20) {
		kick_ai(get_env_status(), enviroment->get_size().first, enviroment->get_size().second);
		change_enviroment(check_ai_status());
		enviroment->print_enviroment();
	}
	ai.print_enviroment();
}


/**
 * Create random application with 3 params.
 *
 * \param width Width of enviroment
 * \param length Length of enviroment
 * \param objnum Max number of objects of enviroment
 */
Application::Application(int width, int length, int objnum) {
	enviroment = new EnvDescriptor(width, length);
	Object* plat = nullptr;
	if (enviroment != nullptr) {
		std::map<std::pair<int, int>, Object*>::iterator it = enviroment->map.begin();
		for (int i = 0; i < objnum; i++) {
			ObjType r = (ObjType)(std::rand() % 3);
			std::pair<int, int> coord(std::rand() % width, std::rand() % length);
			if (r != ObjType::MyPlatform) {
				Object* obj = new Object(coord, r, enviroment);
				enviroment->add_object(obj);
				if (r == ObjType::SmthInteresting) {
				}
			}
			else {
				bool mobile = std::rand() % 2, manage = std::rand() % 2;
				ComponentTypes modtype = (ComponentTypes)(!mobile * 2 + !manage);
				int price = std::rand() % 10;
				int maxmodnum = std::rand() % 20;
				std::string desc = "";
				if (mobile) {
					Direction dir = (Direction)(std::rand() % 4);
					int speed = (std::rand() % 5) + 1;
					if (modtype == ComponentTypes::ManRobot) {
						ManageRobot* mr = new ManageRobot(maxmodnum, price, desc, speed, dir, coord, enviroment);
						plat = dynamic_cast<Object*>(mr);
					}
					else if (modtype == ComponentTypes::ScRobot) {
						ScoutRobot* sr = new ScoutRobot(maxmodnum, price, desc, speed, dir, coord, enviroment);
						plat = dynamic_cast<Object*>(sr);
					}
				}
				else if (modtype == ComponentTypes::ManCentre) {
					ManageCentre* mc = new ManageCentre(maxmodnum, price, desc, coord, enviroment);
					plat = dynamic_cast<Object*>(mc);
				}
				else if (modtype == ComponentTypes::ObsCentre) {
					ObserveCentre* oc = new ObserveCentre(maxmodnum, price, desc, coord, enviroment);
					plat = dynamic_cast<Object*>(oc);
				}
				Module* md = nullptr;
				Platform* platf = dynamic_cast<Platform*>(plat);
				for (int j = 0; j < maxmodnum; j++) {
					bool here = std::rand() % 2;
					if (!here) continue;
					NodeTypes t = (NodeTypes)(std::rand() % 3);
					int prior = std::rand() % 10;
					int enlev = std::rand() % 10;
					bool enab = std::rand() % 2;
					int price = std::rand() % 10;
					if (t == NodeTypes::SenseNode) {
						int rad = std::rand() % 10;
						Direction d = (Direction)(std::rand() % 4);
						Angles ang = (Angles)(std::rand() % 5);
						Sensor* ss = new Sensor(rad, d, ang, enlev, prior, enab, price, *dynamic_cast<Platform*>(plat));
						md = dynamic_cast<Module*>(ss);
						ss = static_cast<Sensor*>(md);
						int yyyyyy = 0;
					}
					else if (t == NodeTypes::GenNode) {
						EnergyGenerator* eg = new EnergyGenerator(enlev, prior, enab, price, *dynamic_cast<Platform*>(plat));
						md = dynamic_cast<Module*>(eg);

					}
					else if (t == NodeTypes::ManNode) {
						if (!manage) break;
						int maxn = std::rand() % 10;
						int rad = std::rand() % 20;
						Manager* man = new Manager(enlev, prior, enab, price, *dynamic_cast<Platform*>(plat), maxn, rad);
						md = dynamic_cast<Module*>(man);
						platf->new_manager(man);
						ai.add_manager(man);
						//man->add_platform(platf);
						man->add_platform(dynamic_cast<Platform*>(plat));
						while (it != enviroment->map.end()) {

							if (dynamic_cast<Platform*>(it->second) && !dynamic_cast<Platform*>(it->second)->get_managed()) {
								dynamic_cast<Platform*>(it->second)->new_manager(man);
								man->add_platform(dynamic_cast<Platform*>(it->second));
							}
							it++;
						}
					}
					int q = 5;
					std::vector<Module*> mi = dynamic_cast<Platform*>(plat)->get_modules();
					if (dynamic_cast<Platform*>(plat)->add_module(md)) {
						mi.push_back(md);
						//dynamic_cast<Platform*>(plat)->set_modules(mi);
					}
				}
				enviroment->add_object(plat);
			}
		}

	}
}
