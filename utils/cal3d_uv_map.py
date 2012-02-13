#!/usr/bin/python2

#  DccNiTghtmare: a satirical post-apocalyptical RPG.
#  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
# 
#  This file is part of DccNiTghtmare.
# 
#  DccNiTghtmare is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  DccNiTghtmare is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.

import argparse
from PIL import Image, ImageDraw
import cal3d_mesh

# Set the arguments parser
parser = argparse.ArgumentParser(description="Save the UV map of a "+
                                 "singlemesh (.xmf) in an image file")
parser.add_argument('--output', action="store", default="output.png",
                    help = "Set output file")
parser.add_argument("--size", action="store", type=int, default=2048)
parser.add_argument("input", action="store", help="Set input mesh file")

# Parse arguments
args = parser.parse_args()

# Load Mesh
f = cal3d_mesh.meshFile()
f.loadFile(args.input)

# Create the dest image
size = int(args.size)
img = Image.new("RGB", [size, size])

draw = ImageDraw.Draw(img)

t1 = [0, 0]
t2 = [0, 0]
t3 = [0, 0]

# Draw each face
for fa in f.faceData:
   v1 = f.vertexData[fa.f[0]].texCoord[0]
   v2 = f.vertexData[fa.f[1]].texCoord[0]
   v3 = f.vertexData[fa.f[2]].texCoord[0]

   t1[0] = round(size*float(v1.x))
   t1[1] = round(size*float(v1.y))

   t2[0] = round(size*float(v2.x))
   t2[1] = round(size*float(v2.y))

   t3[0] = round(size*float(v3.x))
   t3[1] = round(size*float(v3.y))

   draw.line( [(t1[0], t1[1]), (t2[0], t2[1]), (t3[0], t3[1])])

del draw
img.save(args.output, "PNG")

print "Output image written to: ", args.output

