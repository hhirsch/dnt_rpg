#"""
#Name: 'DNT md5 model (.md5)...'
#Blender: 263
#Group: 'Export'
#Tooltip: 'Export a DNT md5 File'
#
# It'll save all armature animations to respective .md5anim files,
# and also create a "header" md5 (.md5def).
#
#"""

########################################################################
#                                                                      #
#                          Script Definition                           #
#                                                                      #
########################################################################
bl_info = {
    "name": "Export DNT md5 (.md5def/.md5anim/.md5mesh)",
    "author": "DNTeam <dnt@dnteam.org>,"\
              "based on .md5 exporter from nemyax",
    "version": (0, 10, 0),
    "blender": (2, 6, 3),
    "location": "File > Export > DNT md5 Data (.md5def/.md5mesh/.md5anim)",
    "description": "Export DNT (.md5def/.md5mesh/.md5anim)",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

import bpy
import bmesh
import os.path

########################################################################
#                                                                      #
#                       Common Export Functions                        #
#                                                                      #
########################################################################

######################################################
# Get the record parameters used (matrices) as string
######################################################
def record_parameters(correctionMatrix):
    return [
    "// Parameters used during export:\n",
    "//   Reorient: {}\n".format(bool(correctionMatrix.to_euler()[2])),
    "//   Scale: {}\n".format(correctionMatrix.decompose()[2][0])]

######################################################
# Define the components used at export
######################################################
def define_components(obj, bm, bones, correctionMatrix):
    scaleFactor = correctionMatrix.to_scale()[0]
    armature = [a for a in bpy.data.armatures if bones[0] in a.bones[:]][0]
    armatureObj = [o for o in bpy.data.objects if o.data == armature][0]
    vertGroups = obj.vertex_groups
    uvData = bm.loops.layers.uv.active
    weightData = bm.verts.layers.deform.active
    tris = [[f.index, f.verts[2].index, f.verts[1].index, f.verts[0].index]
        for f in bm.faces] # reverse vert order to flip normal
    verts = []
    weights = []
    wtIndex = 0
    firstWt = 0
    for vert in bm.verts:
        wtDict = vert[weightData]
        try:
           u = vert.link_loops[0][uvData].uv.x
           v = 1 - vert.link_loops[0][uvData].uv.y # MD5 wants it flipped
        except:
           u = 0
           v = 0
        numWts = len(wtDict.keys())
        verts.append([vert.index, u, v, firstWt, numWts])
        wtScaleFactor = 1.0 / sum(wtDict.values())
        firstWt += numWts
        for vGroup in wtDict.keys():
            bone = [b for b in bones
                if b.name == vertGroups[vGroup].name][0]
            boneIndex = bones.index(bone)
            coords4d =\
                bone.matrix_local.inverted() *\
                armatureObj.matrix_world.inverted() *\
                obj.matrix_world *\
                (vert.co.to_4d() * scaleFactor)
            x, y, z = coords4d[:3]
            weight = wtDict[vGroup] * wtScaleFactor
            wtEntry = [wtIndex, boneIndex, weight, x, y, z]
            weights.append(wtEntry)
            wtIndex += 1
    return (verts, tris, weights)

######################################################
# Make the bone hierarchy string
######################################################
def make_hierarchy_block(bones, boneIndexLookup):
    block = ["hierarchy {\n"]
    xformIndex = 0
    for b in bones:
        if b.parent:
            parentIndex = boneIndexLookup[b.parent.name]
        else:
            parentIndex = -1
        block.append("  \"{}\" {} 63 {} //\n".format(
            b.name, parentIndex, xformIndex))
        xformIndex += 6
    block.append("}\n")
    block.append("\n")
    return block

######################################################
# Define the base frame block string
######################################################
def make_baseframe_block(bones, correctionMatrix):
    block = ["baseframe {\n"]
    armature = bones[0].id_data
    armObject = [o for o in bpy.data.objects
        if o.data == armature][0]
    armMatrix = armObject.matrix_world
    for b in bones:
        objSpaceMatrix = b.matrix_local
        if b.parent:
            bMatrix =\
            b.parent.matrix_local.inverted() *\
            armMatrix *\
            objSpaceMatrix
        else:
            bMatrix = correctionMatrix * objSpaceMatrix
        xPos, yPos, zPos = bMatrix.translation
        xOrient, yOrient, zOrient = (-bMatrix.to_quaternion()).normalized()[1:]
        block.append("  ( {:.6f} {:.6f} {:.6f} ) ( {:.6f} {:.6f} {:.6f} )\n".\
        format(xPos, yPos, zPos, xOrient, yOrient, zOrient))
    block.append("}\n")
    block.append("\n")
    return block

######################################################
# Create the joint block string
######################################################
def make_joints_block(bones, boneIndexLookup, correctionMatrix):
    block = []
    block.append("joints {\n")
    for b in bones:
        if b.parent:
            parentIndex = boneIndexLookup[b.parent.name]
        else:
            parentIndex = -1
        boneMatrix = correctionMatrix * b.matrix_local
        xPos, yPos, zPos = boneMatrix.translation
        xOrient, yOrient, zOrient =\
        (-boneMatrix.to_quaternion()).normalized()[1:] # MD5 wants it negated
        block.append(\
        "  \"{}\" {} ( {:.6f} {:.6f} {:.6f} ) ( {:.6f} {:.6f} {:.6f} )\n".\
        format(b.name, parentIndex,\
        xPos, yPos, zPos,\
        xOrient, yOrient, zOrient))
    block.append("}\n")
    block.append("\n")
    return block

######################################################
# Create the mesh block string
######################################################
def make_mesh_block(obj, bones, correctionMatrix):
    bm = bmesh.new()
    bm.from_mesh(obj.data)
    triangulate(cut_up(strip_wires(bm)))
    verts, tris, weights = define_components(obj, bm, bones, correctionMatrix)
    bm.free()
    block = []
    block.append("mesh {\n")
    block.append("  shader \"default\"\n")
    block.append("  numverts {}\n".format(len(verts)))
    for v in verts:
        block.append(\
        "  vert {} ( {:.6f} {:.6f} ) {} {}\n".\
        format(v[0], v[1], v[2], v[3], v[4]))
    block.append("  numtris {}\n".format(len(tris)))
    for t in tris:
        block.append("  tri {} {} {} {}\n".format(t[0], t[1], t[2], t[3]))
    block.append("  numweights {}\n".format(len(weights)))
    for w in weights:
        block.append(\
        "  weight {} {} {:.6f} ( {:.6f} {:.6f} {:.6f} )\n".\
        format(w[0], w[1], w[2], w[3], w[4], w[5]))
    block.append("}\n")
    block.append("\n")
    return block

######################################################
# Remove wires from model
######################################################
def strip_wires(bm):
    wireVerts = [v for v in bm.verts if v.is_wire]
    for v in wireVerts: bmesh.utils.vert_dissolve(v)
    for seq in [bm.verts, bm.faces, bm.edges]: seq.index_update()
    return bm

######################################################
# Cut up
######################################################
def cut_up(bm):
    uvData = bm.loops.layers.uv.active
    for v in bm.verts:
        for e in v.link_edges:
            linkedFaces = e.link_faces
            if len(linkedFaces) > 1:
                uvSets = []
                for lf in linkedFaces:
                    uvSets.append([l1[uvData].uv for l1 in lf.loops
                        if l1.vert == v][0])
                if uvSets.count(uvSets[0]) != len(uvSets):
                    e.tag = True
                    v.tag = True
        if v.tag:
            seams = [e for e in v.link_edges if e.tag]
            v.tag = False
            bmesh.utils.vert_separate(v, seams)
    for maybeBowTie in bm.verts: # seems there's no point in a proper test
        boundaries = [e for e in maybeBowTie.link_edges
            if len(e.link_faces) == 1]
        bmesh.utils.vert_separate(maybeBowTie, boundaries)
    for seq in [bm.verts, bm.faces, bm.edges]: seq.index_update()
    return bm         

######################################################
# Triangulate the mesh
######################################################
def triangulate(bm):
    while True:
        nonTris = [f for f in bm.faces if len(f.verts) > 3]
        if nonTris:
            nt = nonTris[0]
            pivotLoop = nt.loops[0]
            allVerts = nt.verts
            vert1 = pivotLoop.vert
            wrongVerts = [vert1,
                pivotLoop.link_loop_next.vert,
                pivotLoop.link_loop_prev.vert]
            bmesh.utils.face_split(nt, vert1, [v for v in allVerts
                if v not in wrongVerts][0])
            for seq in [bm.verts, bm.faces, bm.edges]: seq.index_update()
        else: break
    return bm

######################################################
# Create and write an .md5mesh
######################################################
def write_md5mesh(filePath, prerequisites, correctionMatrix):
    bones, meshObjects = prerequisites
    boneIndexLookup = {}
    for b in bones:
        boneIndexLookup[b.name] = bones.index(b)
    md5joints = make_joints_block(bones, boneIndexLookup, correctionMatrix)
    md5meshes = []
    for mo in meshObjects:
        md5meshes.append(make_mesh_block(mo, bones, correctionMatrix))
    f = open(filePath, 'w')
    lines = []
    lines.extend(record_parameters(correctionMatrix))
    lines.append("MD5Version 10\n")
    lines.append("commandline \"\"\n")
    lines.append("\n")
    lines.append("numJoints " + str(len(bones)) + "\n")
    lines.append("numMeshes " + str(len(meshObjects)) + "\n")
    lines.append("\n")
    lines.extend(md5joints)
    for m in md5meshes: lines.extend(m)
    for line in lines: f.write(line)
    f.close()
    return

######################################################
# Create and write an .md5anim
######################################################
def write_md5anim(filePath, prerequisites, correctionMatrix, frameRange):
    goBack = bpy.context.scene.frame_current
    if frameRange == None:
        startFrame = bpy.context.scene.frame_start
        endFrame = bpy.context.scene.frame_end
    else:
        startFrame, endFrame = frameRange
    bones, meshObjects = prerequisites
    armObj = [o for o in bpy.data.objects if o.data == bones[0].id_data][0]
    pBones = armObj.pose.bones
    boneIndexLookup = {}
    for b in bones:
        boneIndexLookup[b.name] = bones.index(b)
    hierarchy = make_hierarchy_block(bones, boneIndexLookup)
    baseframe = make_baseframe_block(bones, correctionMatrix)
    bounds = []
    frames = []
    for frame in range(startFrame, endFrame + 1):
        bpy.context.scene.frame_set(frame)
        verts = []
        for mo in meshObjects:
            bm = bmesh.new()
            bm.from_object(mo)
            verts.extend([correctionMatrix * mo.matrix_world * v.co.to_4d()
                for v in bm.verts])
            bm.free()
        minX = min([co[0] for co in verts])
        minY = min([co[1] for co in verts])
        minZ = min([co[2] for co in verts])
        maxX = max([co[0] for co in verts])
        maxY = max([co[1] for co in verts])
        maxZ = max([co[2] for co in verts])
        bounds.append(\
        "  ( {:.6f} {:.6f} {:.6f} ) ( {:.6f} {:.6f} {:.6f} )\n".\
        format(minX, minY, minZ, maxX, maxY, maxZ))
        frameBlock = ["frame {} {{\n".format(frame - startFrame)]
        scaleFactor = correctionMatrix.to_scale()[0]
        for b in bones:
            pBone = pBones[b.name]
            pBoneMatrix = pBone.matrix
            if pBone.parent:
                diffMatrix = pBone.parent.matrix.inverted() * \
                             armObj.matrix_world * (pBoneMatrix * scaleFactor)
            else:
                diffMatrix = correctionMatrix * pBoneMatrix
            xPos, yPos, zPos = diffMatrix.translation
            xOrient, yOrient, zOrient =\
            (-diffMatrix.to_quaternion()).normalized()[1:]
            frameBlock.append(\
            "  {:.6f} {:.6f} {:.6f} {:.6f} {:.6f} {:.6f}\n".\
            format(xPos, yPos, zPos, xOrient, yOrient, zOrient))
        frameBlock.append("}\n")
        frameBlock.append("\n")
        frames.extend(frameBlock)
    f = open(filePath, 'w')
    numJoints = len(bones)
    bounds.insert(0, "bounds {\n")
    bounds.append("}\n")
    bounds.append("\n")
    lines = []
    lines.extend(record_parameters(correctionMatrix))
    lines.append("MD5Version 10\n")
    lines.append("commandline \"\"\n")
    lines.append("\n")
    lines.append("numFrames " + str(endFrame - startFrame + 1) + "\n")
    lines.append("numJoints " + str(numJoints) + "\n")
    lines.append("frameRate " + "24" + "\n")
    lines.append("numAnimatedComponents " + str(numJoints * 6) + "\n")
    lines.append("\n")
    for chunk in [hierarchy, bounds, baseframe, frames]:
        lines.extend(chunk)
    for line in lines:
        f.write(line)
    bpy.context.scene.frame_set(goBack)
    return

######################################################
# The material definition
######################################################
class Material:
   name = ""    #string
   def __init__(self, blenderMaterial):
      self.name = blenderMaterial.name
      # Set the texture name
      self.textureName = ""
      if(blenderMaterial.active_texture.type == 'IMAGE'):
         path,self.textureName = os.path.split(
               blenderMaterial.active_texture.image.filepath)
         self.ambient = blenderMaterial.ambient
         self.diffuse = blenderMaterial.diffuse_color
         self.specular = blenderMaterial.specular_color
         self.specularAlpha = blenderMaterial.specular_alpha
         self.shininess = blenderMaterial.emit

######################################################
# Define a key for an animation's name 
# according to DNT precedence
######################################################
def animationsKey(a):
   knowItens = ("idle",
                "walk",
                "die",
                "dead",
                "attack", "attackmeele",
                "run",
                "attackgun", "gunuse" )
   aLower = a.lower()
   if(aLower in knowItens):
      return knowItens.index(aLower)
   return 1000

######################################################
# Create and write an .md5def file
######################################################
def write_md5def(filePath, exportedAnimations, mat):
   # Set filename to export
   md5def_filename = filePath + ".md5def"
   
   # Let's get also the savepath without path!
   crudePath,crudeFileName = os.path.split(filePath)
   
   try:
      file = open(md5def_filename, 'w')
   except IOError:
      errmsg = "IOError " #%s: %s" % (errno, strerror)
   
   file.write("# Exported from Blender by DNT md5 exporter\n\n")
   
   # Get the relative path
   pos = crudePath.find("data")
   if(pos != -1):
      file.write("path = " + crudePath[pos+5:] + "/\n")
   else:
      file.write("path = \n");

   # Set others as default
   file.write("scale = 1.0\n");
   file.write("blendFrames = 4\n");
   file.write("\n# Mesh Information\n")

   # Write the mesh file name
   file.write("mesh = " + crudeFileName + ".md5mesh" + "\n")
   # Write each animation file
   file.write("\n# Animations\n")
   # Sort animations
   exportedAnimations.sort(key=animationsKey)
   for anim in exportedAnimations:
      file.write("animation = " + crudeFileName + "_" + anim+".md5anim" + "\n")
      
   # Write material
   file.write("\n# Material\n")
   file.write("ambient = " + str(mat.ambient) + " " + str(mat.ambient) + " " +
         str(mat.ambient) + " 1.0 \n")
   file.write("diffuse = " + str(mat.diffuse[0]) + " " + str(mat.diffuse[1]) +
         " " + str(mat.diffuse[2]) + " 1.0 \n")
   file.write("specular = " + str(mat.specular[0]) + " " +str(mat.specular[1]) +
         " " + str(mat.specular[2]) + " " + str(mat.specularAlpha) + "\n")
   file.write("shininess = " + str(mat.shininess) + "\n")
   file.write("image = " + mat.textureName + "\n")
   # Done!
   file.close()


########################################################################
#                                                                      #
#                    Interface and Check Functions                     #
#                                                                      #
########################################################################


import bpy
import bmesh
import mathutils
import math
from bpy.props import (BoolProperty,
                       FloatProperty,
                       StringProperty,
                       )
from bpy_extras.io_utils import (ExportHelper,
                                 ImportHelper,
                                 path_reference_mode,
                                 )

msgLines = [] # global for error messages
meshPre = None
animPre = None

######################################################
# Get a string related to the error message
######################################################
def message(id, *details):
    if id == "no_deformables":
        return ["No armature-deformed meshes are selected.",\
        "Select the meshes you want to export, and retry export."]
    elif id == "multiple_armatures":
        return ["The selected meshes use more than one armature.",\
        "Select the meshes using the same armature, and try again."]
    elif id == "no_armature":
        return ["No deforming armature is associated with the selection.",\
        "Select the model or models you want to export, and try again"]
    elif id == "no_bones":
        return ["The deforming armature has no bones.",\
        "Add all of the bones you want to export to the armature,"\
        "and retry export."]
    elif id == "missing_parents":
        return ["One or more bones have parents missing.",\
        "Offending bones:"] + details[0]
    elif id == "orphans":
        return ["There are multiple root bones (listed below)",\
        "in the export-bound collection, but only one root bone",\
        "is allowed in MD5. Revise your armature's membership,",\
        "and retry export.",\
        "Root bones:"] + details[0]
    elif id == "unweighted_verts":
        return ["The '" + details[0] + "' object contains vertices with",\
        "no deformation weights assigned. Valid MD5 data cannot be produced.",\
        "Paint the weights on all the vertices in the mesh,",\
        "and retry export."]
    elif id == "no_uvs":
        return ["The '" + details[0] + "' object has no UV coordinates.",\
        "Valid MD5 data cannot be produced. Unwrap the object",\
        "or exclude it from your selection, and retry export."]

######################################################
# Check if everything is ok to do a export
# \param what: "anim" or "mesh"
# \param selection: current selected objects
######################################################
def is_export_go(what, selection):
    meshObjects = [o for o in selection
        if o.data in bpy.data.meshes[:] and o.find_armature()]
    armatures = [a.find_armature() for a in meshObjects]
    if not meshObjects:
        return ["no_deformables", None]
    armature = armatures[0]
    if armatures.count(armature) < len(meshObjects):
        return ["multiple_armatures", None]
    bones = [b for b in armature.data.bones]
    if not bones:
        return ["no_bones", None]
    rootBones = [i for i in bones if ((not i.parent) and (i.name[0] != '_'))]
    if len(rootBones) > 1:
        boneList = []
        for rb in rootBones:
            boneList.append("- " + str(rb.name))
        return ["orphans", boneList]
    abandonedBones = [i for i in bones
        if i.parent and i.parent not in bones[:]]
    if abandonedBones:
        boneList = []
        for ab in abandonedBones:
            boneList.append("- " + str(ab.name))
        return ["missing_parents", boneList]
    if what != "anim":
        for mo in meshObjects:
            bm = bmesh.new()
            bm.from_mesh(mo.data)
            weightData = bm.verts.layers.deform.active
            unweightedVerts = [v for v in bm.verts if not v[weightData]]
            uvLayer = bm.loops.layers.uv.active
            bm.free()
            if unweightedVerts:
                return ["unweighted_verts", mo.name]
            if not uvLayer:
                return ["no_uvs", mo.name]
    
    return ["ok", (bones, meshObjects)]

######################################################
# Show a blender message with some error occurred
######################################################
class MD5ErrorMsg(bpy.types.Operator):
    global msgLines
    bl_idname = "object.md5_error_msg"
    bl_label = "Show DNT .md5 export failure reason"
    def draw(self, context):
        layout = self.layout
        layout.label(icon='ERROR',text="DNT .md5* Export Error")
        frame = layout.box()
        frame.separator
        for l in msgLines:
            layout.label(text=l)
        return
    def execute(self, context):
        return {'CANCELLED'}
    def invoke(self, context, event):
        wm = context.window_manager
        return wm.invoke_popup(self, width=600)

######################################################
# Do the DNT MD5 export check and call
######################################################
class DNT_MD5Export(bpy.types.Operator):
    '''Export to DNT md5 (.md5def/.md5mesh/.md5anim)'''
    bl_idname = "export_scene.dnt_md5export"
    bl_label = 'Export MD5ANIM'
    def invoke(self, context, event):
        global msgLines, meshPre, animPre

        selection = context.selected_objects

        # check and set pre-requisites for mesh
        checkResult = is_export_go("mesh", selection)
        if checkResult[0] == "ok":
            meshPre = checkResult[-1]
        else:
            # Error on check, show error message!
            msgLines = message(checkResult[0], checkResult[1])
            for l in msgLines:
                print(l)
            return bpy.ops.object.md5_error_msg('INVOKE_DEFAULT')

        # check and set pre-requisites for anim
        checkResult = is_export_go("anim", selection)
        if checkResult[0] == "ok":
            animPre = checkResult[-1]
        else:
            msgLines = message(checkResult[0], checkResult[1])
            for l in msgLines:
                print(l)
            return bpy.ops.object.md5_error_msg('INVOKE_DEFAULT')

        # Export the mesh and animations
        return bpy.ops.export_scene.dntmd5('INVOKE_DEFAULT')

######################################################
# Really export the md5 files (with a dialog 
# to get input values)
######################################################
class ExportDNTMD5(bpy.types.Operator, ExportHelper):
    '''Save DNT md5 files (.md5def/.md5mesh/.md5anim)'''
    global meshPre, animPre
    bl_idname = "export_scene.dntmd5"
    bl_label = 'Export DNT'
    bl_options = {'PRESET'}
    filename_ext = ""
    path_mode = path_reference_mode
    check_extension = False
    def execute(self, context):
        scaleFactor = 1.0
        # First, export the meshes
        scaleTweak = mathutils.Matrix.Scale(scaleFactor, 4)
        correctionMatrix = scaleTweak
        write_md5mesh(self.filepath+".md5mesh", meshPre, correctionMatrix)

        # Let's try to export all animations related to the armature

        # Set correction matrix
        scaleTweak = mathutils.Matrix.Scale(scaleFactor, 4)
        correctionMatrix = scaleTweak

        # Get all selected meshes
        selection = context.selected_objects
        meshObjects = [o for o in selection if o.data in bpy.data.meshes[:]]

        previousAction = None
        previousFrame = None

        # Save previous action to restore latter
        armature = meshObjects[0].find_armature()
        
        if(armature.animation_data):
           previousAction = armature.animation_data.action
           previousFrame = bpy.context.scene.frame_current

        # Export each animation
        exportedAnimations = []
        for arm_action in bpy.data.actions:
            # Set the active action to current
            #for ob in meshObjects:
            #   if(ob.type == 'MESH'):
            #      ob.animation_data.action = arm_action
            armature.animation_data.action = arm_action
            bpy.context.scene.update()

            # Set range
            rangestart = int( arm_action.frame_range[0] )
            rangeend = int( arm_action.frame_range[1] )

            # Export the current action 
            write_md5anim(
                self.filepath + "_" + arm_action.name + ".md5anim", 
                animPre, correctionMatrix, [rangestart, rangeend])
            exportedAnimations.append(arm_action.name)

        # Restore the previous active action and frame
        if(previousAction):
           #for ob in meshObjects:
           #    ob.animation_data.action = previousAction
           armature.animation_data.action = previousAction
           bpy.context.scene.frame_set(previousFrame)

        # And finally, export the .md5def file
        material = Material(meshObjects[0].data.materials[0])
        write_md5def(self.filepath, exportedAnimations, material)

        # Done!
        return {'FINISHED'}

######################################################
# The function to call when selected on menu
######################################################
def menu_func_export_dnt(self, context):
    self.layout.operator(DNT_MD5Export.bl_idname, 
                         text="DNT md5 model (.md5def/.md5mesh/.md5anim)")

######################################################
# Register script components
######################################################
def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func_export_dnt)

######################################################
# Unregister script components
######################################################
def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func_export_dnt)

######################################################
# Main
######################################################
if __name__ == "__main__":
    register()

