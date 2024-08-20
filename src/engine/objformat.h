#ifndef SRC_ENGINE_OBJFORMAT_H
#define SRC_ENGINE_OBJFORMAT_H
#include <core/definitions.h>
#include <core/linear.h>
#include <core/arena.h>

// --- Wavefront Objects -------------------------------------------------------
//
// https://cs418.cs.illinois.edu/website/text/obj.html
//
// Reference above, since 3D modelling by hand is a pain in the literal ass to
// hand-program, I figure I'll bite the bullet and just write the object file
// parser and be done with it. To do this, we will need to first reserve space
// for the file in memory, load it in, and then generate the object.
//
// - Identifier: "v"
//      Denotes a vertex point in the model.
//      Can be 3 or 6 values which denote vertex position & color respectively.
//
// - Identifier: "vn"
//      Denotes a vertex normal.
//      Can be 3 values that denote the vertex normal.
//
// - Identifier: "vt"
//      Denotes a vertex texture-coordinate.
//      Can be 2 values that denote the texture coordinates.
//
// - Identifier: "f"
//      Denotes a face.
//      Can be 3+ values, where each successive one is an adjoining triangle
//      such that the leading number is the start, and each pair of two indicate
//      a set that references the first.
//
//      f 2 3 4 5 6 7 8 -> 2, 3, 4 + 2, 4, 5 + 2, 5, 6 + 2, 6, 7, + 2, 7, 8...
//
//      Index specifications are separated by slashes, with trailing slashes ignored.
//      For example: f 1//1 2//2 3//3 is v//vt with a null vn.
//
// - Identifier: "#"
//      Denotes a comment.
//
// The absence of vn/vt is acceptable.
// The offset positions of the object can be anywhere, so we may want to recenter.
// Object orientation itself is also arbitrary.
//

typedef struct wavefront_object_face
{
    u32 vertex_index;   
    u32 normal_index;
    u32 texture_coordinate_index;
} wavefront_object_face;

typedef struct wavefront_object_mesh
{
    v3 *vertices;
    v3 *color;
    v3 *normals;
    v2 *texture_coords;
    wavefront_object_face *faces;
    u64 vertex_count;
    u64 color_count;
    u64 normal_count;
    u64 texture_coord_count;
    u64 face_count;
} wavefront_object_mesh;

b32 wavefront_object_parse_file(ccptr object_file, wavefront_object_mesh *mesh, memory_arena *arena);

#endif
