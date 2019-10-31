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

//template <typename T>
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
			if (poly[i]) {
				if (i == 0) std::cout << "1";
				else std::cout << "+x^" << i;
			}
				
			///
		}
		point_first = convert();
	}
	~gen_lfsr() {
		
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

void sub_fun(int r, int delta, int t, int c,int b, int L, int *z) {
	cout << "\nr " << r << " Zr " << z[r - 1] << " delta " << delta << " c ";// << c << " b " << b;
	if (c & 1) {
		cout << "1";
	}
	for (int j = 1; j < 32; j++) {
		if ((c >> j) & 1) {
			cout << "+x^" << j;
		}
	}
	cout << " b ";
	if (b & 1) {
		cout << "1";
	}
	for (int j = 1; j < 32; j++) {
		if ((b >> j) & 1) {
			cout << "+x^" << j;
		}
	}
	cout << " L " << L;
}

void difficult(int *z, int count, int *result) {
	int r(0), delta(0), t(0), c(1), b(1), counter(0), L(0);
	for (int i(0); i < count; i++) {
		r++;
		delta = z[r - 1];
		for (int j = 1; j <= L; j++) {
			delta ^= ((c >> j) & 1u)* z[r - j - 1];
		}
		if (delta == 0) {
			b = b << 1;
			sub_fun(r, delta, t, c, b, L, z);
			continue;
		}
		else {
			t = c ^ (b << 1);
		}
		if ((2 * L) <= (r - 1)) {
			b = c;
			c = t;
			L = r - L;
			sub_fun(r, delta, t, c, b, L, z);
			continue;
		}
		else {
			c = t;
			b = b << 1;
		}
		///вывод
		sub_fun(r, delta, t, c, b, L, z);
	}
	cout << endl;
	/*cout << "Итоговый вектор ";
	if (c & 1) {
		cout << "1";
	}
	for (int j = 1; j < 32; j++) {
		if ((c >> j) & 1) {
			cout << "+x^" << j;
		}
	}
	cout << endl;*/
	result[0] = L;
	result[1] = c;
}

//template <class T>
struct still_lfsr {
	int polynom = 0;
	int* in_vec_value = NULL; //input vector value
	int* generate_vec = NULL;
	int min_polynom = 0;
	int min_exp = 0;
	still_lfsr(int polynom = NULL, int* in_vec_value = NULL, int step = 0) {
			int exp = 2 * step + 1;
			gen_lfsr gn = gen_lfsr(polynom, step, in_vec_value);
			generate_vec = new int[exp];
			for (int i = 0; i < exp; i++) {
				generate_vec[i] = gn.generate();
				cout << " Сгенерированное значение " << generate_vec[i];
			}
			int* result = new int[2];
			difficult(generate_vec, exp, result);
			min_polynom = result[0];
			min_exp = result[1];
			delete[] result;
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	int value[5] = { 1,0,0,0,0 };
	int polynom(23);
	still_lfsr *LFSR1 = new still_lfsr(polynom, value, sizeof(value) / sizeof(int));
	polynom = 25;
	int value2[4] = { 1,0,1,0 };
	still_lfsr *LFSR2 = new still_lfsr(polynom, value2, sizeof(value2) / sizeof(int));
	polynom = 35;
	int value3[6] = { 0,1,0,0,0,1 };
	still_lfsr* LFSR3 = new still_lfsr(polynom, value3, sizeof(value3) / sizeof(int));
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
