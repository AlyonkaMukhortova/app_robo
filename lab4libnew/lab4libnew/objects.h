#pragma once
#include "module.h"
#include "enums.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>


class Object;
class Module;
class Manager;
class ScoutRobot;
class ManageRobot;
class ManageCentre;
class ObserveCentre;


/**
 * Descriptor of enviroment.
 */
class EnvDescriptor {
private:
	int width;
	int length;
	std::map<Object*, std::vector<Instructions>> curinstructions;
	std::map<Object*, std::vector<EnvErr>> curerrors;
	int numinterests=0;
	int budget = 200;

public:
	std::map<std::pair<int, int>, Object*> map;
	~EnvDescriptor();
	/**
	 * Print enviroment.
	 * Top is on the right side
	 */
	void print_enviroment();
	/**
	 * Param constructo of Descriptor of Enviroment.
	 * \param width - width of enviroment
	 * \param length - length of enviroment
	 */
	EnvDescriptor(int width = 10, int length = 10);							//written
	/**
	 * Changes enviroment size.
	 *
	 * \param w Width
	 * \param l Length
	 */
	void change_env_size(int, int);								//written
	/**
	 * Add object to map.
	 *
	 * \param o Pointer to object for add
	 */
	void add_object(Object*);									//written
	/**
	 * Delete object by coords.
	 *
	 * \param coords Pair<x,y> to delete object
	 */
	void delete_object(std::pair<int, int>);					//written
	/**
	 * Give commands to 1 object by coords.
	 *
	 * \param coor pair<x,y> coords
	 * \param inst Vector of instructions
	 */
	void change_object(std::pair<int, int>, std::vector<Instructions>);
	/**
	 * Check if there's smth on given point.
	 *
	 * \param p - Pair<x,y> to check
	 * \return Pointer to object if there's some or nullptr
	 */
	Object* check_coordinates(std::pair<int, int>);				//written
	/**
	 * Get all errors for all objects.
	 *
	 * \return  Map of Object and vector of its errors
	 */
	std::map<Object*, std::vector<EnvErr>> get_errors();		//written
	/**
	 * Get size of enviroment.
	 *
	 * \return pair<width, length>
	 */
	std::pair<int, int> get_size();								//written
	/**
	 * Get and do instructions for all objects + moves platforms.
	 *
	 * \param inst Map of objects and its instructions
	 */
	void get_instructions(std::map<Object*, std::vector<Instructions>> inst);
	/**
	 * Getter of number of points of interest.
	 * 
	 * \return number of points of interest
	 */
	int get_num_interests() { return numinterests; }
	/**
	 * Setter of number of points of interest.
	 * 
	 * \param i - new number of points of interest
	 */
	void set_num_interests(int i) { numinterests = i; }
	void move_objs();
};

/**
 * Objects of enviroment. 3 possible types: obstacle, platform and point of interest
 */
class Object {
protected:
	std::pair<int, int> coordinates;
	ObjType type;
	EnvDescriptor* env;
	std::vector<EnvErr> curerrs;
public:
	virtual void v() {}
	~Object() {
		curerrs.clear();
	}
	/**
	 * Empty constructor of object.
	 */
	Object();										//written
	/**
	 * Param constructor of object.
	 * \param c - coordinates of object
	 * \param t - type of object
	 * \param enviroment - pointer to enviroment descriptor
	 */
	Object(std::pair<int, int> c, ObjType t, EnvDescriptor* enviroment);			//written
	/**
	 * Getter of coords of object
	 *
	 * \return  coordinates of object
	 */
	std::pair<int, int> get_coordinates();			//written
	/**
	 * Setter for coordinates of object
	 *
	 * \param x in pair<x,y>
	 * \param y in pair<x,y>
	 */
	void set_coordinates(int, int);					//written
	/**
	 * Setter of coordinates.
	 * \param a - new coordinates of object
	 */
	void set_coordinates(std::pair<int, int> a);		//written
	/**
	 * Get type of object.
	 *
	 * \return object type - Platform, SmthInteresting or Obstacle
	 */
	ObjType get_obj_type();							//written
	/**
	 * Getter of pointer of enviroment descriptor.
	 */
	EnvDescriptor* ret_env() { return env; }		//written
	/**
	 * Get vector of errors of object.
	 */
	std::vector<EnvErr> get_obj_err();	//written
	/**
	 * Operator greater.
	 * 
	 * \param a - second object of compairing
	 * \return bool if greater
	 */
	bool operator>(Object a) const{ return coordinates > a.coordinates; }
	/**
	 * Operator less.
	 *
	 * \param a - second object of compairing
	 * \return bool if less
	 */
	bool operator<(Object a) const{ return coordinates < a.coordinates; }
	/**
	 * Operator equal.
	 *
	 * \param a - second object of compairing
	 * \return bool if equal
	 */
	bool operator==(Object a) const { return coordinates == a.coordinates; }
};
class ScoutRobot;



/**
 * Platform. 4 possible types: Manage Robot, Scout Robot, Manage Centre, Observe Centre
 */
class Platform : public Object {
protected:
	int neededenergy = 0;
	int maxmodulesnum;
	std::vector<Module*> modules;
	ComponentTypes type;
	bool managed = false;
	Manager* manager = nullptr;
public:
	std::vector<int> ints;
	int price;
	std::string description =  "some description";
	virtual void v() {}
	~Platform();
	std::vector<Module*> get_modules() { return modules; }
	void new_manager(Manager* m) { manager = m; managed = true; }
	void set_modules(std::vector<Module*> ii) { modules = ii; }
	Platform(int, ComponentTypes, int, std::string, std::pair<int, int>, EnvDescriptor*);	//written
	/**
	 * Adding new module to platform
	 * \param m - Module to add
	 * \return if success adding
	 */
	bool add_module(Module*);												//written
	/**
	 * Deletes module from platform if there's such.
	 *@brief qwe
	 * \param m - ptr to module to delete
	 */
	void delete_module(Module*);
	/**
	 * Turns module with highest priority on.
	 */
	void turn_module_on();													//written											
	/**
	 * Turns module with lowest priority off.
	 *
	 */
	void turn_module_off();													//written
	/**
	 * Get overall energy level.
	 *
	 * \return in energy level
	 */
	int get_cur_energy_level();												//written
	/**
	 * Gets some of platform problems.
	 *
	 * \return all list of platform errors
	 */
	std::vector<EnvErr> get_status();										//written
	/**
	 * Gets commands and do them.
	 *
	 * \param instructions Instructions for Platform
	 */
	void get_command(std::vector<Instructions>);							//written so-so
	/**
	 * Get type of platform.
	 *
	 * \return type of platform - ManCentre, ManRobot, ScRobot or ObsCentre
	 */
	ComponentTypes get_type();												//written
	bool get_managed() { return managed; }					//written
	int get_max_modules_num() { return maxmodulesnum; }
	Manager* get_manager() { return manager; }
	void set_managed(bool m) { managed = m; }
};

/**
 * Manage platform. Only on such modules can be built.
 */
class ManagePlatform : virtual public Platform {
public:
	int a = 8;
	~ManagePlatform() {}
	ManagePlatform(int a, ComponentTypes b, int c, std::string d, std::pair<int, int> e, EnvDescriptor* ed) :
		Platform(a, b, c, d, e, ed) {};
	/**
	 * Get errors.
	 *
	 * \param p Platform for get errors
	 * \return platform p errors
	 */
	std::vector<EnvErr> get_error_messages(Platform*);
};

/**
 * Mobile Platform, contains speed and speed direction.
 */
class MobilePlatform : virtual public Platform {
protected:
	int speedval;
	Direction speeddir;
	int curspeedval;
public:
	~MobilePlatform() {}													//written
	MobilePlatform(int, ComponentTypes, int, std::string,					//written
		int, Direction, std::pair<int, int>, EnvDescriptor*);				//written
	/**
	 * Moves platform for curspeed * 1.
	 *
	 */
	void move_platform();													//written
	/**
	 * Checks if moving is possible. If not, changes curspeed to maximum value to move
	 *
	 */
	void ask_move_permission();						//written
	/**
	 * Set cur speed value.
	 * \param c - New current speed value
	 */
	void set_cur_speed(int c) { curspeedval = c; }							//written
	/**
	 * Get speed value.
	 * \return speed value
	 */
	int get_speed_val() { return speedval; }								//written													//written
	/**
	 * Turns platform pi/2 right.
	 *
	 */
	void turn();															//written															//written
	/**
	 * Get direction of move.
	 * \return speed direction
	 */
	Direction get_dir() { return speeddir; }								//written
	/**
	 * Gets all instruction for mobile platform and platform.
	 *
	 * \param instructions All instructions for platform
	 */
	void get_commands(std::vector<Instructions>);							//written
};

/**
 * Manage Centre. Stationary, Manage Platform
 */
class ManageCentre :public ManagePlatform {
public:
	~ManageCentre() {}
	ManageCentre(int a, int c, std::string d, std::pair<int, int> e, EnvDescriptor* ed) :
		Platform(a, ComponentTypes::ManCentre, c, d, e, ed),
		ManagePlatform(a, ComponentTypes::ManCentre, c, d, e, ed) {};
};

/**
 * Manage Robot. Mobile, Manage Platform
 */
class ManageRobot :public MobilePlatform, public ManagePlatform {
public:
	~ManageRobot() {}
	ManageRobot(int, int, std::string, int,
		Direction, std::pair<int, int>, EnvDescriptor*);
};

/**
 * Observe Centre. Non-Manage, Stationary Platform
 */
class ObserveCentre :public Platform {
public:
	~ObserveCentre() {}
	ObserveCentre(int, int, std::string, std::pair<int, int>, EnvDescriptor*);
};

/**
 * Scout Robot. Non-Manage, Mobil Platform.
 */
class ScoutRobot :public MobilePlatform {

public:
	~ScoutRobot() {}
	ScoutRobot(int, int, std::string, int, Direction, std::pair<int, int>, EnvDescriptor*);
};

