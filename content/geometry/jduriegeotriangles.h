/**
 * Author: jdurie
 * Date: 2019-10-31
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/geometry/_geom.cc#L744
 * Description: jdurie
 * Time: O(n)
 * Status: Tested on GNYR 2019 Gerrymandering, stress-tested
 */

/**
 * TRIANGLE CENTERS
 */

//altitude from p to l
line alt(pt p, line l) { l.S = 0; return line(p, cl_pt_on_l(p, l)); }

//angle bisector of angle abc
line ang_bis(pt a, pt b, pt c) { return line(b, b + INF * (U(a - b) + U(c - b)), 1); }

//perpendicular bisector of l (assumes l.S == 1)
line perp_bis(line l) { return line(l.P + l.D / (ld)2, arg(l.D * I)); }

//orthocenter of triangle abc
pt orthocent(pt a, pt b, pt c) { pt p; intsct(alt(a, line(b, c)), alt(b, line(a, c)), p); return p; }

//incircle of triangle abc
circ incirc(pt a, pt b, pt c) {
	pt cent; intsct(ang_bis(a, b, c), ang_bis(b, a, c), cent);
	return {cent, dist_to(cent, line(a, b))};
}

//circumcircle of triangle abc
circ circumcirc(pt a, pt b, pt c) {
	pt cent; intsct(perp_bis(line(a, b, 1)), perp_bis(line(a, c, 1)), cent);
	return {cent, abs(cent - a)};
}