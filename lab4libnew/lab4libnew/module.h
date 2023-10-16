#pragma once
#include "objects.h"
#include "enums.h"
#include <map>
#include <vector>
#include "../../lab4maplib/lab4maplib/mymap.h"

class Platform;
class MobilePlatform;
class Object;

class ComponentTable
{
public:
	ComponentTable() {};
	/**
	 * Add platform to table.
	 *
	 * \param platf Platform to add
	 */
	void add(Platform*);
	/**
	 * Delete platform from table.
	 *
	 * \param coords Coords of platform to delete
	 */
	void deletep(std::pair<int, int>);
	void save_to_file(FILE*);
	/**
	 * Get platform by its coordinates.
	 *
	 * \param coords pair<x,y>
	 * \return platform pointer
	 */
	Platform* get(std::pair<int, int>);
	//std::map<std::pair<int, int>, Platform*> component;
	MyMap< std::pair<int, int>, Platform*> component;
};

//full
class Module {
protected:
	int energylevel;
	int priority;
	bool enable;
	Platform* plat;
	NodeTypes type;
public:
	int price;
	Module();
	~Module() {}
	Module(int enlev, int pri, bool enab, int cost, Platform&, NodeTypes);			//tested
	void turn_on() { enable = true; }												//tested
	void turn_off() { enable = false; }												//tested
	/**
	 * Get energy usage.
	 *
	 * \return in energy
	 */
	int get_in_energy_level();														//tested
	/**
	* Get energy given by module.
	*
	* \return out energy
	*/
	int get_out_energy_level();														//tested
	/**
	 * 	 * Get type of Module: Generator, Sensor or Manager.
	 *
	 * \return module type
	 */
	NodeTypes get_type();												//tested
	int get_priority() { return priority; }								//tested
	bool get_working() { return enable; }								//tested
	/**
	 * Get platform where module is located.
	 *
	 * \return Platform
	 */
	Platform* get_platform();
};

/**
 * Class Sensor. Inherited from Module.
 */
class Sensor :public Module {
private:
	int radius;
	Direction vector;
	Angles angle;
public:
	~Sensor() {}
	Sensor(int, Direction, Angles);																		//written
	Sensor(int rad, Direction vect, Angles ang, int enlev, int pri, bool enab, int cost, Platform& p);	//tested
	/**
	 * Find zone of view of sensor. Check if there's smth on each of coords of that zone
	 *
	 * \return all objects Sensor can see
	 */
	std::map<std::pair<int, int>, Object*> check_enviroment();											//tested
	int get_radius() { return radius; }
	Direction get_vector() { return vector; }
	Angles get_angle() { return angle; }
};

/**
 * Class Manager. Inherited from Module.
 */
class Manager :public Module {
private:
	ComponentTable manobjs;
	int maxnum;
	//std::map<Object*, std::vector<EnvErr>> curerr;
	int rad;
public:
	~Manager() {}
	Manager(int enlev, int pri, bool enab, int cost, Platform&, int maxn, int r);
	/**
	 * Check each point of trajectory of moving for any obstacles.
	 *
	 * \param mobplat Platform to move
	 * \return max num of points to move
	 */
	int check_move_n_obstacles(MobilePlatform&);							//tested
	/**
	 * Get Component Table of Module.
	 * \return component table
	 */
	ComponentTable& get_table() { 
		return manobjs;
	}							//tested
	/**
	 * Turns mobile platform.
	 *
	 * \param mobplat Platform to turn
	 */
	void turn_command(MobilePlatform&);										//tested
	/**
	 * Get errors from 1 platform.
	 *
	 * \param platf Platform to fine errors
	 * \return all errors of platform
	 */
	std::map<Object*, std::vector<EnvErr>> get_erors(Platform&);			//tested
	/**
	 * Give all objects can be seen by senson to ai.
	 *
	 * \param sens Sensor to find
	 * \return  all objects can be seen with their coords
	 */
	std::map<std::pair<int, int>, Object*> give_info_ai(Sensor&);			//tested
	/**
	 * Check energy level of platform.
	 *
	 * \param platf Platform to check energy
	 * \return Energy level of platform
	 */
	int check_energy(Platform&);											//written, useless
	/**
	 * Give info from component table and all sensors from platform are managed by manager.
	 *
	 * \return map of objects and thir coords
	 */
	std::map<std::pair<int, int>, Object*> give_all_info_ai();				//tested
	/**
	 * Add new platform to manage.
	 *
	 * \param plat Platform to add
	 */
	void add_platform(Platform*);											//tested
	/**
	 * Get radius.
	 * 
	 * \return redius
	 */
	int get_radius() { return rad; }										//tested
	/**
	 * Delete platform from component table of manager.
	 *
	 * \param plat Platform to delete
	 */
	void delete_platform(Platform* plat);									
	/**
	 * Get if there is no place for platforms in Module.
	 * 
	 * \return num of platforms >= max num of platforms
	 */
	bool get_full_platforms() { return manobjs.component.size() >= maxnum; }
	int get_max_num_plat() { return maxnum; }
};

//full
class EnergyGenerator : public Module {
private:
	int outenergylevel;
public:
	~EnergyGenerator() {}
	/**
	 * Init constructor with 1 param.
	 * \param a - out energy level
	 */
	EnergyGenerator(int a);															//written
	/**
	 * Init constructor with 5 params.
	 * \param outenergylevel - Energy level of Energy Generator
	 * \param pri - Priority of Module
	 * \param enab - if Energy Generator is enable
	 * \param cost - Price of Module
	 * \param p - Platform where Module's standing
	 */
	EnergyGenerator(int outenergylevel, int pri, bool enab, int cost, Platform& p);	//written
	/**
	 * Getter of generator energy level.
	 *
	 * \return out energy level of geerator
	 */
	int get_energy_level();																		//written
};

/**
 * Give triangle of view.
 * A
 * |\ rad
 * |  \
 * |	\
 * |_ _ _ \ rad
 * O       B
 * \param radius Radius of view
 * \param vector Direstion of left side
 * \param coords Coordinates of O point
 * \return
 */
std::vector<std::pair<int, int>> find_triangle(int radius, Direction vector, std::pair<int, int> coords);
