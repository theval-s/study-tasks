#include <bits/stdc++.h>
using namespace std;

int main() {
    int q;
    cin >> q;
    map<string, vector<string>> BusInfo, StopInfo;
    for(int z = 0; z < q; z++){
        string s;
        cin >> s;
        if(s == "NEW_BUS"){
            string bus;
            cin >> bus;
            int cnt;
            cin >> cnt;
            for(int i = 0; i < cnt; i++){
                string stop;
                cin >> stop;
                BusInfo[bus].push_back(stop);
                StopInfo[stop].push_back(bus);
            }
        } else if (s == "BUSES_FOR_STOP"){
            string stop;
            cin >> stop;
            if(StopInfo.find(stop) != StopInfo.end()) {
                for (const string& b: StopInfo[stop]) {
                    cout << b << " ";
                }
                cout << endl;
            } else cout << "No stop" << endl;
        } else if (s == "STOPS_FOR_BUS"){
            string bus;
            cin >> bus;
            if(BusInfo.find(bus)!= BusInfo.end()){
                for(const string& stop: BusInfo[bus]){
                    cout << "Stop " << stop << ":";
                    if(StopInfo[stop].size()>1) for(const string& b: StopInfo[stop]){
                        if(b!= bus) cout << " " << b;
                    } else cout << " no interchange";
                    cout << endl;
                }
            } else cout << "No bus" << endl;
        } else if (s == "ALL_BUSES"){
            if(!BusInfo.empty()){
                for(const auto& bus: BusInfo){
                    cout << "Bus " << bus.first << ":";
                    for(const string& stop: bus.second){
                        cout << " " << stop;
                    }
                    cout << endl;
                }
            }  else cout << "No buses" << endl;
        }
    }
    return 0;
}
