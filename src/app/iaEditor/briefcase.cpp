#include "briefcase.h"


briefCases::briefCases()
{
   totalCases = 0;
}

briefCases::~briefCases()
{
   totalCases = -1;
}

void briefCases::insertBriefCase(GLfloat x, GLfloat z, GLfloat angle, int value)
{
   cases[totalCases].x = x;
   cases[totalCases].z = z;
   cases[totalCases].angle = angle;
   cases[totalCases].value = value;
   totalCases++;
}
