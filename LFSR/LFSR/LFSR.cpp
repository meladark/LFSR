// LFSR.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <io.h>

using namespace std;

struct list_lfsr
{
	int value;
	list_lfsr* point_next;
	list_lfsr(int value, list_lfsr* point_next) {
		this->value = value;
		this->point_next = point_next;
	}
};

struct gen_lfsr
{
	int step;
	bool* poly;
	int* values;
	list_lfsr *point_first;
	gen_lfsr(int polinom, int step, int* values) { //
		this->poly = new bool[++step];
		this->values = values;
		this->step = step;
		for (int i = 0; i < step; i++) {
			poly[i] = (((int)pow(2, i) & polinom) >> i) > 0 ? true : false;
			///
			std::cout << poly[i];
			///
		}
		point_first = convert();
	}
public:
	int generate() { //todo: переделать генерацию, так как ксорятся все x 
		int result(0);
		bool equal(false);
		list_lfsr* sub_point;
		///
		cout << "\nМассив ";
		sub_point = point_first;
		for (int i(0); i < step - 1; i++) {
			cout << sub_point->value;
			sub_point = sub_point->point_next;
		}
		///
		sub_point = point_first;
		for (int i(1); i < this->step - 1; i++) {
			if (poly[i]) {
				if (equal) {
					result ^= sub_point->value;
				}
				else {
					result = sub_point->value;
					equal = true;
				}	
			}
			sub_point = sub_point->point_next;
		}
		if (poly[this->step - 1]) {
			if (equal) {
				result ^= sub_point->value;
			}
			else {
				result = sub_point->value;
				equal = true;
			}
		}
		int out = sub_point->value;
		sub_point->value = result;
		sub_point->point_next = point_first;
		point_first = sub_point;
		return out;
	}
	list_lfsr* convert() {
		list_lfsr *first_point = new list_lfsr(values[step - 2], NULL);
		for (int i = step - 3; i >= 0; i--) {
			first_point = new list_lfsr(values[i], first_point);
		}
		return first_point;
	}
};


int difficult(int *z, int count) {
	int r(0), delta(0),t(0), c(1), b(1), counter(0);
	for (int i(0); i < count - 1; i++) {
		r++;
		delta = z[r];
		for (int j = 1; j < r - 1; j++) {
			delta ^= ((c >> j) & 1u)* z[r - j];
		}
		if (delta == 0) {
			b = b << 1;
		}
		else {
			t = c + (b << 1);
			b = c;
			c = t;
		}
		cout << "\nr " << r << " Zr " << z[r] << " delta " << delta << " c " << c << " b " << b;
	}
	int i(8 * sizeof(c));
	while (i != -1 && !((c >> --i) & 1));
	return i;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	int value[6] = { 0,1,0,0,0,1 };
	int polynom(34);
	int i(8 * sizeof(polynom));
	while (i != -1 && !((polynom >> --i) & 1));
	int count(2 * i);
	gen_lfsr gn = gen_lfsr(polynom, sizeof(value)/sizeof(int), value);
	int* mas = new int[20];
	
	for (int i(0); i < count; i++) {
		mas[i] = gn.generate();
		cout << "\nСгенерированное число " << mas[i];
	}
	int mas2[12] = {0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1 };
	cout << "\nЛинейная сложность " << difficult(mas, count);
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
