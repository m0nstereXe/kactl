/**
 * Author: Aeren1564
 * Date: lmao
 * Source: https://github.com/Aeren1564/Algorithms/blob/master/Algorithm_Implementations_Cpp/Data_Structure/Wavelet_Structures/wavelet_tree.sublime-snippet
 * Description: wavelets
 * Time: $O(26N)$
 * Status: stress-tested a bit
 */
#pragma once

template<bool HAS_QUERY, class B, class T, class F, class I>
struct wavelet_tree_base{
	int n, lg;
	B sigma;
	vector<vector<int>> data;
	vector<vector<T>> aggregate;
	F TT; // commutative group operation
	T T_id; // commutative group identity
	I Tinv; // commutative group inverse
	wavelet_tree_base(F TT, T T_id, I Tinv): TT(TT), T_id(T_id), Tinv(Tinv){ }
	wavelet_tree_base &operator=(const wavelet_tree_base &wt){
		n = wt.n;
		lg = wt.lg;
		sigma = wt.sigma;
		data = wt.data;
		return *this;
	}
	// O(n * log(sigma)) time and O(n * log(sigma)) memory
	void build(const vector<B> &key, B sigma){
		static_assert(!HAS_QUERY);
		assert(sigma > 0);
		for(auto x: key) assert(0 <= x && x < sigma);
		n = (int)key.size();
		this->sigma = sigma;
		lg = __lg(sigma) + (B(1) << lg != sigma) + 1;
		data.assign(lg, vector<int>(n + 1));
		vector<B> cur = key, next(n);
		for(auto h = lg; h --;){
			for(auto i = 0; i < n; ++ i) data[h][i + 1] = data[h][i] + (~cur[i] >> h & 1);
			array it{next.begin(), next.begin() + data[h][n]};
			for(auto i = 0; i < n; ++ i) *it[data[h][i] == data[h][i + 1]] ++ = cur[i];
			swap(cur, next);
		}
	}
	// O(n * log(sigma)) time and O(n * log(sigma)) memory
	template<class U>
	void build(const vector<B> &key, B sigma, const vector<U> &value){
		static_assert(HAS_QUERY);
		assert(sigma > 0);
		for(auto x: key) assert(0 <= x && x < sigma);
		n = (int)key.size();
		this->sigma = sigma;
		lg = __lg(sigma) + (B(1) << lg != sigma) + 1;
		data.assign(lg, vector<int>(n + 1));
		aggregate.assign(lg + 1, vector<T>(n + 1, T_id));
		vector<pair<B, T>> cur(n), next(n);
		for(auto i = 0; i < n; ++ i) cur[i] = {key[i], value[i]};
		for(auto h = lg; h --;){
			for(auto i = 0; i < n; ++ i) data[h][i + 1] = data[h][i] + (~cur[i].first >> h & 1);
			array it{next.begin(), next.begin() + data[h][n]};
			for(auto i = 0; i < n; ++ i){
				*it[data[h][i] == data[h][i + 1]] ++ = cur[i];
				aggregate[h + 1][i + 1] = data[h][i] == data[h][i + 1] ? aggregate[h + 1][i] : TT(aggregate[h + 1][i], cur[i].second);
			}
			swap(cur, next);
		}
		for(auto i = 0; i < n; ++ i) aggregate[0][i + 1] = TT(aggregate[0][i], cur[i].second);
	}
	// Returns the frequency of x in the interval [ql, qr)
	// O(log(sigma))
	int freq(int ql, int qr, int x) const{
		assert(0 <= ql && ql <= qr && qr <= n);
		assert(0 <= x);
		if(ql == qr || sigma <= x) return 0;
		for(auto h = lg; h --; ){
			if(~x >> h & 1) ql = data[h][ql], qr = data[h][qr];
			else ql += data[h][n] - data[h][ql], qr += data[h][n] - data[h][qr];
		}
		return qr - ql;
	}
	// Returns the frequency of x in the interval [ql, qr), along with the sum of their values
	// O(log(sigma))
	pair<int, T> freq_query(int ql, int qr, int x) const{
		static_assert(HAS_QUERY);
		assert(0 <= ql && ql <= qr && qr <= n);
		assert(0 <= x);
		if(ql == qr || sigma <= x) return {0, T_id};
		for(auto h = lg; h --; ){
			if(~x >> h & 1) ql = data[h][ql], qr = data[h][qr];
			else ql += data[h][n] - data[h][ql], qr += data[h][n] - data[h][qr];
		}
		return {qr - ql, TT(aggregate[0][qr], Tinv(aggregate[0][ql]))};
	}
	// Returns the number of occurrences of numbers in [0, xr) in the interval [ql, qr)
	// O(log(sigma))
	int count(int ql, int qr, B xr) const{
		assert(0 <= ql && ql <= qr && qr <= n);
		assert(0 <= xr);
		if(sigma <= xr) return qr - ql;
		if(xr == 0) return 0;
		int cnt = 0;
		for(auto h = lg; h --; ){
			if(~xr >> h & 1) ql = data[h][ql], qr = data[h][qr];
			else{
				cnt += data[h][qr] - data[h][ql];
				ql += data[h][n] - data[h][ql], qr += data[h][n] - data[h][qr];
			}
		}
		return cnt;
	}
	// Returns the number of occurrences of numbers in [0, xr) in the interval [ql, qr), along with the sum of their values
	// O(log(sigma))
	pair<int, T> count_query(int ql, int qr, B xr) const{
		static_assert(HAS_QUERY);
		assert(0 <= ql && ql <= qr && qr <= n);
		assert(0 <= xr);
		if(xr == 0) return {0, T_id};
		xr = min(sigma, xr);
		int cnt = 0;
		T sum = T_id;
		for(auto h = lg; h --; ){
			if(~xr >> h & 1) ql = data[h][ql], qr = data[h][qr];
			else{
				cnt += data[h][qr] - data[h][ql];
				sum = TT(sum, TT(aggregate[h + 1][qr], Tinv(aggregate[h + 1][ql])));
				ql += data[h][n] - data[h][ql], qr += data[h][n] - data[h][qr];
			}
		}
		return {cnt, sum};
	}
	// Returns the number of occurrences of numbers in [xl, xr) in the interval [ql, qr)
	// O(log(sigma))
	int count(int ql, int qr, B xl, B xr) const{
		assert(xl <= xr);
		if(xl == xr) return 0;
		return count(ql, qr, xr) - count(ql, qr, xl);
	}
	// Returns the number of occurrences of numbers in [xl, xr) in the interval [ql, qr), along with the sum of their values
	// O(log(sigma))
	pair<int, T> count_query(int ql, int qr, B xl, B xr) const{
		static_assert(HAS_QUERY);
		assert(xl <= xr);
		if(xl == xr) return {0, T_id};
		auto [lcnt, lsum] = count_query(ql, qr, xl);
		auto [rcnt, rsum] = count_query(ql, qr, xr);
		return {rcnt - lcnt, TT(rsum, Tinv(lsum))};
	}
	// Find the k-th smallest element in the interval [ql, qr), sigma if no such element
	// O(log(sigma))
	B find_by_order(int ql, int qr, int k) const{
		assert(0 <= k);
		if(k >= qr - ql) return sigma;
		B x = 0;
		for(auto h = lg; h --; ){
			if(k < data[h][qr] - data[h][ql]) ql = data[h][ql], qr = data[h][qr];
			else{
				k -= data[h][qr] - data[h][ql];
				x |= (B)1 << h;
				ql += data[h][n] - data[h][ql], qr += data[h][n] - data[h][qr];
			}
		}
		return x;
	}
	// Find the k-th smallest element in the interval [ql, qr), sigma if no such element, along with the sum of values of the k smallest elements (prioritizing smaller index)
	// O(log(sigma))
	pair<B, T> find_by_order_query(int ql, int qr, int k) const{
		assert(0 <= k);
		k = min(k, qr - ql);
		B x = 0;
		T sum = T_id;
		for(auto h = lg; h --; ){
			if(k < data[h][qr] - data[h][ql]) ql = data[h][ql], qr = data[h][qr];
			else {
				k -= data[h][qr] - data[h][ql];
				x |= (B)1 << h;
				sum = TT(sum, TT(aggregate[h + 1][qr], Tinv(aggregate[h + 1][ql])));
				ql += data[h][n] - data[h][ql], qr += data[h][n] - data[h][qr];
			}
		}
		return {x, TT(sum, TT(aggregate[0][ql + k], Tinv(aggregate[0][ql])))};
	}
	// Find the k-th smallest element in the interval [ql, qr) among elements >= xl, sigma if no such element
	// O(log(sigma))
	B find_by_order(int ql, int qr, B xl, int k) const{
		assert(0 <= ql && ql <= qr && qr <= n);
		assert(0 <= xl && 0 <= k);
		if(xl >= sigma) return sigma;
		k += count(ql, qr, 0, xl);
		if(k >= qr - ql) return sigma;
		return find_by_order(ql, qr, k);
	}
	// Find the k-th smallest element in the interval [ql, qr) among elements >= xl, sigma if no such element, along with the sum of values of the k smallest elements (prioritizing smaller index)
	// O(log(sigma))
	pair<B, T> find_by_order_query(int ql, int qr, B xl, int k) const{
		assert(0 <= ql && ql <= qr && qr <= n);
		assert(0 <= xl && 0 <= k);
		if(xl >= sigma) return {sigma, T_id};
		auto [cnt, sum] = count_query(ql, qr, 0, xl);
		k += cnt;
		auto [x, sum2] = find_by_order_query(ql, qr, k);
		return {x, TT(sum2, Tinv(sum))};
	}
	// Find the smallest element >= x, sigma if no such element
	// O(log(sigma))
	B lower_bound(int ql, int qr, B x) const{
		assert(0 <= x);
		return find_by_order(ql, qr, x, 0);
	}
	// Find the smallest element > x, sigma if no such element
	// O(log(sigma))
	B upper_bound(int ql, int qr, B x) const{
		assert(0 <= x);
		return find_by_order(ql, qr, x + 1, 0);
	}
	// Find the largest element <= x, -1 if no such element
	// O(log(sigma))
	B reverse_lower_bound(int ql, int qr, B x) const{
		assert(0 <= x);
		int cnt = count(ql, qr, x);
		return cnt ? find_by_order(ql, qr, cnt - 1) : -1;
	}
	// Find the largest element < x, -1 if no such element
	// O(log(sigma))
	B reverse_upper_bound(int ql, int qr, B x) const{
		assert(0 <= x);
		int cnt = count(ql, qr, x + 1);
		return cnt ? find_by_order(ql, qr, cnt - 1) : -1;
	}
};

template<class B>
auto make_wavelet_tree(){
	return wavelet_tree_base<false, B, int, plus<>, negate<>>(plus<>(), 0, negate<>());
}
// Supports query
template<class B, class T = long long, class F = plus<>, class I = negate<>>
auto make_Q_wavelet_tree(F TT = plus<>(), T T_id = 0, I Tinv = negate<>()){
	return wavelet_tree_base<true, B, T, F, I>(TT, T_id, Tinv);
}