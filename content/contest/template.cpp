#include <bits/stdc++.h>
using namespace std;

// #define rep(i, a, b) for(int i = a; i < (b); ++i)
#define A(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
template<typename T>
ostream_iterator<T> oit(const string &s = " "){ return ostream_iterator<T>(cout,s.c_str()); }
inline auto rep(int l, int r) { return views::iota(min(l, r), r); }
inline auto rep(int n) { return rep(0, n); }
inline auto rep1(int l, int r) { return rep(l, r + 1); }
inline auto rep1(int n) { return rep(1, n + 1); }
inline auto per(int l, int r) { return rep(l, r) | views::reverse; }
inline auto per(int n) { return per(0, n); }
inline auto per1(int l, int r) { return per(l, r + 1); }
inline auto per1(int n) { return per(1, n + 1); }
inline auto len = ranges::ssize;

int main() {
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(cin.failbit);
}
