#include "pch.h"
#include "../../lab4libnew/lab4libnew/app.h"
#include "../../lab4maplib/lab4maplib/mymap.h"


TEST(Application, Constructors) {
	Application app;
	auto coords = std::pair<int, int>(10, 10);
	EXPECT_EQ(app.enviroment->get_size(), coords);
	app = Application(5, 5, 10);
	EXPECT_NEAR(app.enviroment->map.size(), 10, 2);
	int a = 0;
}

TEST(Application, MainFuncs) {
	Application app(2, 2, 100);
	app.change_enviroment(std::map<Object*, std::vector<Instructions>>());
	auto errs = app.get_env_status();
	EXPECT_EQ(errs.size(), 4);
	app.kick_ai(errs, 2, 2);
	auto inst = app.check_ai_status();
	EXPECT_NE(inst.size(), 0);
	int a = 0;
	EXPECT_NO_THROW(app.work());
}

TEST(MyMap, Indexation) {
	MyMap<int, int> map;
	map[3] = 3;
	map[4] = 4;
	map[5] = 5;
	map[-8] = -8;
	EXPECT_EQ(map[-8], -8);
	EXPECT_EQ(map[3], 3);
	map[5] = 55;
	EXPECT_EQ(map[5], 55);
}

TEST(MyMap, InitConstructors) {
	MyMap<int, int> mapint;
	mapint[0] = 0;
	mapint[1] = 1;
	mapint[2] = 2;
	EXPECT_EQ(mapint[2], 2);
	MyMap<std::pair<int, int>, std::pair<int, int>> mappair;
	mappair[std::make_pair(0, 0)] = std::make_pair(0, 0);
	mappair[std::make_pair(1, 1)] = std::make_pair(1, 1);
	mappair[std::make_pair(2, 2)] = std::make_pair(2, 2);
	EXPECT_EQ(mappair[std::make_pair(1, 1)], std::make_pair(1, 1));
	MyMap<float, float> mapfloat;
	mapfloat[0.5] = 0.5;
	mapfloat[1.5] = 1.5;
	mapfloat[2.5] = 2.5;
	EXPECT_EQ(mapfloat[0.5], 0.5);
	MyMap<Object, Object> mapobj;
	std::pair<int, int> p(10, 10);
	Object obj(p, ObjType::Obstacle, nullptr);
	mapobj[Object(p, ObjType::Obstacle, nullptr)] = obj;
	p = std::make_pair(5, 5);
	obj = Object(p, ObjType::Obstacle, nullptr);
	mapobj[Object(p, ObjType::Obstacle, nullptr)] = obj;
	EXPECT_EQ(mapobj[obj], obj);
	p = std::make_pair(10, 10);
	MyMap<Object*, Object*> mapobjptr;
	Object* obj1 = new Object(p, ObjType::Obstacle, nullptr);
	mapobjptr[&obj] = &obj;
	/*mapobjptr[obj1] = obj1;
	EXPECT_EQ(mapobjptr[&obj], &obj);
	EXPECT_EQ(mapobjptr[obj1], obj1);*/
}

TEST(MyMap, OtherConstructors) {
	MyMap<int, int> mapint;
	mapint[0] = 0;
	mapint[1] = 1;
	mapint[2] = 2;
	MyMap<int, int> map1(mapint);
	EXPECT_EQ(map1[1], 1);
	MyMap<int, int> map2(map1.begin(), map1.end());
	EXPECT_EQ(map2[2], 2);
}

TEST(MyMap, Insertion) {
	MyMap<int, int> map, map1, map2;
	map.insert(std::make_pair(0, 0));
	EXPECT_EQ(map[0], 0);
	map1.insert(map);
	EXPECT_EQ(map[0], 0);
	map2.insert(map1.begin(), map1.end());
	EXPECT_EQ(map[0], 0);
}

TEST(MyMap, Iterator) {
	MyMap<int, int> map;
	map[3] = 3;
	map[4] = 4;
	map[5] = 5;
	map[-8] = -8;
	int arr[] = { -8, 3, 4, 5 };
	MyMap<int, int>::iterator it = map.begin();
	EXPECT_EQ(it.cur->key, -8);
	it++; ++it;
	EXPECT_EQ(it.cur->key, 4);
	it--; --it;
	EXPECT_EQ(it.cur->key, -8);
	int i = 0;
	for (it = map.begin(); it != map.end(); it++) {
		EXPECT_EQ(it.cur->key, arr[i]);
		i++;
	}
	i = 0;
	for (auto a : map) {
		EXPECT_EQ(a->key, arr[i]);
		i++;
	}
}

TEST(MyMap, Emplace) {
	MyMap<int, int> map;
	map[3] = 3;
	map[4] = 4;
	map[5] = 5;
	map[-8] = -8;
	auto a = map.emplace(std::make_pair(-8, -88));
	EXPECT_EQ(a.second, false);
	EXPECT_EQ(a.first.cur->key, -8);
	a = map.emplace(std::make_pair(8, 8));
	EXPECT_EQ(a.second, true);
	EXPECT_EQ(a.first.cur->key, 8);
	a = map.emplace(5, 55);
	EXPECT_EQ(a.second, false);
	EXPECT_EQ(a.first.cur->key, 5);
	a = map.emplace(-5, -55);
	EXPECT_EQ(a.second, true);
	EXPECT_EQ(a.first.cur->key, -5);
}

TEST(MyMap, Find) {
	MyMap<int, int> map;
	map[3] = 3;
	map[4] = 4;
	map[5] = 5;
	map[-8] = -8;
	auto m = map.find(4);
	EXPECT_EQ(m.cur->key, 4);
}

TEST(MyMap, Erase) {
	MyMap<int, int> map;
	map[3] = 3;
	map[4] = 4;
	map[5] = 5;
	map[-8] = -8;
	auto m = map.erase(3);
	EXPECT_EQ(m.cur->key, 4);
	m = map.begin();
	m = map.erase(m);
	EXPECT_EQ(m.cur->key, 3);
	m = map.erase(map.begin(), map.end());
	EXPECT_EQ(m.cur, nullptr);
}

TEST(EnvDescriptor, ConstructorsnOther) {
	EnvDescriptor env; 
	std::pair<int, int> p(10, 10);
	std::map<Object*, std::vector<EnvErr>> pp;
	EXPECT_EQ(env.get_size(), p);
	EXPECT_EQ(env.get_errors(), pp);
	env.change_env_size(15, 15);
	p = std::pair<int, int>(15, 15);
	EXPECT_EQ(env.get_size(), p);
	p = std::pair<int, int>(0, 0);
	Object* obj = new Object(p, ObjType::Obstacle, &env);
	env.add_object(obj);
	EXPECT_EQ(env.check_coordinates(p), obj);
	env.delete_object(p);
	EXPECT_EQ(env.check_coordinates(p), nullptr);
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(EnvDescriptor, MoveProblem) {
	EnvDescriptor env;
	ScoutRobot sr(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 2), &env);
	Object* obj = new Object(std::pair<int, int>(0, 1), ObjType::Obstacle, &env);
	Manager* man = new Manager(1, 1, 1, 1, sr, 2, 5);
	sr.new_manager(man);
	man->add_platform(&sr);
	std::map<Object*, std::vector<Instructions>> m;
	env.add_object(&sr);
	env.add_object(obj);
	env.get_instructions(m);
	env.move_objs();
	std::map<Object*, std::vector<EnvErr>> a = env.get_errors();
	EXPECT_EQ(a[&sr][0], EnvErr::MoveProblem);
}

TEST(EnvDescriptor, ManageProblem) {
	EnvDescriptor env;
	ScoutRobot sr(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 2), nullptr);
	std::map<Object*, std::vector<Instructions>> m;
	env.add_object(&sr);
	env.get_instructions(m);
	env.move_objs();
	std::map<Object*, std::vector<EnvErr>> a = env.get_errors();
	EXPECT_EQ(a[&sr][0], EnvErr::ManageProblem);
}

TEST(EnvDescriptor, NegEnProblem) {
	EnvDescriptor env;
	ScoutRobot sr(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 2), nullptr);
	std::map<Object*, std::vector<Instructions>> m;
	Manager* man = new Manager(1, 1, 1, 1, sr, 2, 5);
	sr.add_module(man);
	sr.new_manager(man);
	env.add_object(&sr);
	std::map<Object*, std::vector<EnvErr>> a = env.get_errors();
	EXPECT_EQ(a[&sr][0], EnvErr::NegEnergyProblem);
}

TEST(EnvDescriptor, PosEnProblem) {
	EnvDescriptor env;
	ScoutRobot sr(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 2), nullptr);
	std::map<Object*, std::vector<Instructions>> m;
	EnergyGenerator* man = new EnergyGenerator(2, 1, 1, 1, sr);
	Manager* manager = new Manager(1, 1, 1, 1, sr, 1, 1);
	sr.add_module(man);
	sr.add_module(manager);
	sr.new_manager(manager);
	env.add_object(&sr);
	std::map<Object*, std::vector<EnvErr>> a = env.get_errors();
	EXPECT_EQ(a[&sr][0], EnvErr::PosEnergyProblem);
}

TEST(ComponentTable, Functions) {
	ComponentTable ct;
	Platform* p = new Platform(1, ComponentTypes::ManCentre, 1, "", std::pair<int, int>(0, 0), nullptr);
	ct.add(p);
	EXPECT_EQ(ct.get(std::pair<int, int>(0, 0)), p);
	ct.deletep(p->get_coordinates());
	EXPECT_EQ(ct.component.size(), 0);
}


TEST(Manager, Functions) {
	Platform* p = new Platform(1, ComponentTypes::ManCentre, 1, "", std::pair<int, int>(0, 0), nullptr);
	Manager module(1, 1, 1, 1, *p, 5, 5);
	EXPECT_EQ(module.get_in_energy_level(), 1);
	EXPECT_EQ(module.get_out_energy_level(), 0);
	EXPECT_EQ(module.get_platform(), p);
	EXPECT_EQ(module.get_priority(), 1);
	EXPECT_EQ(module.get_radius(), 5);
	EXPECT_EQ(module.get_type(), NodeTypes::ManNode);
	EXPECT_EQ(module.get_working(), 1);
	module.turn_off();
	EXPECT_EQ(module.get_working(), 0);
	module.turn_on();
	EXPECT_EQ(module.get_working(), 1);
}

TEST(EnergyGenerator, Functions) {
	Platform* p = new Platform(1, ComponentTypes::ManCentre, 1, "", std::pair<int, int>(0, 0), nullptr);
	EnergyGenerator module(1, 1, 1, 1, *p);
	EXPECT_EQ(module.get_in_energy_level(), 0);
	EXPECT_EQ(module.get_out_energy_level(), 1);
	EXPECT_EQ(module.get_platform(), p);
	EXPECT_EQ(module.get_priority(), 1);
	EXPECT_EQ(module.get_type(), NodeTypes::GenNode);
	EXPECT_EQ(module.get_working(), 1);
	module.turn_off();
	EXPECT_EQ(module.get_working(), 0);
	module.turn_on();
	EXPECT_EQ(module.get_working(), 1);
}

TEST(Sensor, Functions) {
	Platform* p = new Platform(1, ComponentTypes::ManCentre, 1, "", std::pair<int, int>(0, 0), nullptr);
	Sensor module(5, Direction::Down, Angles::A270, 1, 1, 1, 1, *p);
	EXPECT_EQ(module.get_in_energy_level(), 1);
	EXPECT_EQ(module.get_out_energy_level(), 0);
	EXPECT_EQ(module.get_platform(), p);
	EXPECT_EQ(module.get_priority(), 1);
	EXPECT_EQ(module.get_type(), NodeTypes::SenseNode);
	EXPECT_EQ(module.get_working(), 1);
	module.turn_off();
	EXPECT_EQ(module.get_working(), 0);
	module.turn_on();
	EXPECT_EQ(module.get_working(), 1);
}

TEST(Application, Constructor) {
	Application app(10, 10, 10);
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(ScoutRobot, Constructor) {
	std::pair<int, int> a(0, 0);
	ScoutRobot sr(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0,0), nullptr);
	EXPECT_EQ(sr.get_coordinates(), a);
	EXPECT_EQ(sr.get_cur_energy_level(), 0);
	EXPECT_EQ(sr.get_dir(), Direction::Down);
	EXPECT_EQ(sr.get_managed(), false);
	EXPECT_EQ(sr.get_manager(), nullptr);
	EXPECT_EQ(sr.get_max_modules_num(), 1);
	EXPECT_EQ(sr.get_modules(), std::vector<Module*>());
	EXPECT_EQ(sr.get_type(), ComponentTypes::ScRobot);
	EXPECT_EQ(sr.get_obj_type(), ObjType::MyPlatform);
	EXPECT_EQ(sr.get_speed_val(), 1);
	EXPECT_EQ(sr.ret_env(), nullptr);
	EXPECT_TRUE(true);
}

TEST(ManageRobot, Constructor) {
	std::pair<int, int> a(0, 0);
	ManageRobot* sr = new ManageRobot(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), nullptr);
	EXPECT_EQ(sr->get_coordinates(), a);
	EXPECT_EQ(sr->get_cur_energy_level(), 0);
	EXPECT_EQ(sr->get_dir(), Direction::Down);
	EXPECT_EQ(sr->get_managed(), false);
	EXPECT_EQ(sr->get_manager(), nullptr);
	EXPECT_EQ(sr->get_max_modules_num(), 1);
	EXPECT_EQ(sr->get_modules(), std::vector<Module*>());
	EXPECT_EQ(sr->get_type(), ComponentTypes::ManRobot);
	EXPECT_EQ(sr->get_obj_type(), ObjType::MyPlatform);
	EXPECT_EQ(sr->get_speed_val(), 1);
	EXPECT_EQ(sr->ret_env(), nullptr);
	ManageRobot sr1(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), nullptr);
	delete sr;
}


TEST(ManageCentre, Constructor) {
	std::pair<int, int> a(0, 0);
	ManageCentre sr(1, 1, "", std::pair<int,int>(0,0), nullptr);
	EXPECT_EQ(sr.get_coordinates(), a);
	EXPECT_EQ(sr.get_cur_energy_level(), 0);
	EXPECT_EQ(sr.get_managed(), false);
	EXPECT_EQ(sr.get_manager(), nullptr);
	EXPECT_EQ(sr.get_max_modules_num(), 1);
	EXPECT_EQ(sr.get_modules(), std::vector<Module*>());
	EXPECT_EQ(sr.get_type(), ComponentTypes::ManCentre);
	EXPECT_EQ(sr.get_obj_type(), ObjType::MyPlatform);
	EXPECT_EQ(sr.ret_env(), nullptr);
	EXPECT_TRUE(true);
}

TEST(ObserveCentre, Constructor) {
	std::pair<int, int> a(0, 0);
	ObserveCentre sr(1, 1, "1", std::pair<int, int>(0, 0), nullptr);
	EXPECT_EQ(sr.get_coordinates(), a);
	EXPECT_EQ(sr.get_cur_energy_level(), 0);
	EXPECT_EQ(sr.get_managed(), false);
	EXPECT_EQ(sr.get_manager(), nullptr);
	EXPECT_EQ(sr.get_max_modules_num(), 1);
	EXPECT_EQ(sr.get_modules(), std::vector<Module*>());
	EXPECT_EQ(sr.get_type(), ComponentTypes::ObsCentre);
	EXPECT_EQ(sr.get_obj_type(), ObjType::MyPlatform);
	EXPECT_EQ(sr.ret_env(), nullptr);
	EXPECT_TRUE(true);
}

TEST(MobilePlatform, MoveTest) {
	MobilePlatform* mp = new ScoutRobot(1, 1, "1", 1, Direction::Down, std::pair<int, int>(2, 2), nullptr);
	mp->move_platform();
	std::pair<int, int> a(2, 1);
	EXPECT_EQ(mp->get_coordinates(), a);
	mp->turn();
	mp->move_platform();
	a.first--;
	EXPECT_EQ(mp->get_coordinates(), a);
	mp->turn();
	mp->move_platform();
	a.second++;
	EXPECT_EQ(mp->get_coordinates(), a);
	mp->turn();
	mp->move_platform();
	a.first++;
	EXPECT_EQ(mp->get_coordinates(), a);
}

TEST(AI, TurnAndGenOnInst) {
	EnvDescriptor env = EnvDescriptor(2, 2);
	AI ai;
	ManageRobot* sr = new ManageRobot( 1 , 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), &env);
	Manager module(1, 1, 1, 1, *sr, 5, 5);
	sr->add_module(&module);
	sr->new_manager(&module);
	env.add_object(sr);
	ai.add_manager(&module);
	env.get_instructions(std::map<Object*, std::vector<Instructions>>());
	env.move_objs();
	ai.get_errors(env.get_errors(), 2, 2);
	auto inst = ai.give_instructions();
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::TurnVector) != inst[sr].end(), true);
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::OnGenerator) != inst[sr].end(), true);
}

TEST(AI, NewManInst) {
	EnvDescriptor env = EnvDescriptor(2, 2);
	AI ai;
	ManageRobot* sr = new ManageRobot(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), &env);
	env.add_object(sr);
	env.get_instructions(std::map<Object*, std::vector<Instructions>>());
	env.move_objs();
	ai.get_errors(env.get_errors(), 2, 2);
	auto inst = ai.give_instructions();
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::ReconnectNewMan) != inst[sr].end(), true);
}

TEST(AI, ModuleOnInst) {
	EnvDescriptor env = EnvDescriptor(2, 2);
	AI ai;
	ManageRobot* sr = new ManageRobot(1, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), &env);
	EnergyGenerator eg = EnergyGenerator(2, 1, 1, 1, *sr);
	sr->add_module(&eg);
	env.add_object(sr);
	env.get_instructions(std::map<Object*, std::vector<Instructions>>());
	env.move_objs();
	ai.get_errors(env.get_errors(), 2, 2);
	auto inst = ai.give_instructions();
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::ReconnectNewMan) != inst[sr].end(), true);
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::ModuleOn) != inst[sr].end(), true);
}

TEST(AI, ModulAddnInst) {
	EnvDescriptor env = EnvDescriptor(2, 2);
	AI ai;
	ManageRobot* sr = new ManageRobot(2, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), &env);
	EnergyGenerator eg(2, 1, 1, 1, *sr);
	sr->add_module(&eg);
	env.add_object(sr);
	env.get_instructions(std::map<Object*, std::vector<Instructions>>());
	env.move_objs();
	ai.get_errors(env.get_errors(), 2, 2);
	auto inst = ai.give_instructions();
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::ReconnectNewMan) != inst[sr].end(), true);
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::AddModule) != inst[sr].end(), true);
}

TEST(AI, TurnAndGenAddInst) {
	EnvDescriptor env = EnvDescriptor(2, 2);
	AI ai;
	ManageRobot* sr = new ManageRobot(2, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), &env);
	Manager module(1, 1, 1, 1, *sr, 5, 5);
	sr->add_module(&module);
	sr->new_manager(&module);
	env.add_object(sr);
	ai.add_manager(&module);
	env.get_instructions(std::map<Object*, std::vector<Instructions>>());
	env.move_objs();
	ai.get_errors(env.get_errors(), 2, 2);
	auto inst = ai.give_instructions();
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::TurnVector) != inst[sr].end(), true);
	EXPECT_EQ(std::find(inst[sr].begin(), inst[sr].end(), Instructions::AddGenerator) != inst[sr].end(), true);
}

TEST(AI, GetInfo) {
	EnvDescriptor env = EnvDescriptor(2, 2);
	AI ai;
	ManageRobot* sr = new ManageRobot(3, 1, "1", 1, Direction::Down, std::pair<int, int>(0, 0), &env);
	Object* obj = new Object(std::pair<int, int>(1, 1), ObjType::SmthInteresting, &env);
	Manager module(1, 1, 1, 1, *sr, 5, 5);
	Sensor sens(2, Direction::Right, Angles::A360, 2, 1, 1, 1, *sr);
	EnergyGenerator gen(14, 1, 1, 1, *sr);
	sr->add_module(&module);
	sr->add_module(&sens);
	sr->add_module(&gen);
	sr->new_manager(&module);
	module.add_platform(sr);
	env.add_object(sr);
	env.add_object(obj);
	ai.add_manager(&module);
	env.get_instructions(std::map<Object*, std::vector<Instructions>>());
	env.move_objs();
	ai.get_errors(env.get_errors(), 2, 2);
	auto inst = ai.give_instructions();
	ai.get_all_info();
	EXPECT_EQ(ai.get_int_num(), 1);
	ai.set_int_num(2);
	EXPECT_EQ(ai.get_int_num(), 2);
	EXPECT_EQ(ai.get_timer(), 1);
}





int _tmain(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	return 0;
}
