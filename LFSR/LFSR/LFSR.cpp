// LFSR.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <io.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
template <typename T>
void logging(T in) {
	return;
	ofstream out;
	out.open("log1.txt", ios::app);
	if (out.is_open()) {
		out << in;
	}
	out.close();
}

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
				if (i == 0) { std::cout << "1"; logging("1"); }
				else { std::cout << "+x^" << i; logging("+x^"); logging(i); }
			}
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
		logging("\nМассив ");
		sub_point = point_first;
		for (int i(0); i < step - 1; i++) {
			cout << sub_point->value;
			logging(sub_point->value);
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
	logging("\nr ");
	logging(r);
	logging(" Zr ");
	logging(z[r - 1]);
	logging(" delta ");
	logging(delta);
	logging(" c ");
	if (c & 1) {
		logging("1");
		cout << "1";
	}
	for (int j = 1; j < 32; j++) {
		if ((c >> j) & 1) {
			cout << "+x^" << j;
			logging("+x^"); logging(j);
		}
	}
	cout << " b ";
	logging(" b ");
	if (b & 1) {
		cout << "1";
		logging("1");
	}
	for (int j = 1; j < 32; j++) {
		if ((b >> j) & 1) {
			cout << "+x^" << j;
			logging("+x^"); logging(j);
		}
	}
	cout << " L " << L;
	logging(" L ");
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
	logging('\n');
	result[0] = L;
	result[1] = c;
}

//template <class T>




void multip(int* a, int *b, int* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = a[i] * b[i];
	}
}

void sum(int* a, int* b, int* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = a[i] ^ b[i];
	}
}

void multip_thread(int* a, int* b, int* result, int size, int num_threads = 5) {
	vector<thread> threads(num_threads);
	if (size % num_threads == 0) {
		int new_size = size / num_threads;
		for (int i(0); i < num_threads; i++) {
			threads[i] = thread(multip, a, b, result, new_size * (i + 1), new_size * i);
		}
		for (auto& th : threads) {
			th.join();
		}
	}
	else {
		int balalne = size % num_threads;
		int whole = size / num_threads;
		for (int i(0); i < num_threads - 1; i++) {
			threads[i] = thread(multip, a, b, result, whole * (i + 1), whole * i);
		}
		num_threads--;
		threads[num_threads] = thread(multip, a, b, result, whole * (num_threads + 1) + balalne, whole * num_threads);
		for (auto& th : threads) {
			th.join();
		}
	}
}

void sum_thread(int* a, int* b, int* result, int size, int num_threads = 5) {
	vector<thread> threads(num_threads);
	if (size % num_threads == 0) {
		int new_size = size / num_threads;
		for (int i(0); i < num_threads; i++) {
			threads[i] = thread(sum, a, b, result, new_size * (i + 1), new_size * i);
		}
		for (auto& th : threads) {
			th.join();
		}
	}
	else {
		int balalne = size % num_threads;
		int whole = size / num_threads;
		for (int i(0); i < num_threads - 1; i++) {
			threads[i] = thread(sum, a, b, result, whole * (i + 1), whole * i);
		}
		num_threads--;
		threads[num_threads] = thread(sum, a, b, result, whole * (num_threads + 1) + balalne, whole * num_threads);
		for (auto& th : threads) {
			th.join();
		}
	}
}

class still_lfsr {
public:
	int polynom = 0;
	int* in_vec_value = NULL; //input vector value
	int* generate_vec = NULL;
	int min_polynom = 0;
	int min_exp = 0;
	int exp = 0;
	gen_lfsr *gn;
	still_lfsr(int polynom = NULL, int* in_vec_value = NULL, int step = 0) {
		exp = 2 * step + 1;
		gn = new gen_lfsr(polynom, step, in_vec_value);
		generate_vec = new int[exp];
		for (int i = 0; i < exp; i++) {
			generate_vec[i] = gn->generate();
			cout << " Сгенерированное значение " << generate_vec[i];
			logging(" Сгенерированное значение ");
			logging(generate_vec[i]);
		}
		int* result = new int[2];
		difficult(generate_vec, exp, result);
		min_polynom = result[0];
		min_exp = result[1];
		delete[] result;
	}
	void gen(int num) {
		int* mas = new int[num];
		for (int i(0); i < exp; i++) {
			mas[i] = generate_vec[i];
		}
		for (int i(exp); i < num; i++) {
			mas[i] = gn->generate();
		}
		generate_vec = mas;
		exp = num;
	}
	still_lfsr* operator+ (still_lfsr right) {
		cout << "Сумма" << endl;
		logging("Сумма\n");
		for (int i(0); i < right.exp; i++) {
			cout << i << ") " << generate_vec[i] << " + " << right.generate_vec[i] << endl;
			logging(i);
			logging(") ");
			logging(generate_vec[i]);
			logging(" + ");
			logging(right.generate_vec[i]);
			logging('\n');
		}
		sum_thread(generate_vec, right.generate_vec, right.generate_vec, exp);
		cout << "Результат"<<endl;
		logging("Результат\n");
		for (int i(0); i < exp; i++) {
			cout << i << ") " << right.generate_vec[i] << endl;
			logging(i);
			logging(") ");
			logging(right.generate_vec[i]);
			logging('\n');
		}
		return &right;
	}
	still_lfsr* operator* (still_lfsr right) {
		cout << "Умножение" << endl;
		logging("Умножение\n");
		for (int i(0); i < right.exp; i++) {
			cout << i << ") " << generate_vec[i] << " * " << right.generate_vec[i] << endl;
			logging(i);
			logging(") ");
			logging(generate_vec[i]);
			logging(" * ");
			logging(right.generate_vec[i]);
			logging('\n');
		}
		multip_thread(generate_vec, right.generate_vec, right.generate_vec, exp);
		cout << "Результат" << endl;
		for (int i(0); i < exp; i++) {
			cout << i << ") " << right.generate_vec[i] << endl;
			logging(i);
			logging(") ");
			logging(right.generate_vec[i]);
			logging('\n');
		}
		return &right;
	}
};

void read_hex_(const char* file_txt, vector<still_lfsr*>* all_lfsr) {
	ifstream input(file_txt, ios::binary);
	char x('h'), y('k'), sub('o');
	input >> x;
	for (int i(0); i < (int)x; i++) {
		int lf(0);
		input >> y;
		for (int j(0); j < (int)y; j++) {
			input >> sub;
			lf += (int)sub * (int)pow(2, (int)y - j);
		}
		lf++;
		int* mas = new int[(int)y];
		for (int j(0); j < (int)y; j++) {
			input >> sub;
			mas[j] = (int)sub;
		}
		all_lfsr->push_back(new still_lfsr(lf, mas, (int)y));
	}
	input.close();
}

void operations(vector<still_lfsr*> *all_lfsr) {
	int exp = all_lfsr->at(0)->exp * all_lfsr->at(1)->exp + all_lfsr->at(2)->exp;
	all_lfsr->at(0)->gen(exp);
	all_lfsr->at(1)->gen(exp);
	all_lfsr->at(2)->gen(exp);
	still_lfsr* test1 = *all_lfsr->at(0) * *all_lfsr->at(1);
	test1 = *test1 + *all_lfsr->at(2);
	int* result = new int[exp];
	difficult(test1->generate_vec, exp, result);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	double time = clock();
	int value[5] = { 1,0,0,0,0 };
	int polynom(23);
	still_lfsr* LFSR1 = new still_lfsr(polynom, value, sizeof(value) / sizeof(int));
	polynom = 25;
	int value2[4] = { 1,0,1,0 };
	still_lfsr* LFSR2 = new still_lfsr(polynom, value2, sizeof(value2) / sizeof(int));
	polynom = 35;
	int value3[6] = { 0,1,0,0,0,1 };
	still_lfsr* LFSR3 = new still_lfsr(polynom, value3, sizeof(value3) / sizeof(int));
	
	//
	std::ofstream outfile("test1.txt");
	vector<still_lfsr*> *all_lfsr = new vector<still_lfsr*>();
	const char* fn = "F:\\LFSR\\LFSR\\LFSR\\test.hex";
	read_hex_(fn, all_lfsr);
	operations(all_lfsr);
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
