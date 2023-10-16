#include "dialog.h"



void dialog_add_module(Platform& plat, AI& ai) {

	int t = -1, pri = -1, err = -1;
	Module* module = nullptr;
	std::cout << "Please, choose module type:" << std::endl;
	std::cout << "0. Manager" << std::endl;
	std::cout << "1. Energy Generator" << std::endl;
	std::cout << "2. Sensor" << std::endl;

	while (t < 0 || t > 2)
		std::cin >> t;
	std::cout << "Enter priority:" << std::endl;

	while (pri < 0 || err == -1)
		err = getNum(pri);
		//std::cin >> pri;

	if (t == 0) {
		std::cout << "Enter maximum number of managed platforms:" << std::endl;
		int m = -1, r = -1;

		while (m < 1)
			std::cin >> m;
		std::cout << "Enter radius:" << std::endl;
		r = -1;

		while (r < 1)
			std::cin >> r;

		Manager* man = new Manager(1, pri, 1, 1, plat, m, r);
		module = static_cast<Module*>(man);
		plat.new_manager(man);
		ai.add_manager(man);
		man->add_platform(&plat);
	}

	else if (t == 1) {
		int o = -1;
		std::cout << "Enter out energy level" << std::endl;

		while (o < 1) {
			std::cin >> o;
		}

		module = static_cast<Module*>(new EnergyGenerator(o, pri, 1, 1, plat));
	}

	else {
		int r = -1;
		std::cout << "Enter radius" << std::endl;

		while (r < 1) {
			std::cin >> r;
		}

		int d = -1;
		std::cout << "Enter direction" << std::endl;
		std::cout << "0. Up" << std::endl;
		std::cout << "1. Right" << std::endl;
		std::cout << "2. Down" << std::endl;
		std::cout << "3. Left" << std::endl;

		while (d < 0 || d > 3) {
			std::cin >> d;
		}

		int a = -1;
		std::cout << "Enter angle" << std::endl;
		std::cout << "0. 0°" << std::endl;
		std::cout << "1. 90°" << std::endl;
		std::cout << "2. 180°" << std::endl;
		std::cout << "3. 270°" << std::endl;
		std::cout << "4. 360°" << std::endl;

		while (a < 0 || d > 4) {
			std::cin >> a;
		}

		module = static_cast<Module*>(new Sensor(r, (Direction)d, Angles(a), 1, pri, 1, 1, plat));
	}

	plat.add_module(module);
}


Object* dialog_add_object(EnvDescriptor* e, AI& ai) {
	int x = -1, y = -1, type = -1, manage = 0;
	Object* object = nullptr;
	Module* md = nullptr;
	std::map<std::pair<int, int>, Object*>::iterator it = e->map.begin();

	while (x < 0 || y < 0 || type < 0 || type > 2) {
		std::cout << "Enter x" << std::endl;
		std::cin >> x;
		std::cout << "Enter y" << std::endl;
		std::cin >> y;
		std::cout << "Enter type: \n0. Obctacle\n1. Something Interesting\n2. Platform" << std::endl;
		std::cin >> type;
	}

	if (type != 2) {
		Object* obj = new Object(std::pair<int, int>(x, y), (ObjType)type, e);
		return obj;
	}

	else {
		type = -1;
		int maxmodulesnum = 0, speed, dir;

		while (type < 0 || type > 3) {
			std::cout << "Enter type: \n0. ManRobot\n1. ScRobot\n2. ManCentre\n3. ObsCentre" << std::endl;
			std::cin >> type;
		}

		std::cout << "Enter max number of modules" << std::endl;
		std::cin >> maxmodulesnum;

		if (type == (int)ComponentTypes::ManRobot || type == (int)ComponentTypes::ScRobot) {
			std::cout << "Enter speed value" << std::endl;
			std::cin >> speed;
			std::cout << "Enter direction: 0 - up, 4 - left" << std::endl;
			std::cin >> dir;

			if (type == (int)ComponentTypes::ManRobot) {
				manage = 1;
				ManageRobot* obj = new ManageRobot(maxmodulesnum, 10, "", speed, (Direction)(dir % 4), std::pair<int, int>(x, y), e);
				object = dynamic_cast<Object*>(obj);

			}

			else {
				ScoutRobot* obj = new ScoutRobot(maxmodulesnum, 10, "", speed, (Direction)(dir % 4), std::pair<int, int>(x, y), e);
				object = dynamic_cast<Object*>(obj);
			}
		}

		else if (type == (int)ComponentTypes::ManCentre) {
			manage = 1;
			ManageCentre* obj = new ManageCentre(maxmodulesnum, 10, "", std::pair<int, int>(x, y), e);
			object = dynamic_cast<Object*>(obj);
		}

		else {
			ObserveCentre* obj = new ObserveCentre(maxmodulesnum, 10, "", std::pair<int, int>(x, y), e);
			object = dynamic_cast<Object*>(obj);
		}

		std::cout << "Please, choose the way to add modules:" << std::endl;
		std::cout << "0. Randomly" << std::endl;
		std::cout << "1. By myself" << std::endl;
		type = -1;

		while (type < 0 || type > 1)
			std::cin >> type;

		if (type == 0) {

			for (int j = 0; j < maxmodulesnum; j++) {
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
					Sensor* ss = new Sensor(rad, d, ang, enlev, prior, enab, price, *dynamic_cast<Platform*>(object));
					md = dynamic_cast<Module*>(ss);
					ss = static_cast<Sensor*>(md);
				}

				else if (t == NodeTypes::GenNode) {
					EnergyGenerator* eg = new EnergyGenerator(enlev, prior, enab, price, *dynamic_cast<Platform*>(object));
					md = dynamic_cast<Module*>(eg);

				}

				else if (t == NodeTypes::ManNode) {

					if (!manage) break;

					int maxn = std::rand() % 10;
					int rad = std::rand() % 20;
					Manager* man = new Manager(enlev, prior, enab, price, *dynamic_cast<Platform*>(object), maxn, rad);
					md = dynamic_cast<Module*>(man);
					dynamic_cast<Platform*>(object)->new_manager(man);
					ai.add_manager(man);
					//man->add_platform(platf);
					man->add_platform(dynamic_cast<Platform*>(object));

					while (it != e->map.end()) {

						if (dynamic_cast<Platform*>(it->second) && !dynamic_cast<Platform*>(it->second)->get_managed()) {
							dynamic_cast<Platform*>(it->second)->new_manager(man);
							man->add_platform(dynamic_cast<Platform*>(it->second));
						}

						it++;
					}
				}
				int q = 5;
				std::vector<Module*> mi = dynamic_cast<Platform*>(object)->get_modules();

				if (dynamic_cast<Platform*>(object)->add_module(md)) {
					mi.push_back(md);
					//dynamic_cast<Platform*>(plat)->set_modules(mi);
				}
			}
		}

		else {

			for (int i = 0; i < maxmodulesnum; i++) {
				type = -1;
				std::cout << "0. Quit" << std::endl;
				std::cout << "1. One more module" << std::endl;

				while (type < 0 || type > 1) {
					std::cin >> type;
				}

				if (!type) break;

				else dialog_add_module(*dynamic_cast<Platform*>(object), ai);
			}
		}
	}
	return object;
}

void dialog_print_modules(Platform* plat) {
	int i = 0;
	std::vector<std::string> types = { "Manager", "Energy Generator", "Sensor" };
	for (auto m : plat->get_modules()) {
		std::cout << i << ". " << types[(int)m->get_type()] << std::endl;
		i++;
	}

	while (true) {
		std::cout << "Do you want to see information about some module?" << std::endl;
		std::cout << "0. No\n1. Yes" << std::endl;

		i = -1;
		while (i < 0 || i > 1) {
			std::cin >> i;
		}
		if (!i) return;

		i = 0;
		std::vector<std::string> types = { "Manager", "Energy Generator", "Sensor" };
		for (auto m : plat->get_modules()) {
			std::cout << i << ". " << types[(int)m->get_type()] << std::endl;
			i++;
		}

		i = -1;
		std::cout << "Enter index of module" << std::endl;
		while (i < 0 || i >= plat->get_modules().size()) {
			std::cin >> i;
		}

		auto* module = plat->get_modules()[i];
		std::cout << "Priority: " << module->get_priority() << std::endl;
		std::cout << "Enable: " << module->get_working() << std::endl;
		std::cout << "Energy level ( < 0 => no out energy ): " << module->get_out_energy_level() - module->get_in_energy_level() << std::endl;

		if (module->get_type() == NodeTypes::ManNode) {
			Manager* man = static_cast<Manager*>(module);
			std::cout << "Maximum number of managed platforms: " << man->get_max_num_plat() << std::endl;
			std::cout << "Current number of platforms: " << man->get_table().component.size() << std::endl;
			std::cout << "Radius: " << man->get_radius() << std::endl;
		}

		if (module->get_type() == NodeTypes::SenseNode) {
			Sensor* sens = static_cast<Sensor*>(module);
			std::cout << "Radius: " << sens->get_radius() << std::endl;
			std::vector<std::string> dir = { "Up", "Right", "Down", "Left" };
			std::cout << "Direction: " << dir[(int)sens->get_vector()] << std::endl;
			std::vector<std::string> angles = { "0 deg", "90 deg", "180 deg", "270 deg", "360 deg" };
			std::cout << "Angle of view: " << angles[(int)sens->get_angle()] << std::endl;
		}
	}
}

void dialog_print_platform(Object* obj) {
	std::vector<std::string> dir = { "Up", "Right", "Down", "Left" };
	if (dynamic_cast<ScoutRobot*>(obj)) {
		std::cout << "Scout Robot" << std::endl;
		std::cout << "Speed value: " << dynamic_cast<ScoutRobot*>(obj)->get_speed_val() << std::endl;
		std::cout << "Direction: " << dir[(int)dynamic_cast<ScoutRobot*>(obj)->get_dir()] << std::endl;
	}

	if (dynamic_cast<ManageRobot*>(obj)) {
		std::cout << "Manage Robot" << std::endl;
		std::cout << "Speed value: " << dynamic_cast<ManageRobot*>(obj)->get_speed_val() << std::endl;
		std::cout << "Direction: " << dir[(int)dynamic_cast<ManageRobot*>(obj)->get_dir()] << std::endl;
	}
	if (dynamic_cast<ManageCentre*>(obj)) {
		std::cout << "Manage Centre" << std::endl;
	}

	if (dynamic_cast<ObserveCentre*>(obj)) {
		std::cout << "Observe Centre" << std::endl;
	}

	Platform* plat = dynamic_cast<Platform*>(obj);
	std::cout << "Max num of modules: " << plat->get_max_modules_num() << std::endl;
	std::cout << "Managed: " << plat->get_managed() << std::endl;
	std::cout << "Cur modules num: " << plat->get_modules().size() << std::endl;
	std::cout << "Do you want to see info about modules?" << std::endl;
	std::cout << "0. No" << std::endl;
	std::cout << "1. Yes" << std::endl;
	int i = -1;

	while (i < 0 || i > 1) {
		std::cin >> i;
	}

	if (!i) return;
	dialog_print_modules(plat);
}


void dialog_print_object(Application& app) {
	int x, y;
	std::cout << "Enter x" << std::endl;
	std::cin >> x;
	std::cout << "Enter y" << std::endl;
	std::cin >> y;
	if (app.enviroment->map.find(std::pair<int, int>(x, y)) == app.enviroment->map.end()) {
		std::cout << "No object with such coordinates" << std::endl;
		return;
	}
	Object* obj = app.enviroment->map.find(std::pair<int, int>(x, y))->second;
	if (obj->get_obj_type() == ObjType::MyPlatform) {
		std::cout << "Platform" << std::endl;
		dialog_print_platform(obj);
	}

	else if (obj->get_obj_type() == ObjType::Obstacle) {
		std::cout << "Obstacle" << std::endl;
	}

	else {
		std::cout << "Point of interest" << std::endl;
	}
}


void dialog_do_all(Application& app) {
	int num = 1;

	while (num != 0) {
		app.enviroment->print_enviroment();
		std::cout << "0. Continue\n 1. Add object\n 2. Print info about object" << std::endl;
		std::cin >> num;

		if (num == 1) {
			Object* object = dialog_add_object(app.enviroment, app.ai);
			app.enviroment->add_object(object);
		}

		else if (num == 2) {
			dialog_print_object(app);
		}
	}
}
