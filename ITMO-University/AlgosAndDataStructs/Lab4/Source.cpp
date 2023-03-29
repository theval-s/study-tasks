#include <iostream>
#include <vector>
#include <algorithm>
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
	/*
	vector<int> test = { 5, 27, 1, 18, 54 };
	bubble_sort(test);
	for (auto i : test) cout << i << " ";*/

	int n, m;
	cin >> n >> m;
	vector<pair<int,int>> segm(n);
	for (int i = 0; i < n; i++) {
		int a, b;
		cin >> a >> b;
		segm[i] = make_pair(a, b);
	}
	sort(segm.begin(), segm.end());
	//sort from <algorithm> - nlogn complexity
	//pair are sorted for both first and second values
		for (int i = 0; i < m; i++) {
		int p;
		cin >> p;
		int cnt = 0;
		for (int j = 0; j < n; j++) {
			if (p < segm[j].first) break;
			else if (p > segm[j].second) continue;
			else cnt++;
		}
		cout << cnt << " ";
	}
	return 0;
}