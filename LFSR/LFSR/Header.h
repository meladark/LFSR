#pragma once
#include <iostream>
#include <thread>
#include <io.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <malloc.h>
#include <string>

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

void sum(bool* a, bool* b, bool* result, int size, int first_point);
void sum_thread(bool* a, bool* b, bool* result, int size, int num_threads = 4);

void multip(bool* a, bool* b, bool* result, int size, int first_point);
void multip_thread(bool* a, bool* b, bool* result, int size, int num_threads = 4);

struct infint
{
	deque<bool> b;
	infint(int x) {
		if (x == 0) {
			b.push_back(0); return;
		}
		for (int i(0); i < sizeof(x) * 8; i++) {
			b.push_back((x >> i) & 1u);
		}
		while (b.back() != true) {
			b.pop_back();
		}
	};
	void print();
	int operator[](int x);
	infint operator<<(int x);
	infint operator^(infint x);
	void operator=(infint x);
};

struct gen_lfsr {
	deque<bool>* values;
	deque<bool>* polinom;
	gen_lfsr(deque<bool>* polinom, deque<bool>* values) { //
		this->values = values;
		this->polinom = polinom;
		/*for (int i(0); i < polinom->size(); i++) {
			if (polinom->at(i)) {
				std::cout << "x^" << polinom->size() - i - 1 << " + "; logging("+x^"); logging(i);
			}
		}
		std::cout << "1"; logging("1");*/
	}
	~gen_lfsr() {
		delete[] values;
		delete[] polinom;
	}
public:
	bool generate(int num_threads = 1);
};

void gen_mult(deque<bool>* polinom, deque<bool>* values, bool* result, int size, int first_point);
infint difficult(bool* z, int count, int* min_exp);

class still_lfsr {
public:
	int polynom = 0;
	int* in_vec_value = NULL; //input vector value
	bool* generate_vec = new bool[1];
	infint min_polynom = 0;
	int* min_exp = new int[1];
	int exp = 0;
	gen_lfsr* gn;
	still_lfsr(deque<bool>* polynom = NULL, deque<bool>* in_vec_value = NULL) {
		exp = polynom->size();
		/*cout << "Вывод still_lfsr !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		for (int i(0); i < polynom->size(); i++) 
			cout << polynom->at(i);
		cout << endl;*/
		gn = new gen_lfsr(polynom, in_vec_value);

		generate_vec = new bool[2 * exp + 1];
		for (int i = 0; i < 2 * exp + 1; i++) {
			generate_vec[i] = gn->generate();
			/*cout << " Сгенерированное значение " << generate_vec[i];
			logging(" Сгенерированное значение ");
			logging(generate_vec[i]);*/
		}
		min_polynom = difficult(generate_vec, 2 * exp + 1, min_exp);
	}
	void gen(int num);
	still_lfsr* operator+ (still_lfsr right);
	still_lfsr* operator* (still_lfsr right);
};

void operations(vector<still_lfsr*>* all_lfsr, deque<bool>* combination, int num_threads = 2);

void read_hex_(const char* file_txt, vector<still_lfsr*>* all_lfsr, deque<bool>* combination, bool thrb = true);
void thr(still_lfsr** all_lfsr, int i, deque<bool>* lf, deque<bool>* mas);
void sub_fun(int r, unsigned long long delta, infint c, infint b, unsigned long long L, bool* z);

