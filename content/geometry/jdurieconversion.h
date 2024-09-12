/**
 * Author: jdurie
 * Date: 2019-10-31
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/geometry/_geom.cc#L744
 * Description: converts between jdurie and kactl
 * Time: O(n)
 * Status: Tested on GNYR 2019 Gerrymandering, stress-tested
 */

#include "Point.h"

typedef Point<ld> kapt;
kapt ptok(pt p) { return Point<ld>(p.X, p.Y); }
#define ltok(l) ptok(l.P),ptok(l.P+l.D) // Assumes A=>B
argument order . Works on segments and l ines .
#define ctok(c) ptok(c.C),c.R // DOES NOTWORKFOR CIRCLE=TO=
CIRCLE INTERSECTION. ASSUMES C, R ARGUMENTORDER
pt ktopt(Point<ld> p) { return { p.x, p.y }; }