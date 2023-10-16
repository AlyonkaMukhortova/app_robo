#include "objects.h"

EnvDescriptor::~EnvDescriptor() {
	for (auto m : map) {
		Object* obj = m.second;
		if (dynamic_cast<ScoutRobot*>(m.second)) {
			delete dynamic_cast<ScoutRobot*>(obj);
		}
		else if (dynamic_cast<ManageRobot*>(obj)) {
			auto* p = dynamic_cast<ManageRobot*>(obj);
			delete p;
		}
		else if (dynamic_cast<ManageCentre*>(obj)) {
			auto* p = dynamic_cast<ManageCentre*>(obj);
			delete p;
		}
		else if (dynamic_cast<ObserveCentre*>(obj)) {
			auto* p = dynamic_cast<ObserveCentre*>(obj);
			delete p;
		}
		else {
			delete m.second;
		}
	}
	for (auto m : curerrors) {
		m.second.clear();
	}
	curerrors.clear();
	for (auto m : curinstructions) {
		m.second.clear();
	}
	curinstructions.clear();
	map.clear();
}

EnvDescriptor::EnvDescriptor(int w, int l) {
	width = w;
	length = l;
}

void EnvDescriptor::add_object(Object* o) {
	if (budget < 0) {
		//std::cout << "Can't add platform" << std::endl;
		return;
	}
	int i = 1;
	if (dynamic_cast<Platform*>(o)) {
		i += dynamic_cast<Platform*>(o)->get_modules().size();
	}
	budget -= i;
	map[o->get_coordinates()] = o;
	if (o->get_obj_type() == ObjType::SmthInteresting) numinterests++;
}

void EnvDescriptor::change_env_size(int w, int l) {
	width = w;
	length = l;
}

std::vector<EnvErr> ManagePlatform::get_error_messages(Platform* p) {
	return  p->get_obj_err();
}

Object* EnvDescriptor::check_coordinates(std::pair<int, int> p) {
	std::map<std::pair<int, int>, Object*>::iterator it;
	it = map.find(p);
	if (p.first == 1 && p.second == 4) {
		int r = 0;
	}
	if (it == map.end())
		return nullptr;
	return (it->second);
}

std::pair<int, int> EnvDescriptor::get_size() {
	return std::make_pair(width, length);
}

void EnvDescriptor::delete_object(std::pair<int, int> coords) {
	map.erase(coords);
}

std::vector<EnvErr> Object::get_obj_err() {
	return curerrs;
}

std::map<Object*, std::vector<EnvErr>> EnvDescriptor::get_errors() {
	std::map<Object*, std::vector<EnvErr>> errs;
	std::vector<EnvErr> err1, err0;
	for (auto n : map) {
		err0 = n.second->get_obj_err();
		if (n.second && n.second->get_obj_type() == ObjType::MyPlatform) {
			err1 = dynamic_cast<Platform*>(n.second)->get_status();
			
			err0.insert(err0.end(), err1.begin(), err1.end());
		}
		errs[n.second] = err0;
	}
	err1.clear();
	err0.clear();
	return errs;
}

void EnvDescriptor::get_instructions(std::map<Object*, std::vector<Instructions>> inst) {
	for (auto p : curinstructions) {
		p.second.clear();
	}
	curinstructions.clear();
	curinstructions = inst;
	std::map<Object*, std::vector<Instructions>>::iterator it = inst.begin();
	while (it != inst.end()) {
		change_object(it->first->get_coordinates(), it->second);
		it++;
	}
	for (auto p : inst) {
		p.second.clear();
	}
	inst.clear();
}

void EnvDescriptor::move_objs() {
	std::map<std::pair<int, int>, Object*> mymap;

	std::map<std::pair<int, int>, Object*>::iterator ito = map.begin();
	while (ito != map.end()) {
		if (ito->second && ito->second->get_obj_type() != ObjType::MyPlatform) {
			ito++; continue;
		}
		Platform* plat = dynamic_cast<Platform*>(ito->second);
		ito++;
		if (plat->get_type() != ComponentTypes::ManRobot && plat->get_type() != ComponentTypes::ScRobot) continue;
		if (plat->get_type() == ComponentTypes::ManRobot) {
			ManageRobot* mr = dynamic_cast<ManageRobot*>(plat);
			map.erase(mr->get_coordinates());
			mr->ask_move_permission();
			mr->move_platform();
			mymap[mr->get_coordinates()] = mr;
			bool u = plat->get_manager();
			if (u && (abs(mr->get_coordinates().first - mr->get_manager()->get_platform()->get_coordinates().first) > mr->get_manager()->get_radius() ||
				abs(mr->get_coordinates().second - mr->get_manager()->get_platform()->get_coordinates().second) > mr->get_manager()->get_radius())) {
				mr->new_manager(nullptr);
				mr->set_managed(false);
				//curerrors[mr].push_back(EnvErr::ManageProblem);
			}
		}

		if (plat->get_type() == ComponentTypes::ScRobot) {
			ScoutRobot* mr = dynamic_cast<ScoutRobot*>(plat);
			if (mr->get_coordinates().first == 4) {
				int y = 0;
			}
			map.erase(mr->get_coordinates());
			mr->ask_move_permission();
			mr->move_platform();
			//add_object(dynamic_cast<Object*> (mr));
			mymap[mr->get_coordinates()] = mr;
			bool u = plat->get_manager();
			if (u && (abs(mr->get_coordinates().first - mr->get_manager()->get_platform()->get_coordinates().first) > mr->get_manager()->get_radius() ||
				abs(mr->get_coordinates().second - mr->get_manager()->get_platform()->get_coordinates().second) > mr->get_manager()->get_radius())) {
				mr->new_manager(nullptr);
				mr->set_managed(false);
				//curerrors[mr].push_back(EnvErr::ManageProblem);
			}
		}
	}
	map.insert(mymap.begin(), mymap.end());
	
}
void EnvDescriptor::print_enviroment() {
	std::cout << "\t\tPLAN" << std::endl;
	std::map<std::pair<int, int>, Object*>::iterator ito = map.begin();
	int i = 0, j = 0;
	while (ito != map.end()) {
		while (ito->first.first != i || ito->first.second != j) {
			if (j < length - 1) {
				j++;
				std::cout << "No    ";
			}
			else if (i < width - 1) {
				i++; j = 0;
				std::cout << "No    " << std::endl;
			}
			else {
				std::cout << std::endl;
				return;
			}
		}

		if (ito->second && ito->second->get_obj_type() != ObjType::MyPlatform) {
			if (ito->second && ito->second->get_obj_type() == ObjType::Obstacle) {
				std::cout << "Obst  ";
			}
			else if (ito->second && ito->second->get_obj_type() == ObjType::SmthInteresting) {
				std::cout << "Smth  ";
			}
			ito++;
		}
		else {
			if (dynamic_cast<ManageCentre*>(ito->second) != nullptr)
				std::cout << "ManC  ";
			else if (dynamic_cast<ManageRobot*>(ito->second) != nullptr)
				std::cout << "ManR  ";
			else if (dynamic_cast<ScoutRobot*>(ito->second) != nullptr)
				std::cout << "ScRo  ";
			else if (dynamic_cast<ObserveCentre*>(ito->second) != nullptr)
				std::cout << "ObsC  ";
			ito++;
		}
		if (j <= width - 1) {
			j++;
		}
		else if (i < length - 1) {
			i++; j = 0;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

void EnvDescriptor::change_object(std::pair<int, int> coor, std::vector<Instructions> inst) {
	Platform* a = dynamic_cast<Platform*>(map[coor]);
	if (a->get_type() == ComponentTypes::ManRobot || a->get_type() == ComponentTypes::ScRobot) {
		MobilePlatform* b = dynamic_cast<MobilePlatform*>(a);
		b->get_commands(inst);
	}
	else {
		a->get_command(inst);
	}
}


Object::Object() {
	coordinates.first = 0;
	coordinates.second = 0;
	type = ObjType::MyPlatform;
}

Object::Object(std::pair<int, int> coords, ObjType t, EnvDescriptor* e) {
	coordinates = coords;
	type = t;
	env = e;
}

std::pair<int, int> Object::get_coordinates() {
	return coordinates;
}

void Object::set_coordinates(int x, int y) {
	coordinates = std::make_pair(x, y);
}


void Object::set_coordinates(std::pair<int, int> coords) {
	coordinates = coords;
}

ObjType Object::get_obj_type() {
	return this ? type : ObjType::Obstacle;
}

Platform::Platform(int mmnum, ComponentTypes t, int pr, std::string desc,
	std::pair<int, int> coords, EnvDescriptor* ed) : Object(coords, ObjType::MyPlatform, ed) {
	maxmodulesnum = mmnum;
	type = t;
	price = pr;
	description = desc;
}

Platform::~Platform() {
	for (auto m : modules) {
		if (static_cast<Manager*>(m)) {
			delete static_cast<Manager*>(m);
		}
		else if (static_cast<EnergyGenerator*>(m)) {
			delete static_cast<EnergyGenerator*>(m);
		}
		else if (static_cast<Sensor*>(m)) {
			delete static_cast<Sensor*>(m);
		}
		modules.clear();
	}
}

ComponentTypes Platform::get_type() {
	return this ? type: ComponentTypes::ManCentre;
}

int Platform::get_cur_energy_level() {
	int en=0;
	for (Module* m : modules) {
		en += m->get_out_energy_level();
		en -= m->get_in_energy_level();
	}
	return en;
}

bool Platform::add_module(Module* m) {
	if (modules.size() < maxmodulesnum) {
		Module* mm = new Module(*m);
		modules.push_back(m);
		neededenergy += m->get_in_energy_level();
		delete mm;
		return true;
	}
	return false;
}

void Platform::turn_module_on() {
	Module m1;
	Module& ptr = m1;
	for (Module* m : modules) {
		if (m->get_priority() < ptr.get_priority() && !m->get_working()) {
			ptr = *m;
		}
	}
	ptr.turn_on();
	neededenergy += ptr.get_in_energy_level();
}

void Platform::turn_module_off() {
	Module m1;
	Module& ptr = m1;
	for (Module* m : modules) {
		if (m->get_priority() > ptr.get_priority() && m->get_working()) {
			ptr = *m;
		}
	}
	ptr.turn_off();
	neededenergy -= ptr.get_in_energy_level();
}

void Platform::delete_module(Module* m) {
	std::vector<Module*>::iterator position = std::find(modules.begin(), modules.end(), m);
	if (position != modules.end())
		modules.erase(position);
}

void Platform::get_command(std::vector<Instructions> instructions) {
	if (!this) return;
	for (Instructions instruction : instructions) {
		if (instruction == Instructions::ModuleOn) {
			turn_module_on();
		}
		else if (instruction == Instructions::AddGenerator) {
			EnergyGenerator* gen = new EnergyGenerator(1, 10, 1, 1, *this);
			add_module(gen);
		}
		else if (instruction == Instructions::OnGenerator) {
			for (Module* mod : modules) {
				if (mod->get_type() == NodeTypes::GenNode && mod->get_working() == false)
					mod->turn_on();
			}
		}
		else if (instruction == Instructions::AddModule) {
			int countman = 0, countsens = 0;
			for (Module* mod : modules) {
				if (mod->get_type() == NodeTypes::ManNode)
					countman++;
				else if (mod->get_type() == NodeTypes::SenseNode)
					countsens++;
			}
			if (countman > countsens) {
				Sensor* sens = new Sensor(5, Direction::Left, Angles::A90, 1, 2, 1, 10, *this);
				add_module(sens);
			}
			else {
				add_module(new Manager(5, 2, 1, 10, *this, 15, 10));
			}
		}
		else if (instruction == Instructions::ReconnectNewMan) {
			int a = 0;
		}
		else if (instruction == Instructions::ReconnectNewPlat) {
			for (auto mod : modules) {
				if (mod->get_type() == NodeTypes::ManNode) {
					auto it = env->map.begin();
					while (it != env->map.end()) {

						if (dynamic_cast<Platform*>(it->second) && !dynamic_cast<Platform*>(it->second)->get_managed()) {
							dynamic_cast<Platform*>(it->second)->new_manager(static_cast<Manager*>(mod));
							static_cast<Manager*>(mod)->add_platform(dynamic_cast<Platform*>(it->second));
							//break;
						}
						it++;
					}
				}
			}
		}
	}
}


MobilePlatform::MobilePlatform(int mmnum, ComponentTypes t,
	int pr, std::string desc, int sval, Direction dir,
	std::pair<int, int> coords, EnvDescriptor* ed) :
	Platform(mmnum, t, pr, desc, coords, ed) {
	speedval = sval;
	speeddir = dir;
	curspeedval = sval;
}

void MobilePlatform::move_platform() {
	int xspeed = 0, yspeed = 0;
	if (speeddir == Direction::Down) {
		yspeed = -curspeedval;
	}
	else if (speeddir == Direction::Left) {
		xspeed = -curspeedval;
	}
	else if (speeddir == Direction::Right) {
		xspeed = curspeedval;
	}
	else {
		yspeed = curspeedval;
	}
	if (curspeedval != 0) {
		int a;
	}
	set_cur_speed(speedval);
	coordinates.first += xspeed;
	coordinates.second += yspeed;
	int a = 0;
	/*if (coordinates.first < 0) {
		coordinates.first = 0;
	}
	if (coordinates.second < 0) {
		coordinates.second = 0;
	}*/
}

void MobilePlatform::turn() {
	speeddir = (Direction)(((int)speeddir + 1) % 4);
}

std::vector<EnvErr> Platform::get_status() {
	curerrs.clear();
	if (!managed) curerrs.push_back(EnvErr::ManageProblem);
	int en = get_cur_energy_level();
	if (en < 0) curerrs.push_back(EnvErr::NegEnergyProblem);
	else if (en > 0) curerrs.push_back(EnvErr::PosEnergyProblem);
	return curerrs;
}


void MobilePlatform::ask_move_permission() {
	if (!managed) {
		curspeedval = 0;
		return;
	}
	std::pair<int, int> p(coordinates);
	if (p.second == 4) {
		int r = 0;
	}
	curspeedval = manager->check_move_n_obstacles(*this);

	if (speeddir == Direction::Down) p.second -= curspeedval;
	else if (speeddir == Direction::Left) p.first -= curspeedval;
	else if (speeddir == Direction::Right) p.first += curspeedval;
	else p.second += curspeedval;
	if (p.first < 0 || p.first >= this->env->get_size().first) {
		if (p.first < 0) p.first = 0;
		else p.first = this->env->get_size().first;
	}
	if (p.second < 0 || p.second >= this->env->get_size().second) { 
		if (p.second < 0) p.second = 0;
		else p.second = this->env->get_size().second;
	}

	if (curspeedval != speedval)
		curerrs.push_back(EnvErr::MoveProblem);;
}

void MobilePlatform::get_commands(std::vector<Instructions> instructions) {
	for (Instructions instruction : instructions) {
		if (instruction == Instructions::TurnVector) {
			turn();
		}
	}
	get_command(instructions);
}


ScoutRobot::ScoutRobot(int mmnum, int pr, std::string desc, int sval,
	Direction dir, std::pair<int, int> coords, EnvDescriptor* ed) :
	MobilePlatform(mmnum, ComponentTypes::ScRobot, pr, desc, sval, dir, coords, ed),
	Platform(mmnum, ComponentTypes::ScRobot, pr, desc, coords, ed) {}


ObserveCentre::ObserveCentre(int mmnum, int pr, std::string desc, std::pair<int, int> coords, EnvDescriptor* ed) :
	Platform(mmnum, ComponentTypes::ObsCentre, pr, desc, coords, ed) {}


ManageRobot::ManageRobot(int mmnum, int pr, std::string desc, int sval,
	Direction dir, std::pair<int, int> coords, EnvDescriptor* ed) :
	MobilePlatform(mmnum, ComponentTypes::ManRobot, pr, desc, sval, dir, coords, ed),
	ManagePlatform(mmnum, ComponentTypes::ManRobot, pr, desc, coords, ed),
	Platform(mmnum, ComponentTypes::ManRobot, pr, desc, coords, ed) {}
