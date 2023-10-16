#include "module.h"


Module::Module(int enlev, int pri, bool enab, int cost, Platform& p, NodeTypes ntype) : plat(&p) {
	energylevel = enlev;
	priority = pri;
	enable = enab;
	price = cost;
	type = ntype;
}

Module::Module() {
	energylevel = 0;
	priority = 1;
	enable = 0;
	price = 1;
	Platform* ps = nullptr;
	plat = ps;
}

NodeTypes Module::get_type() {
	return type;
}

int Module::get_in_energy_level() {
	if (type != NodeTypes::GenNode)
		return energylevel;
	return 0;
}

int Module::get_out_energy_level() {
	if (type == NodeTypes::GenNode)
		return energylevel;
	return 0;
}

Sensor::Sensor(int rad, Direction vect, Angles ang) {
	radius = rad;
	vector = vect;
	angle = ang;
}

Sensor::Sensor(int rad, Direction vect, Angles ang, int enlev, int pri, bool enab, int cost, Platform& p) :
	Module(enlev, pri, enab, cost, p, NodeTypes::SenseNode) {
	radius = rad;
	vector = vect;
	angle = ang;
}

EnergyGenerator::EnergyGenerator(int outen) {
	outenergylevel = outen;
	energylevel = outen;
}

EnergyGenerator::EnergyGenerator(int outen, int pri, bool enab, int cost, Platform& p) :
	Module(outen, pri, enab, cost, p, NodeTypes::GenNode) {
	outenergylevel = outen;
}

int EnergyGenerator::get_energy_level() {
	return outenergylevel;
}

std::vector<std::pair<int, int>> find_triangle(int radius, Direction vector, std::pair<int, int> coords) {
	std::vector<std::pair<int, int>> vect;
	for (int i = 0; i < radius; i++) {
		for (int j = 0; j < radius; j++) {
			vect.push_back(std::pair<int, int>(coords.first + i, coords.second + j));
		}
	}
	for (std::pair<int, int>& v : vect) {
		if (vector == Direction::Down) {
			v.second = 2 * coords.second - v.second;
			v.first = 2 * coords.first - v.first;
		}
		else if (vector == Direction::Left) {
			v.first = 2 * coords.first - v.first;
		}
		else if (vector == Direction::Right) {
			v.second = 2 * coords.second - v.second;
		}
	}
	return vect;
}

std::map<std::pair<int, int>, Object*> Sensor::check_enviroment() {
	std::map< std::pair<int, int>, Object*> map;
	Object* objptr;
	std::vector<std::pair<int, int>> coords, newtri;
	Direction vect = vector;
	for (int i = 0; i < (int)angle && i < 4; i++) {
		newtri = find_triangle(radius, vect, plat->get_coordinates());
		coords.insert(coords.end(), newtri.begin(), newtri.end());
		vect = (Direction)(((int)vect + 1) % 4);
	}
	/*if (angle < 0 || angle > 4) {
		int b = 0;
	}*/
	for (std::pair<int, int>p : coords) {
		objptr = plat->ret_env()->check_coordinates(p);
		if (objptr) map[p] = objptr;
	}
	return map;
}


Manager::Manager(int enlev, int pri, bool enab, int cost, Platform& p, int maxn, int r) :
	Module(enlev, pri, enab, cost, p, NodeTypes::ManNode) {
	maxnum = maxn;
	rad = r;
}

std::map<std::pair<int, int>, Object*> Manager::give_info_ai(Sensor& sens) {
	return sens.check_enviroment();
}

std::map<Object*, std::vector<EnvErr>> Manager::get_erors(Platform& platf) {
	std::map < Object*, std::vector<EnvErr>> m;
	m[&platf] = platf.get_status();
	return m;
}

void Manager::turn_command(MobilePlatform& mobplat) {
	mobplat.turn();
}

int Manager::check_move_n_obstacles(MobilePlatform& mobplat) {
	if (this == nullptr) return 0;
	std::pair<int, int> p(mobplat.get_coordinates());
	Object* o;
	int i;
	for (i = 1; i <= mobplat.get_speed_val(); i++) {
		Direction dir = mobplat.get_dir();
		if (dir == Direction::Down) p.second--;
		else if (dir == Direction::Left) p.first--;
		else if (dir == Direction::Right) p.first++;
		else p.second++;
		if (plat->ret_env()->check_coordinates(p) || p.first > plat->ret_env()->get_size().first - 1 ||
			p.first < 0 || p.second < 0 || p.second > plat->ret_env()->get_size().second - 1) {
			break;
		}
	}
	return i - 1;
}

void Manager::add_platform(Platform* plat) {
	manobjs.add(plat);
}

int Manager::check_energy(Platform& platf) {
	return platf.get_cur_energy_level();
}

void ComponentTable::add(Platform* platf) {
	component[platf->get_coordinates()] = platf;
}

void ComponentTable::deletep(std::pair<int, int> coords) {
	component.erase(coords);
}

Platform* ComponentTable::get(std::pair<int, int> coords) {
	return component[coords];
}

std::map<std::pair<int, int>, Object*> Manager::give_all_info_ai() {
	std::map<std::pair<int, int>, Object*> newmap, ptrmap;
	auto it = manobjs.component.begin(); //std::map<std::pair<int, int>, Platform*>::iterator
	while (it != manobjs.component.end()) {
		for (Module* mod : it.cur->data->get_modules()) {
			if (mod->get_type() == NodeTypes::SenseNode) {
				ptrmap = give_info_ai(*static_cast<Sensor*>(mod));
				newmap.insert(ptrmap.begin(), ptrmap.end());
			}
		}
		it++;
	}
	return newmap;
}

Platform* Module::get_platform() { return plat; }

void Manager::delete_platform(Platform* plat) {
	manobjs.deletep(plat->get_coordinates());
}