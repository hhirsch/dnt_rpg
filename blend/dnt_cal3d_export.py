########################################################################
#
# Cal3D exporter for Blender 2.5x [Python 3.x]
#
#   Author: Farrer <farrer@dnteam.org>
#   Website: http://dnt.dnteam.org
#
#   FIXME-list:
#      - Texture uvmap;
#
#   TODO-list:
#      - Accept more than one UV map per vertex
#
########################################################################

########################################################################
#
#  DccNiTghtmare: a satirical post-apocalyptical RPG.
#  Copyright (C) 2005-2010 DNTeam <dnt@dnteam.org>
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
#
########################################################################


# Script Version
SCRIPT_VERSION = "0.1"

import bpy


########################################################################
# A world coordinate (x,y,z)
########################################################################
class cal3dCoordinate:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.x = 0.0
      self.y = 0.0
      self.z = 0.0

########################################################################
# The Cal3d vertex definition (with normal, coordinate, texture and 
# influences)
########################################################################
class cal3dVertex:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.pos = cal3dCoordinate()
      self.normal = cal3dCoordinate()
      self.uv = cal3dCoordinate()
      self.index = 0
      self.influences = []

########################################################################
# The cal3d face definition (3 vertices)
########################################################################
class cal3dFace:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.indexA = -1
      self.indexB = -1
      self.indexC = -1

########################################################################
# The cal3d mesh, with vertices and faces
# \note -> as blender has no submeshes definition, it only has one mesh
########################################################################
class cal3dMesh:
 
   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.vertex = []
      self.face = []
      self.material = -1
      self.name = "mesh"
      self.influences = []

   ##################################################################
   # Populate the cal3d mesh with the blender mesh
   # TODO: material coordinate!
   # \param obj -> blender object of type MESH
   ##################################################################
   def populate(self, obj):
      objMesh = obj.data
      self.name = obj.name.replace(".", "_")

      # Define all vertices
      for vert in objMesh.verts:
         v = cal3dVertex()
         self.vertex.append(v)
         # Define its index
         v.index = vert.index
         # Define its coordinate
         v.pos.x = vert.co[0]
         v.pos.y = vert.co[1]
         v.pos.z = vert.co[2]
         # Define its normal
         v.normal.x = vert.normal[0]
         v.normal.y = vert.normal[1]
         v.normal.z = vert.normal[2]
         # TODO: Define its influences

      # Define all faces
      for face in objMesh.faces:
         if(len(face.verts) == 3):
            f = cal3dFace()
            self.face.append(f)
            f.indexA = face.verts[0]
            f.indexB = face.verts[1]
            f.indexC = face.verts[2]
         else:
            # TODO: abort
            print("Warning: Ignoring a non triangle face.")

      # Define all uvs for each vertex
      for uv_tex in objMesh.uv_textures:
         faceIndex = 0
         for uv in  uv_tex.data:
            # Get Vertices
            face = self.face[faceIndex]
            vert1 = self.vertex[face.indexA]
            vert2 = self.vertex[face.indexB]
            vert3 = self.vertex[face.indexC]

            # Set their uv's
            vert1.uv.x = uv.uv1[0]
            vert1.uv.y = uv.uv1[1]
            vert2.uv.x = uv.uv2[0]
            vert2.uv.y = uv.uv2[1]
            vert3.uv.x = uv.uv3[0]
            vert3.uv.y = uv.uv3[1]

            faceIndex += 1


   ##################################################################
   # Save the mesh to a cal3d mesh file (*.xmf)
   # \note Only a single submesh per mesh
   ##################################################################
   def save(self, directory):
      # Do not save empty meshes!
      if( (len(self.vertex) == 0) | (len(self.face) == 0) ):
         print("Warning: ignoring empty mesh: ", self.name)
         return;
    
      # Open the file
      meshFile = open(directory + self.name + ".xmf", "w")

      # Write XML header
      meshFile.write("<?xml version=\"1.0\"?>\n")
      meshFile.write("<HEADER MAGIC=\"XMF\" VERSION=\"1200\"/>\n")
      meshFile.write("<MESH NUMSUBMESH=\"1\">\n")
      meshFile.write("   <SUBMESH \n")
      meshFile.write("      NUMVERTICES=\"" + str(len(self.vertex)) + "\"\n")
      meshFile.write("      NUMFACES=\"" + str(len(self.face)) + "\"\n")
      #TODO: material!
      meshFile.write("      MATERIAL=\"0\"\n")
      meshFile.write("      NUMLODSTEPS=\"0\"\n")
      meshFile.write("      NUMSPRINGS=\"0\"\n")
      meshFile.write("      NUMTEXCOORDS=\"1\"\n")
      meshFile.write("   >\n")

      # Write each vertex
      for vert in self.vertex:
         meshFile.write("      <VERTEX ID=\"" + str(vert.index) + 
                        "\" NUMINFLUENCES=\"" + 
                        str(len(vert.influences)) + "\">\n")
         meshFile.write("         <POS>" + str(vert.pos.x) + " " +
                        str(vert.pos.y) + " " + str(vert.pos.z) + " </POS>\n")
         meshFile.write("         <NORM>" + str(vert.normal.x) + " " +
                        str(vert.normal.y) + " " + 
                        str(vert.normal.z) + " </NORM>\n")
         meshFile.write("         <TEXCOORD>" + str(vert.uv.x) + " " +
                        str(vert.uv.y) + " </TEXCOORD>\n")
         # Write each vertex influences
         for infl in self.influences:
            meshFile.write("         <INFLUENCE ID=\"" + str(infl.boneIndex) +
                           "\">" + str(infl.weight) + "</INFLUENCE>\n")

         meshFile.write("      </VERTEX>\n")
      
      # Write each face
      for face in self.face:
         meshFile.write("      <FACE VERTEXID=\"" + str(face.indexA) + " " +
                        str(face.indexB) + " " + str(face.indexC) + "\"/>\n")

      # Write the footer
      meshFile.write("   </SUBMESH>\n")
      meshFile.write("</MESH>\n")

      # Flush and close it!
      meshFile.flush()
      meshFile.close()


########################################################################
# The armature bone definition for cal3d
########################################################################
class cal3dBone:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.translation = cal3dCoordinate()
      self.rotation = cal3dCoordinate()
      self.localTranslation = cal3dCoordinate()
      self.localRotation = cal3dCoordinate()
      self.parentName = None
      self.parentId = -1
      self.childName = []
      self.childId = []
      self.id = -1
      self.name = "bone"


########################################################################
# The armature definition, as a list of bones
########################################################################
class cal3dArmature:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.bones = []
      self.name = "armature"

   ##################################################################
   # Get bone from bones list
   ##################################################################
   def getBoneId(self, boneName):
      for b in self.bones:
         if(b.name == boneName):
            return(b.id)
      return(-1)

   ##################################################################
   # Set all children and parent ids
   ##################################################################
   def setFamiliarIds(self):
      for b in self.bones:
         # Set parent
         if(b.parentName != None):
            b.parentId = self.getBoneId(b.parentName)
         # Set each child
         for c in b.childName:
            b.childId.append(self.getBoneId(c))

   ##################################################################
   # Populate
   # \param obj -> blender armature to populate the cal3d armature
   ##################################################################
   def populate(self, obj):
      cur = -1
      self.name = obj.name.replace(".", "_")
      print(self.name)
      # For all bones
      for b in obj.data.bones:
         cur += 1
         # Create the cal3d bone
         bone = cal3dBone()
         self.bones.append(bone)
         bone.name = b.name
         bone.id = cur
         # Define translation and rotation
         bone.localTranslation = b.matrix_local.translation_part()
         bone.localRotation = b.matrix_local.to_euler()
         matrix_copy = b.matrix.copy()
         matrix_copy.resize4x4()
         bone.translation = matrix_copy.translation_part()
         bone.rotation = b.matrix.to_euler()
         # Define Parent and children
         if(b.parent != None):
            bone.parentName = b.parent.name
         for c in b.children:
            bone.childName.append(c.name)
      # Now Set ids, based on name
      self.setFamiliarIds()


   ##################################################################
   # Save armature to the cal3d skeleton file
   # \param directory -> directory to save file to
   ##################################################################
   def save(self, directory):
      # Do not save empty armatures
      if(len(self.bones) == 0):
         print("Warning: ignoring empty armature: ", self.name)
         return;
    
      # Open the file
      f = open(directory + self.name + ".xsf", "w")

      # Write XML header
      f.write("<?xml version=\"1.0\"?>\n")
      f.write("<HEADER MAGIC=\"XSF\" VERSION=\"1200\"/>\n")
      f.write("<SKELETON NUMBONES=\"" + str(len(self.bones)) + "\">\n")

      # Write all bones
      for b in self.bones:
         f.write("   <BONE ID=\"" + str(b.id) + "\" NAME=\"" +
                 b.name + "\" NUMCHILD=\"" + str(len(b.childId)) + "\">\n")
         f.write("      <TRANSLATION>" + str(round(b.translation[0], 4)) + " " +
                 str(round(b.translation[1],4)) + " " + 
                 str(round(b.translation[2],4)) + "</TRANSLATION>\n")
         f.write("      <ROTATION>" + str(round (b.rotation.x,4)) + " " +
                 str(round(b.rotation.y,4)) + " " + 
                 str(round(b.rotation.z,4)) + "</ROTATION>\n")
         f.write("      <LOCALTRANSLATION>" + 
                 str(round(b.localTranslation[0],4)) + " " +
                 str(round(b.localTranslation[1],4)) + " " + 
                 str(round(b.localTranslation[2],4)) + "</LOCALTRANSLATION>\n")
         f.write("      <LOCALROTATION>" + 
                 str(round(b.localRotation[0],4)) + " " +
                 str(round(b.localRotation[1],4)) + " " + 
                 str(round(b.localRotation[2],4)) + "</LOCALROTATION>\n")
         f.write("      <PARENTID>" + str(b.parentId) + "</PARENTID>\n")
         for c in b.childId:
            f.write("      <CHILDID>" + str(c) + "</CHILDID>\n")
         f.write("   </BONE>\n")

      # Footer, flush an close!
      f.write("</SKELETON>")
      f.flush()
      f.close()

########################################################################
# The cal3D Texture
########################################################################
class cal3dTexture:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.name = ""
      self.fileName = ""
      self.imageName = ""

########################################################################
# The cal3D Texture list
########################################################################
class cal3dTextures:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.textures = []

      for t in bpy.data.textures:
         # Create the cal3d related
         tex = cal3dTexture()
         self.textures.append(tex)

         # Populate it
         tex.name = t.name

         if(t.image != None):
            tex.fileName = t.image.filename
            tex.imageName = t.image.name

   ##################################################################
   # Get a texture from the list
   # \param name -> texture name to get
   # \return texture got or None
   ##################################################################
   def getTexture(self, name):
      for t in self.textures:
         if(t.name == name):
            # Found it!
            return(t)

      # Not found
      return(None)

########################################################################
# The cal3D Material
########################################################################
class cal3dMaterial:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.name = "material"
      self.id = -1
      self.textures = []
      self.ambient = [0.0, 0.0, 0.0, 0.0]
      self.diffuse = [0.0, 0.0, 0.0, 0.0]
      self.specular = [0.0, 0.0, 0.0, 0.0]
      self.shininess = 1.0

   ##################################################################
   # Populate
   # \param obj -> blender material to populate the cal3d one
   # \param texList -> list of textures
   ##################################################################
   def populate(self, obj, texList):
      self.name = obj.name.replace(".", "_")

      # Set light influences
      alphaValue = round(obj.alpha*255,4)
      ambValue = round(obj.ambient*255,4)
      self.ambient = [ambValue, ambValue, ambValue, alphaValue]
      self.diffuse = [obj.diffuse_color[0], obj.diffuse_color[1],
                      obj.diffuse_color[2], alphaValue]
      self.specular = [obj.specular_color[0], obj.specular_color[1],
                       obj.specular_color[2], alphaValue]

      # Set all textures related to the material
      for tex in obj.texture_slots:
         if(tex != None):
            texObj = texList.getTexture(tex.name)
            if(texObj != None):
               self.textures.append(texObj.fileName)

   ##################################################################
   # Save material to the file
   # \param directory -> directory to save file to
   ##################################################################
   def save(self, directory):

      # Open the file
      f = open(directory + self.name + ".xrf", "w")

      # Write XML header
      f.write("<?xml version=\"1.0\"?>\n")
      f.write("<HEADER MAGIC=\"XRF\" VERSION=\"1200\"/>\n")

      f.write("<MATERIAL NUMMAPS=\"" + str(len(self.textures)) + "\">\n")
      f.write("   <AMBIENT>" + str(self.ambient[0]) + " " + 
              str(self.ambient[1]) + " " + str(self.ambient[2]) + " " +
              str(self.ambient[3]) + "</AMBIENT>\n")
      f.write("   <DIFFUSE>" + str(self.diffuse[0]) + " " + 
              str(self.diffuse[1]) + " " + str(self.diffuse[2]) + " " +
              str(self.diffuse[3]) + "</DIFFUSE>\n")
      f.write("   <SPECULAR>" + str(self.specular[0]) + " " + 
              str(self.specular[1]) + " " + str(self.specular[2]) + " " +
              str(self.specular[3]) + "</SPECULAR>\n")
      f.write("   <SHININESS>" + str(self.shininess) + "</SHININESS>\n")
      for tex in self.textures:
         f.write("   <MAP>" + tex + "</MAP>\n")
      f.write("</MATERIAL>")

      # Flush and close
      f.flush()
      f.close()


########################################################################
# The cal3D file main informations (.cfg)
########################################################################
class cal3dFile:

   ##################################################################
   # Constructor
   ##################################################################
   def __init__(self):
      self.directory = "/home/farrer/cal3d_test/"
      self.fileName = "model.cfg"
      self.mesh = []
      self.armature = []
      self.material = []
      self.textures = None
   
   ##################################################################
   # Set the directory to export model to
   # \param dir -> directory to export
   ##################################################################
   def setDirectory(self, dir):
      self.directory = dir

   ##################################################################
   # Get the model and objects from blender, creating internal 
   # structures (before saving it to the directory).
   ##################################################################
   def getModel(self):

      # Pass all textures
      self.textures = cal3dTextures()

      # Pass all materials
      cur = -1
      for mat in bpy.data.materials:
         # Create the new material
         mt = cal3dMaterial()
         self.material.append(mt)
         cur += 1
         # Populate it
         mt.id = cur
         mt.populate(mat, self.textures)
      
      # Pass all objects
      for obj in bpy.data.objects:
         print("Got: ", obj.name, " !")
         
         if(obj.type == "MESH"):
            # Create the new mesh
            m = cal3dMesh()
            self.mesh.append(m)
            # Populate it with blender mesh
            m.populate(obj)

         elif(obj.type == "ARMATURE"):
            arm = cal3dArmature()
            self.armature.append(arm)
            # Populate ir with blender armature
            arm.populate(obj)

   ##################################################################
   # save
   ##################################################################
   def save(self):
      # Create and write cfg header
      file = open(self.directory + self.fileName, "w")
      file.write("# Cal3D model exported from blender " + 
                 "with dnt_cal3d_export.py " + SCRIPT_VERSION + "\n\n")
      file.write("scale=1.0f\n")

      # Write each armature file
      if(len(self.armature) == 0):
         print("Warning: Cal3D needs one or more armatures!")
      for a in self.armature:
         file.write("skeleton=" + a.name + ".xsf\n")
         a.save(self.directory)

      # Write each mesh file
      for m in self.mesh:
         file.write("mesh="+m.name+".xmf\n")
         m.save(self.directory)

      # Write each material file
      for mt in self.material:
         file.write("material="+mt.name+".xrf")
         mt.save(self.directory)

      # Flush and close the cfg file
      file.flush()
      file.close()


########################################################################
# Main
########################################################################
if __name__ == "__main__":

   c3d = cal3dFile()
   c3d.getModel()
   c3d.save()
