#ifndef _moller_overlaps_h
#define _moller_overlaps_h

/*! Verify if two triangles overlaps
 * vertices of triangle 1: V0,V1,V2
 * vertices of triangle 2: U0,U1,U2
 * \return -> 1 if the triangles intersect, otherwise 0 */
int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
                     float U0[3],float U1[3],float U2[3]);


#endif


