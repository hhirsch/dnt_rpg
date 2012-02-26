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

#ifndef _dnt_quaternion_h
#define _dnt_quaternion_h

/*! Vectors */
class vec2_t
{
   public:

      vec2_t()
      {
         x = 0.0f;
         y = 0.0f;
      };
      
      inline float operator[] (const unsigned int i)
      {
         switch(i)
         {
            case 0:
               return(x);
            break;
            case 1:
               return(y);
            break;
            default:
               return(0.0f);
            break;
         }
      }

      /*! Assign */
      inline vec2_t& operator= (const vec2_t & v)
      {
         x = v.x;
         y = v.y;
         return *this;
      }


      float x, y;
};

class vec3_t
{
   public:

      vec3_t()
      {
         x = 0.0f;
         y = 0.0f;
         z = 0.0f;
      };

      vec3_t(float f[3])
      {
         x = f[0];
         y = f[1];
         z = f[2];
      };

      inline float operator[] (const unsigned int i)
      {
         switch(i)
         {
            case 0:
               return(x);
            break;
            case 1:
               return(y);
            break;
            case 2:
               return(z);
            break;
            default:
               return(0.0f);
            break;
         }
      }

      /*! Assign */
      inline vec3_t& operator= (const vec3_t& v)
      {
         x = v.x;
         y = v.y;
         z = v.z;
         return *this;
      }

      /*! Addition */
      vec3_t operator+(const vec3_t& v) const;
      /*! Subtraction */
      vec3_t operator-(const vec3_t& v) const;
      vec3_t operator*(float num) const;
      /*! Cross product */
      vec3_t cross(const vec3_t& v) const;
 
      /*! Normalize vector */
      void normalize();
     


      float x, y, z;
};

/*! Quaternion (x, y, z, w) */
class quaternion
{
   public:
      /*! Constructor */
      inline quaternion(float fX=0.0f, float fY=0.0f, 
                       float fZ=0.0f, float fW=1.0f)
      {
         x = fX;
         y = fY;
         z = fZ;
         w = fW;
      }

      /*! Vector-like access */
      inline float operator[] (const unsigned int i)
      {
         switch(i)
         {
            case 0:
               return(x);
            break;
            case 1:
               return(y);
            break;
            case 2:
               return(z);
            break;
            case 3:
               return(w);
            break;
            default:
               return(0.0f);
            break;
         }
      }

      /*! Assign */
      inline quaternion& operator= (const quaternion& rkQ)
      {
         w = rkQ.w;
         x = rkQ.x;
         y = rkQ.y;
         z = rkQ.z;
         return *this;
      }

      /*! Compute quaternion W component */
      void computeW();
      /*! Normalize quaternion */
      void normalize();

      /*! Multiplicator */
      quaternion operator* (const quaternion& qb) const;
      quaternion operator* (const vec3_t& v) const;

      /*! Rotate a point */
      vec3_t rotatePoint(const vec3_t& in) const;

      /*! Do a dot product */
      float dot(const quaternion& qb) const;

      /*! Slerp operation */
      quaternion slerp(const quaternion& qb, float t) const;

      float x, y, z, w;  /**< Quaternion values */
};



#endif

