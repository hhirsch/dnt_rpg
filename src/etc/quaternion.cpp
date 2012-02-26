/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
  
  Originally based on the work of David Henry <tfc_duke@club-internet.fr>
  Copyright (c) 2005-2007 David HENRY
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "quaternion.h"

#include <math.h>

/***********************************************************************
 *                              operator+                              *
 ***********************************************************************/
vec3_t vec3_t::operator+(const vec3_t& v) const
{
   vec3_t out;

   out.x = x + v.x;
   out.y = y + v.y;
   out.z = z + v.z;

   return(out);
}

/***********************************************************************
 *                              operator-                              *
 ***********************************************************************/
vec3_t vec3_t::operator-(const vec3_t& v) const
{
   vec3_t out;

   out.x = x - v.x;
   out.y = y - v.y;
   out.z = z - v.z;

   return(out);
}

vec3_t vec3_t::operator*(float num) const
{
   vec3_t out;
   out.x = x * num;
   out.y = y * num;
   out.z = z * num;

   return(out);
}

/***********************************************************************
 *                                cross                                *
 ***********************************************************************/
vec3_t vec3_t::cross(const vec3_t& v) const
{
   vec3_t out;

   out.x = (y * v.z) - (z * v.y);
   out.y = (z * v.x) - (x * v.z);
   out.z =  (x * v.y) - (y * v.x);

   return(out);
}

/***********************************************************************
 *                               normalize                             *
 ***********************************************************************/
void vec3_t::normalize()
{
   /* compute magnitude */
   float mag = sqrt ((x * x) + (y * y) + (z * z) );

   /* check for bogus length, to protect against divide by zero */
   if (mag > 0.0f)
   {
      /* normalize it */
      float oneOverMag = 1.0f / mag;

      x *= oneOverMag;
      y *= oneOverMag;
      z *= oneOverMag;
   }
}


/***********************************************************************
 *                               computeW                              *
 ***********************************************************************/
void quaternion::computeW()
{
   float t = 1.0f - (x * x) - (y * y) - (z * z);

   if (t < 0.0f)
   {
      w = 0.0f;
   }
   else
   {
      w = -sqrt(t);
   }
}

/***********************************************************************
 *                               normalize                             *
 ***********************************************************************/
void quaternion::normalize()
{
   /* compute magnitude of the uaternion */
   float mag = sqrt ((x * x) + (y * y) + (z * z) + (w * w));

   /* check for bogus length, to protect against divide by zero */
   if (mag > 0.0f)
   {
      /* normalize it */
      float oneOverMag = 1.0f / mag;

      x *= oneOverMag;
      y *= oneOverMag;
      z *= oneOverMag;
      w *= oneOverMag;
   }
}

/***********************************************************************
 *                             operator*                               *
 ***********************************************************************/
quaternion quaternion::operator* (const quaternion& qb) const
{
   quaternion out;

   out.w = (w * qb.w) - (x * qb.x) - (y * qb.y) - (z * qb.z);
   out.x = (x * qb.w) + (w * qb.x) + (y * qb.z) - (z * qb.y);
   out.y = (y * qb.w) + (w * qb.y) + (z * qb.x) - (x * qb.z);
   out.z = (z * qb.w) + (w * qb.z) + (x * qb.y) - (y * qb.x);

   return(out);
}

/***********************************************************************
 *                             operator*                               *
 ***********************************************************************/
quaternion quaternion::operator* (const vec3_t& v) const
{
   quaternion out;

   out.w = - (x * v.x) - (y * v.y) - (z * v.z);
   out.x =   (w * v.x) + (y * v.z) - (z * v.y);
   out.y =   (w * v.y) + (z * v.x) - (x * v.z);
   out.z =   (w * v.z) + (x * v.y) - (y * v.x);

   return(out);
}

/***********************************************************************
 *                            rotatePoint                              *
 ***********************************************************************/
vec3_t quaternion::rotatePoint(const vec3_t& in) const
{
   quaternion inv, final;
   vec3_t out;

   inv.x = -x; inv.y = -y;
   inv.z = -z; inv.w =  w;
   inv.normalize();

   final = ((*this)*in)*inv;

   out.x = final.x;
   out.y = final.y;
   out.z = final.z;

   return(out);
}

/***********************************************************************
 *                                dot                                  *
 ***********************************************************************/
float quaternion::dot(const quaternion& qb) const
{
   return((x * qb.x) + (y * qb.y) + (z * qb.z) + (w * qb.w));
}


/***********************************************************************
 *                               slerp                                 *
 ***********************************************************************/
quaternion quaternion::slerp(const quaternion& qb, float t) const
{
   /* Check for out-of range parameter and return edge points if so */
   if(t <= 0.0f)
   {
      return(*this);
   }

   if(t >= 1.0f)
   {
      return(qb);
   }

   /* Compute "cosine of angle between quaternions" using dot product */
   float cosOmega = dot(qb);

   /* If negative dot, use -q1.  Two quaternions q and -q
      represent the same rotation, but may produce
      different slerp.  We chose q or -q to rotate using
      the acute angle. */
   float q1w = qb.w;
   float q1x = qb.x;
   float q1y = qb.y;
   float q1z = qb.z;

   if(cosOmega < 0.0f)
   {
      q1w = -q1w;
      q1x = -q1x;
      q1y = -q1y;
      q1z = -q1z;
      cosOmega = -cosOmega;
   }

   /* We should have two unit quaternions, so dot should be <= 1.0 */
   if(cosOmega >= 1.1f)
   {
      /* ERROR! TODO: print error message! */
      return(qb);
   }

   /* Compute interpolation fraction, checking for quaternions
      almost exactly the same */
   float k0, k1;

   if (cosOmega > 0.9999f)
   {
      /* Very close - just use linear interpolation,
         which will protect againt a divide by zero */

      k0 = 1.0f - t;
      k1 = t;
   }
   else
   {
      /* Compute the sin of the angle using the
         trig identity sin^2(omega) + cos^2(omega) = 1 */
      float sinOmega = sqrt(1.0f - (cosOmega * cosOmega));

      /* Compute the angle from its sin and cosine */
      float omega = atan2(sinOmega, cosOmega);

      /* Compute inverse of denominator, so we only have to divide once */
      float oneOverSinOmega = 1.0f / sinOmega;

      /* Compute interpolation parameters */
      k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
      k1 = sin(t * omega) * oneOverSinOmega;
   }

   /* Interpolate and return new quaternion */
   quaternion out;
   out.w = (k0 * w) + (k1 * q1w);
   out.x = (k0 * x) + (k1 * q1x);
   out.y = (k0 * y) + (k1 * q1y);
   out.z = (k0 * z) + (k1 * q1z);

   return(out);
}




