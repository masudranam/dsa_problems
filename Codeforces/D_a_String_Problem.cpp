#include<bits/stdc++.h>
using namespace std;

#define print(a) for(auto x:a)cout<<x<<' ';cout<<'\n';
#define debug(x) cout<<#x<<" "<<x<<'\n'
#define int   long long int
 
const int M = 1e9 + 7;
const int N = 2e5 + 10;

struct Hash {
    string  s;
    const int p = 397, p1 = 313;
    int len;
    vector<int> pw1, pw, hF1, hF, hR1, hR;

    Hash(string s1) {
        s=s1;
        this->len = s.size();
        pw = hF = hR = vector<int>(len+5,0);
        pw1 = hF1 = hR1 = vector<int>(len+5,0);
    }

    void Calc() {
        pw[0]=1;
        hF[0] = hR[len+1] = 0;
        for(int i=1; i<=len; i++) {
            pw[i]=(pw[i-1]*p)%M;
        }
        for(int i=0; i<len; i++) {
            hF[i+1] = (hF[i] * p + (s[i]) ) % M;
            hR[len-i] = (hR[len-i+1] * p + (s[len-i-1])) % M;
        }
    }

    int hashF(int l,int r) {
        int val = hF[r] - (hF[l - 1] * pw[r - l + 1]) % M;
        if(val<0)val+=M;
        return val;
    }

    int hashR(int l,int r) {
        int val = hR[l] - (hR[r + 1] * pw[r - l + 1]) % M;
        if(val<0)val+=M;
        return val;
    }

    bool isPalin(int l,int r) {
        if(r<l)return false;
        return (hashF(l, r) == hashR(l, r));
    }

    void Calc1() {
        pw1[0] = 1;
        hF1[0] = hR1[len + 1] = 0;
        pw[0] = 1;
        hF[0] = hR[len + 1] = 0;

        for (int i = 1; i <= len; i++) {
            pw1[i] = (pw1[i - 1] * p1) % M;
            pw[i] = (pw[i - 1] * p) % M;
        }
        for (int i = 0; i < len; i++) {
            hF1[i + 1] = (hF1[i] * p1 + (s[i])) % M;
            hF[i + 1] = (hF[i] * p + s[i]) % M;
            hR1[len - i] = (hR1[len - i + 1] * p1 + (s[len - i - 1])) % M;
            hR[len - i] = (hR[len - i + 1] * p + (s[len - i - 1])) % M;
        }
    }

    array<int, 2> hashF1 (int l, int r) {
        int val1 = hF1[r] - (hF1[l - 1] * pw1[r - l + 1]) % M;
        int val2 = hF[r] - (hF[l - 1] * pw[r - l + 1]) % M;
        if (val1 < 0) val1 += M;
        if (val2 < 0) val2 += M;
        return {val1, val2};
    }

    array<int, 2> hashR1 (int l, int r) {
        int val1 = hR1[l] - (hR1[r + 1] * pw1[r - l + 1]) % M;
        int val2 = hR[l] - (hR[r + 1] * pw[r - l + 1]) % M;
        if (val1 < 0) val1 += M;
        if (val2 < 0) val2 += M;
        return {val1, val2};
    }

    bool ok(int a, int b, int c, int d){
        return hashF1(a+1,b+1) == hashF1(c+1,d+1);
    }

    bool isPalin1(int l,int r) {
        if(r<l)return false;
        return (hashF1(l, r) == hashR1(l, r));
    }
};

void solve(){
    string s; cin >> s;
    Hash h(s); h.Calc1();
    int n = s.size();

    if(count(s.begin(),s.end(),'a') == n){
        cout << s.size() - 1 << '\n'; return;
    }
    vector<int> p;
    for(int i = 0; i < n; i++){
        if(s[i] != 'a')p.push_back(i);
    }
    int x = p.size(), ans = 0;

    for(int d = 1; d <= x; d++){
        if(x % d)continue;
        bool ok = 1;
        int mn = n;
        for(int i = d-1; i < x; i += d){
            //cout << p[0] <<' '<< p[d-1] <<' '<<p[i-d+1]<<' '<<p[i] << '\n';
            if(!h.ok(p[0],p[d-1],p[i-d+1],p[i])){
                ok = 0;break;
            }
            if(i+1 < x)mn = min(mn, p[i+1] - p[i] - 1);
        }

        if(!ok)continue;
       //debug(p.size());debug(d);debug(mn);
        for(int j = p.back(), cnt = 0; j < n; j++, cnt++){
            ans += max(0ll, min(mn - cnt+1, p[0] + 1));
        }
    }

    cout << ans << '\n';
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
