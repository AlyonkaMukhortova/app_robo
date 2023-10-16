#include "app.h"


void dialog_add_module(Platform& plat, AI& ai);
Object* dialog_add_object(EnvDescriptor* e, AI& ai);
void dialog_print_modules(Platform* plat);
void dialog_print_platform(Object* obj);
void dialog_print_object(Application& app);
void dialog_do_all(Application& app);

template <class T>
int getNum(T& a)
{
	std::cin >> a;
	if (!std::cin.good() || (std::cin.peek() != '\n')) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return -1;
	}
	return 1;
}

