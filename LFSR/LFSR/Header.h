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

//infint difficult(bool* z, int count, int* min_exp);
void sum(bool* a, bool* b, bool* result, int size, int first_point);
void sum_thread(bool* a, bool* b, bool* result, int size, int num_threads = 5);

void multip(bool* a, bool* b, bool* result, int size, int first_point);
void multip_thread(bool* a, bool* b, bool* result, int size, int num_threads = 5);

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
	bool generate(int num_threads = 2);
};

void gen_mult(deque<bool>* polinom, deque<bool>* values, bool* result, int size, int first_point);