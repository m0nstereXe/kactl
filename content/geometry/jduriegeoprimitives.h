/**
 * Author: jdurie
 * Date: 2019-10-31
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/geometry/_geom.cc#L744
 * Description: jdurie
 * Time: O(n)
 * Status: Tested on GNYR 2019 Gerrymandering, stress-tested
 */

typedef complex<ld> pt;
struct line {
	pt P, D; bool S = false;
	line(pt p, pt q, bool b = false) : P(p), D(q - p), S(b) {}
	line(pt p, ld th) : P(p), D(polar((ld)1, th)) {}
};
struct circ { pt C; ld R; };

/**
 * builtin pt operations:
 * +, -: usual vector addition/subtraction
 * *, /: scalar multiplication/division (also complex multiplication / division if that's ever useful)
 * abs(p): vector length
 * norm(p): squared vector length
 * arg(p): angle p forms with positive x axis
 * polar(r, th): vector with length r pointing in direction th
 * conj(p): reflects p over x axis
 */

#define X real()
#define Y imag()
#define CRS(a, b) (conj(a) * (b)).Y //scalar cross product
#define DOT(a, b) (conj(a) * (b)).X //dot product
#define U(p) ((p) / abs(p)) //unit vector in direction of p (don't use if Z(p) == true)
#define Z(x) (abs(x) < EPS)
#define A(a) (a).begin(), (a).end() //shortens sort(), upper_bound(), etc. for vectors

//constants (INF and EPS may need to be modified)
ld PI = acosl(-1), INF = 1e20, EPS = 1e-12;
pt I = {0, 1};

/**
 * GENERAL GEOMETRY
 */

//true if d1 and d2 parallel (zero vectors considered parallel to everything)
bool parallel(pt d1, pt d2) { return Z(d1) || Z(d2) || Z(CRS(U(d1), U(d2))); }

//"above" here means if l & p are rotated such that l.D points in the +x direction, then p is above l. Returns arbitrary boolean if p is on l
bool above_line(pt p, line l) { return CRS(p - l.P, l.D) > 0; }

//true if p is on line l
bool on_line(pt p, line l) { return parallel(l.P - p, l.D) && (!l.S || DOT(l.P - p, l.P + l.D - p) <= EPS); }

//returns 0 for no intersection, 2 for infinite intersections, 1 otherwise. p holds intersection pt
ll intsct(line l1, line l2, pt& p) {
	if(parallel(l1.D, l2.D)) //note that two parallel segments sharing one endpoint are considered to have infinite intersections here
		return 2 * (on_line(l1.P, l2) || on_line(l1.P + l1.D, l2) || on_line(l2.P, l1) || on_line(l2.P + l2.D, l1));
	pt q = l1.P + l1.D * CRS(l2.D, l2.P - l1.P) / CRS(l2.D, l1.D);
	if(on_line(q, l1) && on_line(q, l2)) { p = q; return 1; }
	return 0;
}

//closest pt on l to p
pt cl_pt_on_l(pt p, line l) {
	pt q = l.P + DOT(U(l.D), p - l.P) * U(l.D);
	if(on_line(q, l)) return q;
	return abs(p - l.P) < abs(p - l.P - l.D) ? l.P : l.P + l.D;
}

//distance from p to l
ld dist_to(pt p, line l) { return abs(p - cl_pt_on_l(p, l)); }

//p reflected over l
pt refl_pt(pt p, line l) { return conj((p - l.P) / U(l.D)) * U(l.D) + l.P; }

//ray r reflected off l (if no intersection, returns original ray)
line reflect_line(line r, line l) {
	pt p; if(intsct(r, l, p) - 1) return r;
	return line(p, p + INF * (p - refl_pt(r.P, l)), 1);
}