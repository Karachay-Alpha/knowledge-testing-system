#include"Logics.h"

void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string path_acc = "DB\\";
	string path_tests = "tests\\";
	
	Logics YES(path_acc, path_tests);
	YES.LetsGo();
	system("pause");
}
