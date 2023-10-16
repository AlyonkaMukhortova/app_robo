#include "ai.h"
#include <algorithm>
#include <iterator>

AI::~AI() {
	modules.clear();
	for (auto m : curerr) {
		m.second.clear();
	}
	curerr.clear();
	for (auto m : currinst) {
		m.second.clear();
	}
	currinst.clear();
	plan.clear();
	
}

ComponentTable& AI::get_info(Manager& man) {
	return man.get_table();
}

void AI::get_all_info() {
	for (Manager* man : modules) {
		ComponentTable& tab = get_info(*man);
		from_info_to_plan(tab);
		std::map<std::pair<int, int>, Object*> pp = man->give_all_info_ai();
		plan.insert(pp.begin(), pp.end());
	}
	numinterests = 0;
	for (auto p : plan) {
		if (p.second && p.second->get_obj_type() == ObjType::SmthInteresting) {
			numinterests++;
		}
	}
}

void AI::from_info_to_plan(ComponentTable& tab) {
	auto it = tab.component.begin(); //std::map<std::pair<int, int>, Platform*>::iterator
	while (it != tab.component.end()) {
		plan[it.cur->key] = it.cur->data;
		it++;
	}

}

void AI::print_enviroment() {
	std::cout << "\t\tAI PLAN" << std::endl;
	std::map<std::pair<int, int>, Object*>::iterator ito = plan.begin();
	int i = 0, j = 0;
	while (ito != plan.end()) {
		while (ito->first.first != i || ito->first.second != j) {
			if (j <= envsize.first - 1) {
				j++;
				std::cout << "No    ";
			}
			else if (i < envsize.second - 1) {
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
		if (j <= envsize.first - 1) {
			j++;
		}
		else if (i < envsize.second - 1) {
			i++; j = 0;
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}


void AI::get_errors(std::map<Object*, std::vector<EnvErr>> errs, int a, int b) {
	for (auto p : curerr) {
		p.second.clear();
	}
	curerr.clear();
	curerr = errs;
	envsize = std::make_pair(a, b);
}

std::map<Object*, std::vector<Instructions>> AI::give_instructions() {
	std::map<Object*, std::vector<EnvErr>>::iterator it = curerr.begin();
	for (auto p : currinst) {
		p.second.clear();
	}
	currinst.clear();
	/*auto planit = plan.begin();
	bool more = 1;
	int ind = 0, half = plan.size();
	if (timer >= 10) {

		half /= 2;
		more = planit->first.first > envsize.first / 2;
		for (int i = 0; i < half; i++) {
			if (more) {
				planit++; ind++;
			}
			else {
				planit--; ind--;
			}
		}
	}*/
	while (it != curerr.end()) {
		for (EnvErr err : it->second) {
			if (err == EnvErr::ManageProblem) {
				for (Manager* man : modules) {
					if (!man->get_full_platforms() &&
						(std::find(currinst[man->get_platform()].begin(), currinst[man->get_platform()].end(), Instructions::ReconnectNewPlat) == currinst[man->get_platform()].end())) {
						currinst[man->get_platform()].emplace_back(Instructions::ReconnectNewPlat);
					}
				}
				if (std::find(currinst[it->first].begin(), currinst[it->first].end(), Instructions::ReconnectNewMan) == currinst[it->first].end())
				currinst[it->first].emplace_back(Instructions::ReconnectNewMan);
			}
			else if (err == EnvErr::MoveProblem && (std::find(currinst[it->first].begin(), currinst[it->first].end(), Instructions::TurnVector) == currinst[it->first].end())) {
				currinst[it->first].emplace_back(Instructions::TurnVector);
			}
			else if (err == EnvErr::NegEnergyProblem) {
				if (dynamic_cast<Platform*>(it->first)->get_modules().size() < dynamic_cast<Platform*>(it->first)->get_max_modules_num() &&
					(std::find(currinst[it->first].begin(), currinst[it->first].end(), Instructions::AddGenerator) == currinst[it->first].end()))
					currinst[it->first].emplace_back(Instructions::AddGenerator);
				else if (std::find(currinst[it->first].begin(), currinst[it->first].end(), Instructions::OnGenerator) == currinst[it->first].end())
					currinst[it->first].emplace_back(Instructions::OnGenerator);
			}
			else if (err == EnvErr::PosEnergyProblem) {
				if (dynamic_cast<Platform*>(it->first)->get_modules().size() < dynamic_cast<Platform*>(it->first)->get_max_modules_num() &&
					(std::find(currinst[it->first].begin(), currinst[it->first].end(), Instructions::AddModule) == currinst[it->first].end()))
					currinst[it->first].push_back(Instructions::AddModule);
				else if (std::find(currinst[it->first].begin(), currinst[it->first].end(), Instructions::ModuleOn) == currinst[it->first].end())
					currinst[it->first].emplace_back(Instructions::ModuleOn);
			}
		}
		it++;
	}
	timer++;
	return currinst;
}
