// LFSR.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <io.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <malloc.h>

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

struct infint
{
	deque<bool> b;
	infint(int x) {
		for (int i(0); i < sizeof(x) * 8; i++) {
			b.push_back((x >> i) & 1u);
		}
	}
	void print() {
		long long int k = 0;
		for (int i(0); i < b.size(); i++) {
			cout << b[i] ? '1' : '0';
			if (b[i]) k += pow(2, i);
		}
		cout << endl;
		cout << k << endl;
	}
	int operator[](int x)
	{
		return b[x];
	}
	infint operator<<(int x) {
		infint sub = *this;
		for (int i(0); i < x; i++)
			sub.b.push_front(false);
		return sub;
	}
	infint operator^(infint x) {
		intmax_t s = (b.size() < x.b.size()) ? b.size() : x.b.size();
		infint sub1 = (b.size() > x.b.size()) ? *this : x;
		infint sub2 = !(b.size() > x.b.size()) ? *this : x;
		for (intmax_t i(0); i < s; i++) {
			sub1.b[i] ^= sub2.b[i];
		}
		return sub1;
	}
	void operator=(infint x) {
		b = x.b;
	}
};

void gen_mult(deque<bool>* polinom, deque<bool>* values, bool* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = polinom->at(i) * values->at(i);
	}
}

struct gen_lfsr
{
	deque<bool>* values;
	deque<bool>* polinom;
	gen_lfsr(deque<bool>* polinom, deque<bool>* values) { //
		this->values = values;
		this->polinom = polinom;
		for (int i(0); i < polinom->size(); i++) {
			if (polinom->at(i)) {
				std::cout << "x^" << polinom->size() - i - 1 << " + "; logging("+x^"); logging(i); 
			}
		}
		std::cout << "1"; logging("1");
	}
	~gen_lfsr() {
		delete[] values;
		delete[] polinom;
	}
public:
	bool generate(int num_threads = 2) {
		///
		cout << "\nМассив ";
		logging("\nМассив ");
		for (int i(0); i < values->size(); i++) {
			cout << values->at(i);
		}
		bool* results = new bool[polinom->size()];
		vector<thread> threads(num_threads);
		if (polinom->size() % num_threads == 0) {
			int new_size = polinom->size() / num_threads;
			for (int i(0); i < num_threads; i++) {
				threads[i] = thread(gen_mult, polinom, values, results, new_size * (i + 1), new_size * i);
			}
			for (auto& th : threads) {
				th.join();
			}
		}
		else {
			int balalne = polinom->size() % num_threads;
			int whole = polinom->size() / num_threads;
			for (int i(0); i < num_threads - 1; i++) {
				threads[i] = thread(gen_mult, polinom, values, results, whole * (i + 1), whole * i);
			}
			num_threads--;
			threads[num_threads] = thread(gen_mult, polinom, values, results, whole * (num_threads + 1) + balalne, whole * num_threads);
			for (auto& th : threads) {
				th.join();
			}
		}
		bool out = results[0];
		for (int i(1); i < polinom->size(); i++) {
			out ^= results[i];
		}
		values->push_front(out);
		out = values->back();
		values->pop_back();
		return out;
	}
};

void sub_fun(int r, unsigned long long delta, infint c, infint b, unsigned long long L, bool *z) {
	cout << "\nr " << r << " Zr " << z[r - 1] << " delta " << delta << " c ";// << c << " b " << b;
	logging("\nr ");
	logging(r);
	logging(" Zr ");
	logging(z[r - 1]);
	logging(" delta ");
	logging(delta);
	logging(" c ");
	if (c[0]) {
		logging("1");
		cout << "1";
	}
	for (int j = 1; j < c.b.size(); j++) {
		if (c[j]) {
			cout << "+x^" << j;
			logging("+x^"); logging(j);
		}
	}
	cout << " b ";
	logging(" b ");
	if (b[0]) {
		cout << "1";
		logging("1");
	}
	for (int j = 1; j < b.b.size(); j++) {
		if (b[j]) {
			cout << "+x^" << j;
			logging("+x^"); logging(j);
		}
	}
	cout << " L " << L;
	logging(" L ");
}

void difficult(bool *z, int count, int *result) {
	unsigned long long r(0), counter(0), L(0);
	bool delta(0);
	infint t(0), c(1), b(1);
	for (int i(0); i < count; i++) {
		r++;
		delta = z[r - 1];
		for (int j = 1; j <= L; j++) {
			delta ^= c[j] * z[r - j - 1];
		}
		if (delta == 0) {
			b = b << 1;
			sub_fun(r, delta, c, b, L, z);
			continue;
		}
		else {
			t = c ^ (b << 1);
		}
		if ((2 * L) <= (r - 1)) {
			b = c;
			c = t;
			L = r - L;
			sub_fun(r, delta, c, b, L, z);
			continue;
		}
		else {
			c = t;
			b = b << 1;
		}
		///вывод
		sub_fun(r, delta, c, b, L, z);
	}
	cout << endl;
	logging('\n');
	result[0] = (int)L;
	result[1] = (int)5;
}

void multip(bool* a, bool *b, bool* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = a[i] * b[i];
	}
}

void sum(bool* a, bool* b, bool* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = a[i] ^ b[i];
	}
}

void multip_thread(bool* a, bool* b, bool* result, int size, int num_threads = 5) {
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

void sum_thread(bool* a, bool* b, bool* result, int size, int num_threads = 5) {
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
	bool* generate_vec = new bool[1];
	int min_polynom = 0;
	int min_exp = 0;
	int exp = 0;
	gen_lfsr *gn;
	still_lfsr(deque<bool>* polynom = NULL, deque<bool>* in_vec_value = NULL) {
		exp = 2 * polynom->size() + 1;
		gn = new gen_lfsr(polynom, in_vec_value);
		generate_vec = new bool[exp];
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
		bool* mas = new bool[num];
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

void thr(still_lfsr** all_lfsr, int i, deque<bool>* lf, deque<bool>* mas) {
	still_lfsr *st = new still_lfsr(lf, mas);
	all_lfsr[i] = st;
}

void read_hex_(const char* file_txt, vector<still_lfsr*>* all_lfsr, bool thrb = true) {
	ifstream input(file_txt, ios::binary);
	char x('h'), y('k'), sub('o');
	input >> x;
	vector<thread> threads((int)x);
	still_lfsr** stl = (still_lfsr**)malloc((int)x * sizeof(still_lfsr*));
	for (int i(0); i < (int)x; i++) {
		deque<bool>* lf = new deque<bool>();
		input >> y;
		for (int j(0); j < (int)y; j++) {
			input >> sub;
			lf->push_front((bool)sub);
		}
		deque<bool>* mas = new deque<bool>();
		for (int j(0); j < (int)y; j++) {
			input >> sub;
			mas->push_back((bool)sub);
		}
		if (thrb) {
			threads[i] = thread(thr, stl, i, lf, mas);
		}
		else {
			all_lfsr->push_back(new still_lfsr(lf, mas));
		}
	}
	if (thrb) {
		for (auto& th : threads) {
			th.join();
		}
		for (int i(0); i < (int)x; i++) {
			all_lfsr->push_back(stl[i]);
		}
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
	std::ofstream outfile("test2.txt");
	vector<still_lfsr*> *all_lfsr = new vector<still_lfsr*>();
	const char* fn = "F:\\LFSR\\LFSR\\LFSR\\test2.hex";
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
