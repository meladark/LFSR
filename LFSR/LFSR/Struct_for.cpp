#include "Header.h"

using namespace std;
void infint::print() {
	long long int k = 0;
	for (int i(0); i < b.size(); i++) {
		cout << b[i] ? '1' : '0';
		//if (b[i]) k += pow(2, i);
	}
	cout << endl;
	if (b[0]) cout << "1";
	for (int i(1); i < b.size(); i++) {
		if (b[i]) cout << "+x^" << i;
	}
	//cout << k << endl;
};

int infint::operator[](int x)
{
	return b[x];
}
infint infint::operator<<(int x) {
	infint sub = *this;
	for (int i(0); i < x; i++)
		sub.b.push_front(false);
	return sub;
	}
infint infint::operator^(infint x) {
	intmax_t s = (b.size() < x.b.size()) ? b.size() : x.b.size();
	infint sub1 = (b.size() > x.b.size()) ? *this : x;
	infint sub2 = !(b.size() > x.b.size()) ? *this : x;
	for (intmax_t i(0); i < s; i++) {
		sub1.b[i] ^= sub2.b[i];
	}
	return sub1;
}
void infint::operator=(infint x) {
	b = x.b;
}

void gen_mult(deque<bool>* polinom, deque<bool>* values, bool* result, int size, int first_point) {
	for (int i(first_point); i < size; i++) {
		result[i] = polinom->at(i) * values->at(i);
	}
}


bool gen_lfsr::generate(int num_threads) {
	///
	/*cout << "\nМассив ";
	logging("\nМассив ");
	for (int i(0); i < values->size(); i++) {
	cout << values->at(i);
	}*/
	///
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

void still_lfsr::gen(int num) {
	bool* mas = new bool[num];
	for (int i(0); i < exp; i++) {
		mas[i] = generate_vec[i];
	}
	for (int i(exp); i < num; i++) {
		mas[i] = gn->generate();
	}
	generate_vec = mas;
	exp = num;
};

still_lfsr* still_lfsr::operator+ (still_lfsr right) {
	/*cout << "Сумма" << endl;
	logging("Сумма\n");
	for (int i(0); i < right.exp; i++) {
		cout << i << ") " << generate_vec[i] << " + " << right.generate_vec[i] << endl;
		logging(i);
		logging(") ");
		logging(generate_vec[i]);
		logging(" + ");
		logging(right.generate_vec[i]);
		logging('\n');
	}*/
	sum_thread(generate_vec, right.generate_vec, right.generate_vec, exp);
	/*cout << "Результат"<<endl;
	logging("Результат\n");
	for (int i(0); i < exp; i++) {
		cout << i << ") " << right.generate_vec[i] << endl;
		logging(i);
		logging(") ");
		logging(right.generate_vec[i]);
		logging('\n');
	}*/
	return &right;
};

still_lfsr* still_lfsr::operator* (still_lfsr right) {
	/*cout << "Умножение" << endl;
	logging("Умножение\n");
	for (int i(0); i < right.exp; i++) {
		cout << i << ") " << generate_vec[i] << " * " << right.generate_vec[i] << endl;
		logging(i);
		logging(") ");
		logging(generate_vec[i]);
		logging(" * ");
		logging(right.generate_vec[i]);
		logging('\n');
	}*/
	multip_thread(generate_vec, right.generate_vec, right.generate_vec, exp);
	/*cout << "Результат" << endl;
	for (int i(0); i < exp; i++) {
		cout << i << ") " << right.generate_vec[i] << endl;
		logging(i);
		logging(") ");
		logging(right.generate_vec[i]);
		logging('\n');
	}*/
	return &right;
};