// LFSR.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Header.h" 

using namespace std;

string log_me;

int main()
{
	setlocale(LC_ALL, "Russian");
	double time = clock();
	/*bool value[5] = { 1,0,0,0,0 };
	bool polynom(23);
	still_lfsr* LFSR1 = new still_lfsr(polynom, value);
	polynom = 25;
	int value2[4] = { 1,0,1,0 };
	still_lfsr* LFSR2 = new still_lfsr(polynom, value2);
	polynom = 35;
	bool value3[6] = { 0,1,0,0,0,1 };
	still_lfsr* LFSR3 = new still_lfsr(polynom, value3);
	*/
	//
	//std::ofstream outfile("test2.txt");
	vector<still_lfsr*> *all_lfsr = new vector<still_lfsr*>();
	const char* fn = "test2.hex";
	read_hex_(fn, all_lfsr);
	operations(all_lfsr);
	log_me = log_me + "hell";
	log_me += "help";
	//cout << log_me;
	cout << "Время выполнения все программы " << clock() - time << " с";
	logging("Время ввыполнение всей программы ");
	logging(clock() - time);
	logging(" c");
	std::cout << "\nHello World!\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
