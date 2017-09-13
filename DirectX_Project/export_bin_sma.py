import bpy
import bmesh
import io
import mathutils

from struct import (pack, unpack)

class SmaModel():
    __slots__ = ('name', 'vertices', 'normals', 'texcoords')
    def __init__(self, def_name = ""):
        if(def_name is None):
            def_name = " "
        self.name = def_name
        self.vertices = []
        self.normals = []
        self.texcoords = []
    
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
            
    def print(self):
        print(self.name)
        print("number_vertices={}".format(len(self.vertices)))
        #3for v in self.vertices:
        #    print(v)
        print("number_normals={}".format(len(self.normals)))
        #for n in self.normals:
        #    print(n)
        print("number_texcoord={}".format(len(self.texcoords)))
        #for uv in self.texcoords:
        #    print(uv)
    
    def write(self, fileName):
        try:
            with io.FileIO(fileName, "wb") as out_file:
                
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
                    
                #debug_out = ms3d_model.write(out_file)
                out_file.flush()
                out_file.close()
        finally:
            pass

me = bpy.context.object.data
bm = bmesh.new()
bm.from_mesh(me)
bmesh.ops.triangulate(bm, faces=bm.faces)
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

print(len(bm.faces))

sma.print()

fileName = "{}.sma".format(sma.name)
sma.write(fileName)

bm.free()


