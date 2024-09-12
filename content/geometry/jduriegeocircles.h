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
 * CIRCLES
 */

//invert a point over a circle (doesn't work for center of circle)
pt circInv(pt p, circ c) {
    return c.R * c.R / conj(p - c.C) + c.C;
}

//vector of intersection pts of two circs (up to 2) (if circles same, returns empty vector)
vector<pt> intsctCC(circ c1, circ c2) {
	if(c1.R < c2.R) swap(c1, c2);
	pt d = c2.C - c1.C;
	if(Z(abs(d) - c1.R - c2.R)) return {c1.C + polar(c1.R, arg(c2.C - c1.C))};
	if(!Z(d) && Z(abs(d) - c1.R + c2.R)) return {c1.C + c1.R * U(d)}; 
	if(abs(abs(d) - c1.R) >= c2.R - EPS) return {};
	ld th = acosl((c1.R * c1.R + norm(d) - c2.R * c2.R) / (2 * c1.R * abs(d)));
	return {c1.C + polar(c1.R, arg(d) + th), c1.C + polar(c1.R, arg(d) - th)};
}

//vector of intersection pts of a line and a circ (up to 2)
vector<pt> intsctCL(circ c, line l) {
	vector<pt> v, ans;
	if(parallel(l.D, c.C - l.P)) v = {c.C + c.R * U(l.D), c.C - c.R * U(l.D)};
	else v = intsctCC(c, circ{refl_pt(c.C, l), c.R});
	for(pt p : v) if(on_line(p, l)) ans.push_back(p);
	return ans;
}

//external tangents of two circles (negate c2.R for internal tangents)
vector<line> circTangents(circ c1, circ c2) {
	pt d = c2.C - c1.C;
	ld dr = c1.R - c2.R, d2 = norm(d), h2 = d2 - dr * dr;
	if(Z(d2) || h2 < 0) return {};
	vector<line> ans;
	for(ld sg : {-1, 1}) {
		pt u = (d * dr + d * I * sqrt(h2) * sg) / d2;
		ans.push_back(line(c1.C + u * c1.R, c2.C + u * c2.R, 1));
	}
	if(Z(h2)) ans.pop_back();
	return ans;
}