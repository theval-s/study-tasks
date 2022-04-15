#include <bits/stdc++.h>
using namespace std;

int main() {
    int q;
    cin >> q;
    map<string, string> CountryMap;
    for (int i = 0; i < q; i++) {
        string s;
        cin >> s;
        if (s == "CHANGE_CAPITAL") {
            string country, capital;
            cin >> country >> capital;
            if (CountryMap[country] == "") {
                cout << "Introduce new country " << country << " with capital " << capital << endl;
            } else if (CountryMap[country] != capital) {
                cout << "Country " << country << " has changed its capital from " << CountryMap[country] << " to "
                     << capital << endl;
            } else cout << "Country " << country << " hasn't changed its capital" << endl;
            CountryMap[country] = capital;
        } else if (s == "RENAME") {
            string oldc, newc;
            cin >> oldc >> newc;
            if (oldc != newc && CountryMap.find(oldc) != CountryMap.end() && CountryMap.find(newc) == CountryMap.end()) {
                cout << "Country " << oldc << " with capital " << CountryMap[oldc] << " has been renamed to " << newc << endl;
                CountryMap[newc] = CountryMap[oldc];
                CountryMap.erase(oldc);
            } else cout << "Incorrect rename, skip" << endl;
        } else if (s == "ABOUT") {
            string country;
            cin >> country;
            if(CountryMap.find(country) != CountryMap.end()) cout << "Country " << country << " has capital " << CountryMap[country] << endl;
            else cout << "Country " << country << " doesn't exist" << endl;
        } else if (s == "DUMP") {
            if(CountryMap.size() > 0) {
                for (auto m: CountryMap) cout << m.first << "/" << m.second << " ";
                cout << endl;
            }
            else cout << "There are no countries in the world" << endl;
        }
    }
    return 0;
}
