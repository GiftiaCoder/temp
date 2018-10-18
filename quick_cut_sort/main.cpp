

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

void print(int a[], int len) {
	for (int i = 0; i < len; ++i) {
		std::cout << a[i] << ", ";
	}
	std::cout << std::endl;
}

int partition(int a[], int beg, int end) {
	int div = a[end - 1];
	int pend = beg, pdiv = beg;
	while (pend < end - 1) {
		if (a[pend] > div) {
			int tmp = a[pend];
			a[pend] = a[pdiv];
			a[pdiv] = tmp;

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

void quick_sort(int a[], int beg, int end, int cut) {
	if (cut > beg && beg + 2 < end) {
		int pdiv = partition(a, beg, end);
		quick_sort(a, beg, pdiv);
		if (pdiv + 1 < cut) {
			quick_sort(a, pdiv + 1, end);
		}
	}
}

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

int test(int a[], int len, int cut) {
	rand_ary(a, len);
	quick_sort(a, 0, len, cut);
	return get_max_order(a, len);
}

long long get_time_usec() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}
void test(int a[], int len, int cut, int time) {
	int ordsum = 0, i = 0;

	long long start_time = get_time_usec();
	for (; i < time; ++i) {
		int ord = test(a, len, cut);
		if (ord < cut) {
			std::cout << "fail: " << ord << std::endl;
			return;
		}
		else {
			ordsum += ord;
		}
	}
	long long time_cost = get_time_usec() - start_time;
	std::cout << cut << "\t" << len << "\t" << time_cost << "\t" << (double)(cut) / (double)(len) << "\t" << (double)(ordsum) / (double)(time * len) << std::endl;
	std::cerr << cut << "\t" << len << "\t" << time_cost << "\t" << (double)(cut) / (double)(len) << "\t" << (double)(ordsum) / (double)(time * len) << std::endl;
}

int main() {
	const int len = 1024 * 16;
	const int time = 1024 * 1;
	int a[len];

	for (int cut = 32; cut < len; cut += 32) {
		test(a, len, cut, time);
	}
	return 0;
}

