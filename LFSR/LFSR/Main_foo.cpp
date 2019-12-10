#include "Header.h"

infint difficult(bool* z, int count, int* min_exp) {
	unsigned long long r(0), counter(0), L(0);
	bool delta(0);
	infint t(0), c(1), b(1);
	for (int i(0); i < count; i++) {
		//cout << i << "\n";
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
	cout << "L" << L << endl;
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
	if (num_threads != 1) {
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
	else {
		for (int i(0); i < size; i++) {
			result[i] = a[i] * b[i];
		}
	}
}

void sum_thread(bool* a, bool* b, bool* result, int size, int num_threads) {
	if (num_threads != 1) {
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
	else {
		for (int i(0); i < size; i++) {
			result[i] = a[i] ^ b[i];
		}
	}
	
}

void op_th(vector<still_lfsr*>* all_lfsr, int exp, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		all_lfsr->at(i)->gen(exp);
	}
}

void operations(vector<still_lfsr*>* all_lfsr, deque<bool>* combination, int num_threads) { //TODO: rebuild
	int stl_i(0), exp(0), fond = log2(combination->size()), sub(0);
	for (int i(1); i < combination->size(); i++) {
		if (combination->at(i)) {
			for (int j(0); j < fond; j++) {
				sub = (i >> j) & 1;
				if (sub) {
					if (stl_i != 0) {
						stl_i = stl_i * all_lfsr->at(fond - j - 1)->exp;
					}
					else {
						stl_i = all_lfsr->at(fond - j - 1)->exp;
					}
				}
			}
			if (stl_i != 0) {
				if (exp != 0) {
					exp = exp + stl_i;
				}
				else {
					exp = stl_i;
				}
				stl_i = 0;
			}
		}
	}
	exp = 2 * exp + 1;
	if (num_threads == 1) {
		for (int i(0); i < all_lfsr->size(); i++) {
			all_lfsr->at(i)->gen(exp);
		}
	}
	else {
		int size = all_lfsr->size();
		vector<thread> threads(num_threads);
		if (size % num_threads == 0) {
			int new_size = size / num_threads;
			for (int i(0); i < num_threads; i++) {
				threads[i] = thread(op_th, all_lfsr, exp, new_size * (i + 1), new_size * i);
			}
			for (auto& th : threads) {
				th.join();
			}
		}
		else {
			int balalne = size % num_threads;
			int whole = size / num_threads;
			for (int i(0); i < num_threads - 1; i++) {
				threads[i] = thread(op_th, all_lfsr, exp, whole * (i + 1), whole * i);
			}
			num_threads--;
			threads[num_threads] = thread(op_th, all_lfsr, exp, whole * (num_threads + 1) + balalne, whole * num_threads);
			for (auto& th : threads) {
				th.join();
			}
		}
	}

	still_lfsr* battery = (still_lfsr*)malloc(sizeof(still_lfsr));
	still_lfsr* stl = (still_lfsr*)malloc(sizeof(still_lfsr));
	stl = NULL;
	battery = NULL;
	sub = 0;
	for (int i(1); i < combination->size(); i++) {
		if (combination->at(i)) {
			for (int j(0); j < fond; j++) {
				sub = (i >> j) & 1;
				if (sub) {
					if (stl != NULL) {
						cout << "Умножение stl * " << fond - j - 1 << endl;
						memcpy(stl, *stl * *all_lfsr->at(fond - j - 1), sizeof(still_lfsr));
					}
					else {
						cout << "Равенство stl = " << fond - j - 1 << endl;
						stl = (still_lfsr*)malloc(sizeof(still_lfsr));
						memcpy(stl, all_lfsr->at(fond - j - 1), sizeof(still_lfsr));
					}
				}	
			}
			if (stl != NULL) {
				if (battery != NULL) {
					cout << "Сумма battery + stl" << endl;
					memcpy(battery, *battery + *stl, sizeof(still_lfsr));
					//battery = *battery + *stl;
				}
				else {
					cout << "Равенство battery = stl" << endl;
					battery = (still_lfsr*)malloc(sizeof(still_lfsr));
					memcpy(battery, stl, sizeof(still_lfsr));
					battery = stl;
				}
				stl = NULL;
			}
		}
	}
	/*still_lfsr* test1 = *all_lfsr->at(0) * *all_lfsr->at(1);
	test1 = *test1 + *all_lfsr->at(2);*/
	int* min_exp = new int[0];
	infint min_polynom = difficult(battery->generate_vec, exp, min_exp);
	min_polynom.print();
	cout << endl;
}