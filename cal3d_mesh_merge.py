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


# cal3d_mesh_merge will merge input files meshes in a single mesh output file.
# NOTE: Only supports a single submesh per file

import argparse

# A single influence
class influence():
   def __init__(self, infData):
      self.index = 0
      self.weight = 0
      infData = infData.replace("INFLUENCE ID=","").replace("/","")
      infData = infData.replace("\"","").strip()
      i = infData.split(" ")
      self.index = i[0]
      self.weight = i[1]


# A single texture coordinate
class texCoord():
   def __init__(self, texData):
      self.x = 0
      self.y = 0
      texData = texData.replace("TEXCOORD","").replace("/","").strip()
      p = texData.split(" ")
      self.x = p[0]
      self.y = p[1]

# a single vertex representation
class vertex():
   def __init__(self, vertexData):
      self.index = 0
      self.numInfluences = 0
      self.pos = []
      self.norm = []
      self.texCoord = []
      self.influences = []
      # Clean the vertexData to parse
      vertexData = vertexData.replace(">"," ").replace("<"," ").strip()
      for e in vertexData.split("\n"):
         e = e.strip()
         title = e.split(" ")[0]
         if(title == "VERTEX"):
            self.parseHeader(e)
         elif(title == "POS"):
            self.parsePos(e)
         elif(title == "NORM"):
            self.parseNorm(e)
         elif(title == "TEXCOORD"):
            self.parseTexCoord(e)
         elif(title == "INFLUENCE"):
            self.parseInfluence(e)
         #elif( (title != "") & (title != "/VERTEX")):
         #   print "Ignoring: ", title

   def parseHeader(self, header):
      for e in header.split(" "):
         i = e.split("=")
         if(i[0] == "ID"):
            self.index = int(i[1])
         elif(i[0] == "NUMINFLUENCES"):
            self.numInfluences = i[1]

   def parsePos(self, s):
      s = s.replace("POS","").replace("/","").strip()
      for p in s.split(" "):
         self.pos.append(p)

   def parseNorm(self, s):
      s = s.replace("NORM","").replace("/","").strip()
      for p in s.split(" "):
         self.norm.append(p)

   def parseTexCoord(self, s):
      self.texCoord.append(texCoord(s))

   def parseInfluence(self, s):
      self.influences.append(influence(s))

   def incId(self, i):
      self.index += i

   def toString(self):
     s = "      <VERTEX ID=\""+str(self.index)+"\" "
     s += "NUMINFLUENCES=\""+str(self.numInfluences)+"\">\n"
     s += "         <POS>"+str(self.pos[0])+" "+str(self.pos[1])+" "
     s += str(self.pos[2])+"</POS>\n"
     s += "         <NORM>"+str(self.norm[0])+" "+str(self.norm[1])+" "
     s += str(self.norm[2])+"</NORM>\n"
     for t in self.texCoord:
        s += "         <TEXCOORD>"+str(t.x)+" "+str(t.y)+"</TEXCOORD>\n"
     for i in self.influences:
        s += "         <INFLUENCE ID=\""+str(i.index)+"\">"
        s += str(i.weight) + "</INFLUENCE>\n"
     s += "      </VERTEX>\n"
     return s

# A single face representation (with 3 vertex ids)
class face():
   def __init__(self, s):
      self.f = []
      s = s.replace("FACE VERTEXID=","").replace("/","").strip()
      for i in s.split(" "):
         self.f.append(int(i))

   def incId(self, i):
      self.f[0] += i
      self.f[1] += i
      self.f[2] += i

   def toString(self):
      s = "      <FACE VERTEXID=\""+str(self.f[0])+" "+str(self.f[1])+" "
      s += str(self.f[2])+"\"/>\n"
      return s
      

# The mesh file parser
class meshFile():
   def __init__(self):
      # Definitions
      self.name = ""
      self.numVertices = 0
      self.numFaces = 0
      self.material = 0
      self.numLodSteps = 0
      self.numSprings = 0
      self.numTexCoords = 0
      # Vertex Data
      self.vertexData = []
      self.faceData = []
      # Flux controll variables
      self.strToParse = ""


   def loadFile(self, fileName):
      self.name = fileName
      doc = open(fileName, "r")
      for line in doc:
         if(line != ""):
            self.strToParse += "\n" + line
            # Clean line to parse it
            line = line.strip().replace("<","").replace(">","").replace("\"","")
            e = line.split(" ")[0]
            if(e == "SUBMESH"):
              self.parseSubMeshInfo(line)
            elif(e == "VERTEX"):
              self.strToParse = line
            elif(e == "/VERTEX"):
               self.vertexData.append(vertex(self.strToParse))
            elif(e == "FACE"):
               self.faceData.append(face(line))
      doc.close()

   # Parse the submesh info title
   def parseSubMeshInfo(self, line):
      for e in line.split(" "):
         i = e.split("=")
         if(i[0] == "NUMVERTICES"):
            self.numVertices = int(i[1])
         elif(i[0] == "NUMFACES"):
            self.numFaces = int(i[1])
         elif(i[0] == "MATERIAL"):
            self.material = int(i[1])
         elif(i[0] == "NUMLODSTEPS"):
            self.lodSteps = int(i[1])
         elif(i[0] == "NUMSPRINGS"):
            self.numSprings = int(i[1])
         elif(i[0] == "NUMTEXCOORDS"):
            self.numTexCoords = int(i[1])

   # Increment the mesh ids
   def incId(self, i):
      for v in self.vertexData:
         v.incId(i)
      for f in self.faceData:
         f.incId(i)

   # Append another mesh data to the current one
   def append(self, mesh):
      # Increment the mesh IDs
      mesh.incId(self.numVertices)
      # Now addd each vertex and face to the current
      for v in mesh.vertexData:
         self.vertexData.append(v)
      for f in mesh.faceData:
         self.faceData.append(f)
      # And increment the total vertex and faces
      self.numVertices += mesh.numVertices
      self.numFaces += mesh.numFaces
      # Just verify if same material
      if(self.material != mesh.material):
         print "Warning: Merging meshes with different materials!"

   # Write mesh data to a file
   def write(self, fileName):
      f = open(fileName, "w")
      f.write("<HEADER MAGIC=\"XMF\" VERSION=\"900\"/>\n")
      f.write("<MESH NUMSUBMESH=\"1\">\n")
      f.write("   <SUBMESH NUMVERTICES=\""+str(self.numVertices)+"\" "+
                          "NUMFACES=\""+str(self.numFaces)+"\" "+
                          "MATERIAL=\""+str(self.material)+"\" "+
                          "NUMLODSTEPS=\"0\" NUMSPRINGS=\"0\" "+
                          "NUMTEXCOORDS=\""+str(self.numTexCoords)+"\">\n")
      for v in self.vertexData:
         f.write(v.toString())
      for fa in self.faceData:
         f.write(fa.toString())
      f.write("   </SUBMESH>\n")
      f.write("</MESH>\n")
      f.close()

parser = argparse.ArgumentParser(description="Merge Cal3D meshes on a "+
                                 "singlemesh (.xmf)\n"+
                                 "NOTE: Only supports a single submesh "+
                                 "per file, no springs and no lods.")

parser.add_argument('--output', action="store", default="output.xmf",
                    help = "Set output file")
parser.add_argument("input", action="store", nargs="+", 
                    help="Set input file(s)")

args = parser.parse_args()

f = meshFile()
firstArg = 1

for i in args.input:
   if(firstArg):
      firstArg = 0
      f.load(i)
   else:
      f2 = meshFile()
      f2.load(i)
      f.merge(f2)

f.write(args.output)

#f1 = meshFile()
#f1.loadFile("/projetos/dccnitghtmare/data/models/personagens/horse/Horse.xmf")

#f2 = meshFile()
#f2.loadFile("/projetos/dccnitghtmare/data/models/personagens/horse/HorseTail.xmf")

#f3 = meshFile()
#f3.loadFile("/projetos/dccnitghtmare/data/models/personagens/horse/HorseEars.xmf")

#f1.append(f2)
#f1.append(f3)
#f1.write("HorseFull.xmf")
