#include "Header.h"

infint difficult(bool* z, int count, int* min_exp) {
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
			//sub_fun(r, delta, c, b, L, z);
			continue;
		}
		else {
			t = c ^ (b << 1);
		}
		if ((2 * L) <= (r - 1)) {
			b = c;
			c = t;
			L = r - L;
			//sub_fun(r, delta, c, b, L, z);
			continue;
		}
		else {
			c = t;
			b = b << 1;
		}
		///вывод
		//sub_fun(r, delta, c, b, L, z);
	}
	//cout << endl;
	//logging('\n');
	min_exp[0] = (int)L;
	return c;
}

void multip(bool* a, bool* b, bool* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = a[i] * b[i];
	}
}

void sum(bool* a, bool* b, bool* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = a[i] ^ b[i];
	}
}

void multip_thread(bool* a, bool* b, bool* result, int size, int num_threads) {
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

void sum_thread(bool* a, bool* b, bool* result, int size, int num_threads) {
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

void operations(vector<still_lfsr*>* all_lfsr) {
	int exp = all_lfsr->at(0)->exp * all_lfsr->at(1)->exp + all_lfsr->at(2)->exp;
	all_lfsr->at(0)->gen(exp);
	all_lfsr->at(1)->gen(exp);
	all_lfsr->at(2)->gen(exp);
	still_lfsr* test1 = *all_lfsr->at(0) * *all_lfsr->at(1);
	test1 = *test1 + *all_lfsr->at(2);
	int* min_exp = new int[0];
	infint min_polynom = difficult(test1->generate_vec, exp, min_exp);
	min_polynom.print();
	cout << endl;
}