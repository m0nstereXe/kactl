/**
 * Author: among us 
 * Date: recent 
 * License: ??
 * Source: ??
 * Description: ??
 * Time: ???
 * Status: ???
 */
namespace d2d {
    using key_t = int;
    using T = int;
    
    T id = INT_MAX;
    T f(T a, T b) {return min(a,b);}
    
    struct seg {
        vector<T> t;
        vector<key_t> rr;
        #define rein(x) (lower_bound(A(rr), x) - rr.begin() + rr.size())
        void modify(key_t pin, T value) {  // set value at position p
            int p = rein(pin);
            // assert(p != 2 * rr.size() and rr[p - rr.size()] == pin);
            for (t[p] = f(t[p], value); p /= 2;) t[p] = f(t[2*p], t[2*p+1]);
        }
    
        T query(key_t lin, key_t rin) { // fold f on interval [l, r)
            T resl=id, resr=id;
            for (int l = rein(lin), r = rein(rin); l < r; l /= 2, r /= 2) {
                if (l&1) resl = f(resl, t[l++]);
                if (r&1) resr = f(t[--r], resr);
            }
            return f(resl, resr);
        }
        #undef rein
    } t[2*N];

// setup functions
    void build() { 
        F(i, 0, N*2) {
            auto &rr = t[i].rr;
            sort(A(rr));
            rr.erase(unique(A(rr)), rr.end());
            t[i].t.assign(2*rr.size(), id);
        }
    }

    void pre_modify(int p, key_t k) {
        for (p+=N, t[p].rr.push_back(k); p /= 2;) t[p].rr.push_back(k);
    }

// actual functions
    void modify(int p, key_t k, int v) { 
        for (p+=N, t[p].modify(k, v); p /= 2;) t[p].modify(k, v);
    }

    T query(int l, int r, key_t l2, key_t r2) { // queries f(..[l2, r2)..) for all segtrees [l, r)
        T res = id;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(l & 1) res = f(t[l++].query(l2, r2), res);
            if(r & 1) res = f(res, t[--r].query(l2, r2));
        }
        return res;
    }
}