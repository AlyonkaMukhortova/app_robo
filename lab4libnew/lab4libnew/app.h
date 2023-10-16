#pragma once
#include "ai.h"



class Application {
private:
	std::map<Object*, std::vector<EnvErr>> errors;
	std::map<Object*, std::vector<Instructions>> instructions;
public:
	EnvDescriptor* enviroment;// = new EnvDescriptor();
	AI ai;
	/**
	 * Empty constructor of Application.
	 */
	Application() {};								//tested
	~Application() {
		if (enviroment != nullptr)
			delete enviroment;
		errors.clear();
		instructions.clear();
	}
	/**
	 * Param constructor of Application.
	 * 
	 * \param width - width of enviroment of app
	 * \param length - length of enviroment of app
	 * \param objnum - number of objects in enviroment of app
	 */
	Application(int width, int length, int objnum);							//tested
	/**
	 * Give instructions to enviroment.
	 *
	 * \param inst Map of objects and their instrudtions
	 */
	void change_enviroment(std::map<Object*, std::vector<Instructions>>);	//written
	/**
	 * Get instructions from ai.
	 *
	 * \return all instructions for objects
	 */
	std::map<Object*, std::vector<Instructions>> check_ai_status();			//written
	/**
	 * Give errors to ai'n'give ai command to get info from all managers.
	 *
	 * \param errs Map of objs'n'vector of errors
	 * \param a Smth
	 * \param b Smth
	 */
	void kick_ai(std::map<Object*, std::vector<EnvErr>>, int, int);			//written
	/**
	 * Get all errors for all objects from enviroment.
	 *
	 * \return map of obj'n'vector of errors
	 */
	std::map<Object*, std::vector<EnvErr>> get_env_status();				//written
	/**
	 * Work of app: print of each env condition. Get errors of env, give them to ai. Get instructions from ai, give them to enviroment.
	 *
	 */
	void work();
};
