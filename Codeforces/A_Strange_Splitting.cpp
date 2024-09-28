#include<bits/stdc++.h>
using namespace std;

#define print(a) for(auto x:a)cout<<x<<' ';cout<<'\n';
#define debug(x) cout<<#x<<" "<<x<<'\n'
#define int   long long int
 
const int M = 1e9 + 7;
const int N = 2e5 + 10;

void solve(){
    int n; cin >> n;
    vector<int> a(n);

    for(int i = 0; i < n; i++){
        cin >> a[i];
    }

    int cnt = count(a.begin(),a.end(),a[0]);
    if(cnt == n){
        cout << "NO\n";
        return;
    }
    string s = "";
    for(int i = 0; i < n; i++){
        s += 'B';
    }
    if(cnt == 1)s.back() = 'R';
    else s[0] = 'R';
    cout << "YES\n";
    cout << s << '\n';


}

signed main() {
   ios_base::sync_with_stdio (0);
   cin.tie (0);

   int t = 1;   cin >> t;
   for (int tc = 1; tc <= t; tc++) {
      //cout<<"Case "<<tc<<": ";
      solve();
   }
   return 0;
}