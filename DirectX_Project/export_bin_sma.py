import bpy
import bmesh
import io
import mathutils

from struct import (pack, unpack)

class VertexWeight():
    __slots__ = ('index', 'weight')
    def __init__(self, ind, w):
        self.index = ind
        self.weight = w

class VertexWeights():
    __slots__ = ('vertex_weights')
    def __init__(self):
        self.vertex_weights = []
        
    def addVW(self, vw):
        self.vertex_weights.append(vw)
    
    def __len__(self):
         return len(self.vertex_weights)
    
    def write(self, out_file):
        out_file.write(pack('<H', len(self.vertex_weights))) 
#        print("vertex weights", len(self.vertex_weights))
        for vw in self.vertex_weights:
            out_file.write(pack('<h', vw.index))
            out_file.write(pack('<f', vw.weight))
            #print('index', vw.index, 'weight', vw.weight)

class Texture():
    __slots__ = ('name', 'indexes')
    def __init__(self, n, i):
        self.name = n
        self.indexes = [i]
    def addIndex(self, i):
        if i:
            self.indexes.append(i) 

class Textures():
    __slots__ = ('textures')
    def __init__(self):
        self.textures = []
    def addFaceTexture(self, texname, index):
        texWasAdded = 0
        for tex in self.textures:
            if tex.name == texname:
                tex.addIndex(index)
                texWasAdded = 1
        if texWasAdded == 0:
            newtex = Texture(texname, index)
            self.textures.append(newtex)
    def write(self, out_file):
        out_file.write(pack('<H', len(self.textures)))
        for tex in self.textures:
            buffer = tex.name.encode(encoding='UTF-8', errors='strict')
            if not buffer:
                buffer = bytes()
            out_file.write(pack('<{}s'.format(64), buffer))
            out_file.write(pack('<H', len(tex.indexes)))
            for i in tex.indexes:
                out_file.write(pack('<H', i))
        
class Bone():
    __slots__ = ('parent_idx', 'name', 'rot_x', 'rot_y', 'rot_z', 'pos_x', 'pos_y', 'pos_z')
    def __init__(self, pi = 0, n = ' ', rx = 0,ry = 0,rz = 0, px = 0, py = 0, pz = 0):
        self.parent_idx = pi
        self.name = n
        self.rot_x = rx
        self.rot_y = ry
        self.rot_z = rz
        self.pos_x = px
        self.pos_y = py
        self.pos_z = pz
        
    def print(self):
        print(self.name)
        print('Rotation', self.rot_x, self.rot_y, self.rot_z)
        print('Position', self.pos_x, self.pos_y, self.pos_z)
 
class Vector3f():
    __slots__ = ('x', 'y', 'z')
    def __init__(self, ix = 0, iy = 0, iz = 0):
        self.x = ix
        self.y = iy
        self.z = iz
        
class Keyframe():
    __slots__ = ('index', 'positions', 'rotations')
    def __init__(self):
        self.index = 0
        self.positions = []
        self.rotations = []
    def addRotation(self, r):
        self.rotations.append(r)
    def addPosition(self, p):
        self.positions.append(p)

class Animation():
    __slots__ = ('name', 'numKeyframes', 'keyframe')
    def __init__(self):
        self.name = 'None'
        self.numKeyframes = 0
        self.keyframe = []
    
    def addKeyframe(self, kf):
        self.keyframe.append(kf)

class SmaModel():
    __slots__ = ('name', 'vertices', 'normals', 'texcoords', 'textures', 'vertex_weights', 'bones', 'animations')
    def __init__(self, def_name = ""):
        if(def_name is None):
            def_name = " "
        self.name = def_name
        self.vertices = []
        self.normals = []
        self.texcoords = []
#        self.textures = 
        self.vertex_weights = []
        self.bones = []
        self.animations = []
    
    def number_vertices(self):
        if self.vertices is None:
            return 0
        return len(self.vertices)
    
    def addVertices(self, verts):
        for v in verts:
            self.vertices.append(v)
            
    def addNormals(self, norms):
        for n in norms:
            self.normals.append(n)
    
    def addUvs(self, uvs):
        for uv in uvs:
            self.texcoords.append(uv)
        
    def addTextures(self, texs):
        self.textures = texs
    
    def addVertexWeight(self, vws):
        for vw in vws:
            self.vertex_weights.append(vw)
            
    def addBones(self, bones):
        for b in bones:
            self.bones.append(b)
    
    def addAnimations(self, anims):
        for a in anims:
            self.animations.append(a)
        
    def getBoneIndex(self, bname):
        index = 0
        for b in self.bones:
            if b.name == bname:
                return index
            index += 1
        return -1;
            
    def print(self):
        print(self.name)
        print("number_vertices",(len(self.vertices))/3)
        #3for v in self.vertices:
        #    print(v)
        print("number_normals",(len(self.normals))/3)
        #for n in self.normals:
        #    print(n)
        print("number_texcoord",(len(self.texcoords))/2)
        #for uv in self.texcoords:
        #    print(uv)
        idx = 0
        print('number weights', len(self.vertex_weights))
        print('number bones', len(self.bones))
        print('number animations', len(self.animations))
       # for vert_w in vertex_weights:
        #    for vw in vert_w.vertex_weights:
        #        print('index ', idx, 'bone ' , vw.index, ' weight ', vw.weight)
        #    idx += 1    

    def write(self, fileName):
        try:
            with io.FileIO(fileName, "wb") as out_file:
                
                formatName = "SMA"
                buffer1 = formatName.encode(encoding='UTF-8', errors='strict')
                if not buffer1:
                    buffer1 = bytes()
                out_file.write(pack('<{}s'.format(8), buffer1))
                
                version = 1
                out_file.write(pack('<H', version))
                
                buffer = self.name.encode(encoding='UTF-8', errors='strict')
                if not buffer:
                    buffer = bytes()
                out_file.write(pack('<{}s'.format(32), buffer))
                
                out_file.write(pack('<H', len(self.vertices)))                
                for v in self.vertices:
                    out_file.write(pack('<f', v))
                
                out_file.write(pack('<H', len(self.normals)))                
                for n in self.normals:
                    out_file.write(pack('<f', n))
                
                out_file.write(pack('<H', len(self.texcoords)))                
                for uv in self.texcoords:
                    out_file.write(pack('<f', uv))
                
                self.textures.write(out_file)
                    
                print('Saving bones')
                out_file.write(pack('<H', len(self.bones)))
                for bone in self.bones:
                    out_file.write(pack('<h', bone.parent_idx))
                    out_file.write(pack('<f', bone.rot_x))
                    out_file.write(pack('<f', bone.rot_y))
                    out_file.write(pack('<f', bone.rot_z))
                    out_file.write(pack('<f', bone.pos_x))
                    out_file.write(pack('<f', bone.pos_y))
                    out_file.write(pack('<f', bone.pos_z))
                    #bone.print()
                
                out_file.write(pack('<H', len(self.vertex_weights)))  
                for vert_w in self.vertex_weights:                     
                    vert_w.write(out_file)
                
                out_file.write(pack('<H', len(self.animations))) 
                for a in self.animations:
                    abuffer = a.name.encode(encoding='UTF-8', errors='strict')
                    if not abuffer:
                        abuffer = bytes()
                    out_file.write(pack('<{}s'.format(64), abuffer))
                    out_file.write(pack('<H', a.numKeyframes))
                    #print("Num keyframes", a.numKeyframes)
                   # print(a.name)
                    for kf in a.keyframe:
                        #print("kf index", kf.index)
                        out_file.write(pack('<H', kf.index))
                        idx = 0
                        for rot in kf.rotations:
                            out_file.write(pack('<f', kf.rotations[idx].x))
                            out_file.write(pack('<f', kf.rotations[idx].y))
                            out_file.write(pack('<f', kf.rotations[idx].z))
                            out_file.write(pack('<f', kf.positions[idx].x))
                            out_file.write(pack('<f', kf.positions[idx].y))
                            out_file.write(pack('<f', kf.positions[idx].z))
                            #print(idx)
                            idx += 1
                        
                
                out_file.flush()
                out_file.close()
        finally:
            pass

print('==================== begin ===================')

meshes = [obj for obj in bpy.data.objects if obj.type == 'MESH']
me = meshes[0].data
bm = bmesh.new()
bm.from_mesh(me)
bmesh.ops.recalc_face_normals(bm, faces=bm.faces)

sma = SmaModel(bpy.context.object.name)

#layer_uv = bm.loops.layers.uv[0]
layer_uv = bm.loops.layers.uv.active

for f in bm.faces:
    for v in f.verts:
        sma.addVertices(v.co)
    #recalc normals - flat shaded
    normal = f.normal
    normal.normalize()    
    sma.addNormals(normal)
    sma.addNormals(normal)
    sma.addNormals(normal)          
    
    #uvs
    uvs = [f.loops[0][layer_uv].uv.x, 1-f.loops[0][layer_uv].uv.y]
    sma.addUvs(uvs)
    uvs = [f.loops[1][layer_uv].uv.x, 1-f.loops[1][layer_uv].uv.y]
    sma.addUvs(uvs)
    uvs = [f.loops[2][layer_uv].uv.x, 1-f.loops[2][layer_uv].uv.y]
    sma.addUvs(uvs)

#print(len(bm.faces))

print('==================== textures ===================')

texture_list = []
for obj in bpy.context.scene.objects:
    if obj.type == 'MESH':
        for f in obj.data.polygons:
            mat = obj.material_slots[f.material_index].material
            if mat and mat.use_nodes:
                for n in mat.node_tree.nodes:
                    if n.type == 'TEX_IMAGE':
                        if n.image.name not in texture_list:
                            texture_list += [n.image.name]
                        #print('Face', f.index, 'uses', n.image.name)

print(texture_list)

texture = []

my_textures = Textures()
face_counter = 0

for tex in texture_list:    
    for obj in bpy.context.scene.objects:
        if obj.type == 'MESH':
            for f in obj.data.polygons:
                mat = obj.material_slots[f.material_index].material
                if mat and mat.use_nodes:
                    for n in mat.node_tree.nodes:
                        if n.type == 'TEX_IMAGE':
                            if n.image.name == tex:
                                my_textures.addFaceTexture(tex, face_counter)
            face_counter += 1
            
sma.addTextures(my_textures)

print('==================== armature ===================')

armatures = [obj for obj in bpy.data.objects if obj.type == 'ARMATURE']

if len(armatures) == 0:
    print('No armatures detected')
    sys.exit(-1)
if len(armatures) > 1:
    print('Too many armatures')
    sys.exit(-1)

armature = armatures[0]
#print('Armature name = ', armature.name)
#print('Armature location = ', armature.location)
#print('Armature rotation = ', armature.rotation_euler)
        
print('==================== bones ===================')  

myBones = []
for bone in armature.data.bones:   
    print(bone.name) 
    print(bone.matrix_local)
    loc, rot, scale = bone.matrix_local.decompose()    
    loc += armature.location
    #print('arma loc', armature.location)
    rot = rot.to_euler() 

    index = -1
    counter = 0
    for b in armature.data.bones:
        if b == bone.parent:
            index = counter
        counter += 1  
    b = Bone(index, bone.name, rot.x, rot.y, rot.z, loc.x, loc.y, loc.z)
    myBones.append(b)    

sma.addBones(myBones)

print('==================== weights ===================')

#arm = bpy.data.objects[armature.name]
obj = bpy.data.objects[sma.name]

obj_verts = obj.data.vertices
obj_group_names = [g.name for g in obj.vertex_groups]
#print(obj_group_names)
vertex_weights = []
for v in obj_verts:
    vertex_w = VertexWeights()
    weight_sum = 0
    for vgroup in v.groups:
        weight_sum += vgroup.weight
    if weight_sum < 0:
        normalize_koef = 1
    else:
        normalize_koef = 1 / weight_sum    
   # print(normalize_koef)
    for vgroup in v.groups:      
        print('Vertex',v.index,'has a weight of',vgroup.weight * normalize_koef,'for bone',vgroup.group)     
        vw = VertexWeight(sma.getBoneIndex(obj_group_names[vgroup.group]), vgroup.weight * normalize_koef)        
        #print(sma.getBoneIndex(obj_group_names[vgroup.group]))
        vertex_w.addVW(vw)
    vertex_weights.append(vertex_w)
        
final_weights = []
for f in bm.faces:
    for v in f.verts:
        final_weights.append(vertex_weights[v.index])

sma.addVertexWeight(final_weights)

print('==================== animation ===================')  

animations = []
for mesh in [obj for obj in bpy.data.objects if obj.type == 'MESH']:    
    for action in bpy.data.actions:
        animation = Animation()
        animation.name = action.name
        #print(action.name)
        mesh.animation_data.action = action
        frame_begin, frame_end = [int(x) for x in action.frame_range]
        for frame in range(frame_begin, frame_end + 1):            
            bpy.context.scene.frame_set(frame)
            keyframe = Keyframe()
            keyframe.index = int(frame)
            #print("Frame %i" % frame)
            for pbone in armature.pose.bones:
                #print(pbone.name, pbone.matrix)
                mat = pbone.matrix
                loc, rot, scale = pbone.matrix.decompose()    
                #print(armature.location)
                loc += armature.location
                rot = rot.to_euler()            

                keyframe.addRotation(rot)
                keyframe.addPosition(loc)
			#	loc, rot, scale = mat.decompose() 
                #export_pose(armature_object.pose.bones)
                
            animation.addKeyframe(keyframe)
                            
        animation.numKeyframes = len(animation.keyframe)        
        animations.append(animation)

sma.addAnimations(animations)    
    
print('==================== show data ===================') 

sma.print()

print('==================== save all ===================')  

fileName = "{}.sma".format(sma.name)
sma.write(fileName)

bm.free()