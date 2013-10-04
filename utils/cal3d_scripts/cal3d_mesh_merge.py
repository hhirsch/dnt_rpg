#!/usr/bin/python2

#  DNT: a satirical post-apocalyptical RPG.
#  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
# 
#  This file is part of DNT.
# 
#  DNT is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  DNT is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with DNT.  If not, see <http://www.gnu.org/licenses/>.

import argparse
import cal3d_mesh
         
parser = argparse.ArgumentParser(description="Merge Cal3D meshes on a "+
                                 "singlemesh (.xmf)\n"+
                                 "NOTE: Only supports a single submesh "+
                                 "per file, no springs and no lods.")

parser.add_argument('--output', action="store", default="output.xmf",
                    help = "Set output file")
parser.add_argument("input", action="store", nargs="+", 
                    help="Set input file(s)")

args = parser.parse_args()

f = cal3d_mesh.meshFile()
firstArg = 1

for i in args.input:
   if(firstArg):
      firstArg = 0
      f.loadFile(i)
   else:
      f2 = cal3d_mesh.meshFile()
      f2.loadFile(i)
      f.append(f2)

f.write(args.output)

