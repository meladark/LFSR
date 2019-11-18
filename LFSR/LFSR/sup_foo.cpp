#include "Header.h"

void read_hex_(const char* file_txt, vector<still_lfsr*>* all_lfsr, bool thrb) {
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

void thr(still_lfsr** all_lfsr, int i, deque<bool>* lf, deque<bool>* mas) {
	still_lfsr* st = new still_lfsr(lf, mas);
	all_lfsr[i] = st;
}

void sub_fun(int r, unsigned long long delta, infint c, infint b, unsigned long long L, bool* z) {
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

