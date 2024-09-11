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
 * GENERAL POLYGONS
 */

//is pt p inside the (not necessarily convex) polygon given by poly
bool in_poly(pt p, vector<pt>& poly) {
	line l = line(p, {INF, INF * PI}, 1);
	bool ans = false;
	pt lst = poly.back(), tmp;
	for(pt q : poly) {
		line s = line(q, lst, 1); lst = q;
		if(on_line(p, s)) return false; //change if border included
		else if(intsct(l, s, tmp)) ans = !ans;
	}
	return ans;
}

//area of polygon, vertices in order (cw or ccw)
ld area(vector<pt>& poly) {
	ld ans = 0;
	pt lst = poly.back();
	for(pt p : poly) ans += CRS(lst, p), lst = p;
	return abs(ans / 2);
}

//perimeter of polygon, vertices in order (cw or ccw)
ld perim(vector<pt>& poly) {
	ld ans = 0;
	pt lst = poly.back();
	for(pt p : poly) ans += abs(lst - p), lst = p;
	return ans;
}

//centroid of polygon, vertices in order (cw or ccw)
pt centroid(vector<pt>& poly) {
	ld area = 0;
	pt lst = poly.back(), ans = {0, 0};
	for(pt p : poly) {
		area += CRS(lst, p);
		ans += CRS(lst, p) * (lst + p) / (ld)3;
		lst = p;
	}
	return ans / area;
}