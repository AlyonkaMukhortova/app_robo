#pragma once
#include "objects.h"
#include "module.h"

/**
 * Class of Artificial Intelligence.
 */
class AI {
private:
	std::vector<Manager*> modules;
	std::map<Object*, std::vector<EnvErr>> curerr;
	std::map<Object*, std::vector<Instructions>> currinst;
	std::map<std::pair<int, int>, Object*> plan;
	int numinterests = 0;
	int timer=0;
	std::pair<int, int> envsize;
public:
	~AI();
	/**
	 * Constructor - empty and only.
	 */
	AI() {
		int i = 0;
	};																	//tested
	/**
	 * Get errors from enviroment.
	 *
	 * \param errs map objs'n'errors
	 * \param a Smth
	 * \param b Smth
	 */
	void get_errors(std::map<Object*, std::vector<EnvErr>>, int, int);			//tested
	/**
	 * Get info from 1 manager.
	 *
	 * \param man Manager
	 * \return  component table of manager
	 */
	ComponentTable& get_info(Manager&);											//tested
	/**
	 * Give instructions to enviroment.
	 *
	 * \return map objs'n'instructions
	 */
	std::map<Object*, std::vector<Instructions>> give_instructions();			//tested
	/**
	 * Make map from component table.
	 *
	 * \param tab component table to make map
	 */
	void from_info_to_plan(ComponentTable&);									//tested
	/**
	 * Get info from sensors and manager.
	 *
	 */
	void get_all_info();														//tested
	/**
	 * Add manager to maager list of AI.
	 * 
	 * \param m - new manager
	 */
	void add_manager(Manager* m) { modules.push_back(m); }						//tested
	/**
	 * Set number of points of interest.
	 * 
	 * \param i - new num of points of interest
	 */
	void set_int_num(int i) { numinterests = i; }								//tested
	/**
	 * Get number of points of interest.
	 * 
	 * \return number of points of interest
	 */
	int get_int_num() { return numinterests; }									//tested
	/**
	 * Get current timer.
	 * 
	 * \return int timer
	 */
	int get_timer() { return timer; }											//tested
	void print_enviroment();
};