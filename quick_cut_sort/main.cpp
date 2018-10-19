

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

// system utils
void print(int a[], int len) {
	for (int i = 0; i < len; ++i) {
		std::cout << a[i] << ", ";
	}
	std::cout << std::endl;
}
int64_t get_time_usec() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

// array utils
void rand_ary(int a[], int len) {
	for (int i = 0; i < len; ++i) {
		a[i] = (rand() << 8) ^ rand();
	}
}
int get_max_order(int a[], int len) {
	for (int ord = 1, start = a[0]; ord < len; ++ord) {
		if (a[ord] > start) {
			return ord;
		}
		start = a[ord];
	}
	return len;
}

// algorithm
#define EXC(_T_, _v1_, _v2_) { \
	_T_ tmp = _v1_; \
	_v1_ = _v2_; \
	_v2_ = tmp; \
}
int partition(int a[], int beg, int end) {
	int div = a[end - 1];
	int pend = beg, pdiv = beg;
	while (pend < end - 1) {
		if (a[pend] > div) {
			EXC(int, a[pend], a[pdiv]);
			++pdiv;
		}
		++pend;
	}

	a[pend] = a[pdiv];
	a[pdiv] = div;
	return pdiv;
}
void quick_sort(int a[], int beg, int end) {
	if (beg + 1 < end) {
		int pdiv = partition(a, beg, end);
		quick_sort(a, beg, pdiv);
		quick_sort(a, pdiv + 1, end);
	}
}
void quick_sort(int a[], int beg, int end, int cut_beg, int cut_end) {
	if ((cut_beg <= beg && beg < cut_end) || 
		(cut_beg < end && end <= cut_end) || 
		(beg <= cut_beg && cut_end <= end)) {
		if (beg + 1 < end) {
			int pdiv = partition(a, beg, end);
			quick_sort(a, beg, pdiv, cut_beg, cut_end);
			quick_sort(a, pdiv + 1, end, cut_beg, cut_end);
		}
		/*else if (beg + 2 == end) {
			if (a[beg] < a[end]) {
				EXC(int, a[beg], a[beg + 1]);
			}
		}*/
	}
}

// test
bool test(int a[], int len, int cut_beg, int cut_end, int64_t *time_cost, int *ord) {
	rand_ary(a, len);
	
	int64_t start_time = get_time_usec();
	quick_sort(a, 0, len, cut_beg, cut_end);
	*time_cost = get_time_usec() - start_time;
	
	if ((*ord = get_max_order(a + cut_beg, len - cut_beg)) + cut_beg < cut_end) {
		return false;
	}
	return true;
}
void test(int a[], int len, int time, int cut_beg, int cut_end) {
	int64_t time_cost_sum = 0;
	int64_t ord_sum = 0;
	for (int i = 0; i < time; ++i) {
		int64_t time_cost;
		int ord;
		if (! test(a, len, cut_beg, cut_end, &time_cost, &ord)) {
			std::cout << "fail" << std::endl;
			sleep(2);
			exit(0);
		}
		time_cost_sum += time_cost;
		ord_sum += (int64_t)ord;
	}

	std::cout << (cut_end - cut_beg) << "\t" 
		<< len << "\t" 
		<< time_cost_sum << "\t" 
		<< ((double)(cut_end - cut_beg) / (double)len) << "\t" 
		<< ((double)ord_sum / (double)((int64_t)time * (int64_t)len)) << std::endl;
	//std::cerr << (cut_end - cut_beg) << "\t" 
	//	<< len << "\t" 
	//	<< time_cost_sum << "\t" 
	//	<< ((double)(cut_end - cut_beg) / (double)len) << "\t" 
	//	<< ((double)ord_sum / (double)((int64_t)time * (int64_t)len)) << std::endl;
}

int main() {
	const int len = 1024 * 64;
	const int time = 256;
	int a[len];

	for (int l = 128; l < len; l += 128) {
		for (int cut = 64; cut < l; cut += 64) {
			test(a, l, time, 0, cut);
		}
		std::cout << std::endl;
	}
	
	return 0;
}

