#include "../../lab4libnew/lab4libnew/app.h"
#include "../../lab4maplib/lab4maplib/mymap.h"
#include "../../lab4libnew/lab4libnew/dialog.h"


int main() {
	int width = 0, length = 0, num = 0, err = -1;
	while (width <= 0 || length <= 0 || num < 0) {
		std::cout << "Enter width" << std::endl;
		//std::cin >> width;
		while (err == -1) {
			err = getNum(width);
		}
		std::cout << "Enter length" << std::endl;
		err = -1;
//		std::cin >> length;
		while (err == -1)
			err = getNum(length);
		std::cout << "Enter number of objects" << std::endl;
		err = -1;
		while (err == -1)
			err = getNum(num);
		//std::cin >> num;
	}
	Application app(length, width, num);
	dialog_do_all(app);
	app.work();

	return 0;
}
