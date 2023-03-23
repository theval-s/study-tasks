#include <iostream>
#include <vector>
using namespace std;

void bubble_sort(vector<int>& vec) {
	int n = vec.size();
	for (int i = 0; i < n; i++) {
		short swaps = 0;
		for (int j = 1; j < n - i; j++) {
			if (vec[j - 1] > vec[j]) {
				swaps++;
				swap(vec[j - 1], vec[j]);
			}
		}
		if (swaps == 0) break;
	}
}

int main() {
	vector<int> test = { 5, 27, 1, 18, 54 };
	bubble_sort(test);
	for (auto i : test) cout << i << " ";
	return 0;
}