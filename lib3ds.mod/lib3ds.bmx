
Strict

Module sys87.lib3ds
ModuleInfo "Author: Kevin Primm"
ModuleInfo "License: MIT"

Import "src/*.h"
Import "src/lib3ds_atmosphere.c"
Import "src/lib3ds_background.c"
Import "src/lib3ds_camera.c"
Import "src/lib3ds_chunk.c"
Import "src/lib3ds_chunktable.c"
Import "src/lib3ds_file.c"
Import "src/lib3ds_io.c"
Import "src/lib3ds_light.c"
Import "src/lib3ds_material.c"
Import "src/lib3ds_math.c"
Import "src/lib3ds_matrix.c"
Import "src/lib3ds_mesh.c"
Import "src/lib3ds_node.c"
Import "src/lib3ds_quat.c"
Import "src/lib3ds_shadow.c"
Import "src/lib3ds_track.c"
Import "src/lib3ds_util.c"
Import "src/lib3ds_vector.c"
Import "src/lib3ds_viewport.c"

Const LIB3DS_SEEK_SET     = 0
Const LIB3DS_SEEK_CUR     = 1
Const LIB3DS_SEEK_END     = 2

Const LIB3DS_LOG_ERROR    = 0
Const LIB3DS_LOG_WARN     = 1
Const LIB3DS_LOG_INFO     = 2
Const LIB3DS_LOG_DEBUG    = 3

Const LIB3DS_VIEW_NOT_USED   = 0
Const LIB3DS_VIEW_TOP        = 1
Const LIB3DS_VIEW_BOTTOM     = 2
Const LIB3DS_VIEW_LEFT       = 3
Const LIB3DS_VIEW_RIGHT      = 4
Const LIB3DS_VIEW_FRONT      = 5
Const LIB3DS_VIEW_BACK       = 6
Const LIB3DS_VIEW_USER       = 7
Const LIB3DS_VIEW_SPOTLIGHT  = 18
Const LIB3DS_VIEW_CAMERA     = 65535

Const LIB3DS_LAYOUT_SINGLE                    = 0
Const LIB3DS_LAYOUT_TWO_PANE_VERT_SPLIT       = 1
Const LIB3DS_LAYOUT_TWO_PANE_HORIZ_SPLIT      = 2
Const LIB3DS_LAYOUT_FOUR_PANE                 = 3
Const LIB3DS_LAYOUT_THREE_PANE_LEFT_SPLIT     = 4
Const LIB3DS_LAYOUT_THREE_PANE_BOTTOM_SPLIT   = 5
Const LIB3DS_LAYOUT_THREE_PANE_RIGHT_SPLIT    = 6
Const LIB3DS_LAYOUT_THREE_PANE_TOP_SPLIT      = 7
Const LIB3DS_LAYOUT_THREE_PANE_VERT_SPLIT     = 8
Const LIB3DS_LAYOUT_THREE_PANE_HORIZ_SPLIT    = 9
Const LIB3DS_LAYOUT_FOUR_PANE_LEFT_SPLIT      = 10
Const LIB3DS_LAYOUT_FOUR_PANE_RIGHT_SPLIT     = 11

Const LIB3DS_TEXTURE_DECALE       = $0001
Const LIB3DS_TEXTURE_MIRROR       = $0002
Const LIB3DS_TEXTURE_NEGATE       = $0008
Const LIB3DS_TEXTURE_NO_TILE      = $0010
Const LIB3DS_TEXTURE_SUMMED_AREA  = $0020
Const LIB3DS_TEXTURE_ALPHA_SOURCE = $0040
Const LIB3DS_TEXTURE_TINT         = $0080
Const LIB3DS_TEXTURE_IGNORE_ALPHA = $0100
Const LIB3DS_TEXTURE_RGB_TINT     = $0200

Const LIB3DS_AUTOREFL_USE                     = $0001
Const LIB3DS_AUTOREFL_READ_FIRST_FRAME_ONLY   = $0004
Const LIB3DS_AUTOREFL_FLAT_MIRROR             = $0008 

Const LIB3DS_SHADING_WIRE_FRAME = 0
Const LIB3DS_SHADING_FLAT       = 1 
Const LIB3DS_SHADING_GOURAUD    = 2 
Const LIB3DS_SHADING_PHONG      = 3 
Const LIB3DS_SHADING_METAL      = 4


Const LIB3DS_OBJECT_HIDDEN          = $01 
Const LIB3DS_OBJECT_VIS_LOFTER      = $02 
Const LIB3DS_OBJECT_DOESNT_CAST     = $04 
Const LIB3DS_OBJECT_MATTE           = $08 
Const LIB3DS_OBJECT_DONT_RCVSHADOW  = $10 
Const LIB3DS_OBJECT_FAST            = $20 
Const LIB3DS_OBJECT_FROZEN          = $40 

Const LIB3DS_MAP_NONE           = -1
Const LIB3DS_MAP_PLANAR         = 0
Const LIB3DS_MAP_CYLINDRICAL    = 1
Const LIB3DS_MAP_SPHERICAL      = 2

Const LIB3DS_FACE_VIS_AC    = $01
Const LIB3DS_FACE_VIS_BC    = $02 
Const LIB3DS_FACE_VIS_AB    = $04 
Const LIB3DS_FACE_WRAP_U    = $08
Const LIB3DS_FACE_WRAP_V    = $10 
Const LIB3DS_FACE_SELECT_3  = (1 Shl 13) 
Const LIB3DS_FACE_SELECT_2  = (1 Shl 14)    
Const LIB3DS_FACE_SELECT_1  = (1 Shl 15)

Const LIB3DS_NODE_AMBIENT_COLOR   = 0
Const LIB3DS_NODE_MESH_INSTANCE   = 1
Const LIB3DS_NODE_CAMERA          = 2
Const LIB3DS_NODE_CAMERA_TARGET   = 3
Const LIB3DS_NODE_OMNILIGHT       = 4
Const LIB3DS_NODE_SPOTLIGHT       = 5
Const LIB3DS_NODE_SPOTLIGHT_TARGET= 6

Const LIB3DS_NODE_HIDDEN           = $000800
Const LIB3DS_NODE_SHOW_PATH        = $010000
Const LIB3DS_NODE_SMOOTHING        = $020000
Const LIB3DS_NODE_MOTION_BLUR      = $100000
Const LIB3DS_NODE_MORPH_MATERIALS  = $400000

Const LIB3DS_KEY_USE_TENS         = $01
Const LIB3DS_KEY_USE_CONT         = $02
Const LIB3DS_KEY_USE_BIAS         = $04
Const LIB3DS_KEY_USE_EASE_TO      = $08
Const LIB3DS_KEY_USE_EASE_FROM    = $10

Const LIB3DS_TRACK_BOOL   = 0
Const LIB3DS_TRACK_FLOAT  = 1
Const LIB3DS_TRACK_VECTOR = 3
Const LIB3DS_TRACK_QUAT   = 4

Const LIB3DS_TRACK_REPEAT   = $0001
Const LIB3DS_TRACK_SMOOTH   = $0002
Const LIB3DS_TRACK_LOCK_X   = $0008
Const LIB3DS_TRACK_LOCK_Y   = $0010
Const LIB3DS_TRACK_LOCK_Z   = $0020
Const LIB3DS_TRACK_UNLINK_X = $0100
Const LIB3DS_TRACK_UNLINK_Y = $0200
Const LIB3DS_TRACK_UNLINK_Z = $0400

Extern
	Function lib3ds_file_open:Byte Ptr(filename:Byte Ptr)
	Function lib3ds_file_save(file:Byte Ptr, filename:Byte Ptr)
	Function lib3ds_file_new:Byte Ptr()
	Function lib3ds_file_free(file:Byte Ptr)
	Function lib3ds_file_eval(file:Byte Ptr, t#)
	Function lib3ds_file_read(file:Byte Ptr, io:Byte Ptr)
	Function lib3ds_file_write(file:Byte Ptr, io:Byte Ptr)
	Function lib3ds_file_reserve_materials(file:Byte Ptr, size, force)
	Function lib3ds_file_insert_material(file:Byte Ptr, material:Byte Ptr, index)
	Function lib3ds_file_remove_material(file:Byte Ptr, index)
	Function lib3ds_file_material_by_name(file:Byte Ptr, name:Byte Ptr)
	Function lib3ds_file_reserve_cameras(file:Byte Ptr, size, force)
	Function lib3ds_file_insert_camera(file:Byte Ptr, camera:Byte Ptr, index)
	Function lib3ds_file_remove_camera(file:Byte Ptr, index)
	Function lib3ds_file_camera_by_name(file:Byte Ptr, name:Byte Ptr)
	Function lib3ds_file_reserve_lights(file:Byte Ptr, size, force)
	Function lib3ds_file_insert_light(file:Byte Ptr, light:Byte Ptr, index)
	Function lib3ds_file_remove_light(file:Byte Ptr, index)
	Function lib3ds_file_light_by_name(file:Byte Ptr, name:Byte Ptr)
	Function lib3ds_file_reserve_meshes(file:Byte Ptr, size, force)
	Function lib3ds_file_insert_mesh(file:Byte Ptr, mesh:Byte Ptr, index)
	Function lib3ds_file_remove_mesh(file:Byte Ptr, index)
	Function lib3ds_file_mesh_by_name(file:Byte Ptr, name:Byte Ptr)
	Function lib3ds_file_mesh_for_node:Byte Ptr(file:Byte Ptr, node:Byte Ptr)
	Function lib3ds_file_node_by_name:Byte Ptr(file:Byte Ptr, name:Byte Ptr, type_)
	Function lib3ds_file_node_by_id:Byte Ptr(file:Byte Ptr, node_id:Short)
	Function lib3ds_file_append_node(file:Byte Ptr, node:Byte Ptr, parent:Byte Ptr)
	Function lib3ds_file_insert_node(file:Byte Ptr, node:Byte Ptr, at:Byte Ptr)
	Function lib3ds_file_remove_node(file:Byte Ptr, node:Byte Ptr)
	Function lib3ds_file_minmax_node_id(file:Byte Ptr, min_id:Short Ptr, max_id:Short Ptr)
	Function lib3ds_file_create_nodes_for_meshes(file:Byte Ptr)
	Function lib3ds_file_bounding_box_of_objects(file:Byte Ptr, include_meshes, include_cameras, include_lights, bmin:Float Ptr, bmax:Float Ptr)
	Function lib3ds_file_bounding_box_of_nodes(file:Byte Ptr, include_meshes, include_cameras, include_lights, bmin:Float Ptr, bmax:Float Ptr, matrix:Float Ptr)
	
	Function lib3ds_material_new:Byte Ptr(name:Byte Ptr)
	Function lib3ds_material_free(material:Byte Ptr)
	Function lib3ds_camera_new:Byte Ptr(name:Byte Ptr)
	Function lib3ds_camera_free(camera:Byte Ptr)
	Function lib3ds_light_new:Byte Ptr(name:Byte Ptr)
	Function lib3ds_light_free(light:Byte Ptr)
	Function lib3ds_mesh_new:Byte Ptr(name:Byte Ptr)
	Function lib3ds_mesh_free(mesh:Byte Ptr)
	Function lib3ds_mesh_resize_vertices(mesh:Byte Ptr, nvertices, use_texcos, use_flags)
	Function lib3ds_mesh_resize_faces(mesh:Byte Ptr, nfaces)
	Function lib3ds_mesh_bounding_box(mesh:Byte Ptr, bmin:Float Ptr, bmax:Float Ptr)
	Function lib3ds_mesh_calculate_face_normals(mesh:Byte Ptr, face_normals:Float Ptr)
	Function lib3ds_mesh_calculate_vertex_normals(mesh:Byte Ptr, normals:Float Ptr)
	
	Function lib3ds_node_new:Byte Ptr(type_)
	Function lib3ds_node_new_ambient_color:Byte Ptr(color0:Float Ptr)
	Function lib3ds_node_new_mesh_instance:Byte Ptr(mesh:Byte Ptr, instance_name:Byte Ptr, pos0:Float Ptr, scl0:Float Ptr, rot0:Float Ptr)
	Function lib3ds_node_new_camera:Byte Ptr(camera:Byte Ptr)
	Function lib3ds_node_new_camera_target:Byte Ptr(camera:Byte Ptr)
	Function lib3ds_node_new_omnilight:Byte Ptr(light:Byte Ptr)
	Function lib3ds_node_new_spotlight:Byte Ptr(light:Byte Ptr)
	Function lib3ds_node_new_spotlight_target:Byte Ptr(light:Byte Ptr)
	Function lib3ds_node_free(node:Byte Ptr)
	Function lib3ds_node_eval(node:Byte Ptr, t#)
	Function lib3ds_node_by_name:Byte Ptr(node:Byte Ptr, name:Byte Ptr, type_)
	Function lib3ds_node_by_id:Byte Ptr(node:Byte Ptr, node_id:Short)
	
	Function lib3ds_track_new:Byte Ptr(type_, nkeys)
	Function lib3ds_track_free(track:Byte Ptr)
	Function lib3ds_track_resize(track:Byte Ptr, nkeys)
	Function lib3ds_track_eval_bool(track:Byte Ptr, b:Int Ptr, t#)
	Function lib3ds_track_eval_float(track:Byte Ptr, f:Float Ptr, t#)
	Function lib3ds_track_eval_vector(track:Byte Ptr, v:Float Ptr, t#)
	Function lib3ds_track_eval_quat(track:Byte Ptr, q:Float Ptr, t#)
	
	Function lib3ds_math_ease#(fp#, fc#, fn#, ease_from#, ease_to#)
	Function lib3ds_math_cubic_interp(v:Float Ptr, a:Float Ptr, p:Float Ptr, q:Float Ptr, b:Float Ptr, n, t#)
	
	Function lib3ds_vector_copy(dst:Float Ptr, src:Float Ptr)
	Function lib3ds_vector_neg(c:Float Ptr)
	Function lib3ds_vector_make(c:Float Ptr, x#, y#, z#)
	Function lib3ds_vector_zero(c:Float Ptr)
	Function lib3ds_vector_add(c:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_vector_sub(c:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_vector_scalar_mul(c:Float Ptr, a:Float Ptr, k#)
	Function lib3ds_vector_cross(c:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_vector_dot#(a:Float Ptr, b:Float Ptr)
	Function lib3ds_vector_length#(c:Float Ptr)
	Function lib3ds_vector_normalize(c:Float Ptr)
	Function lib3ds_vector_normal(n:Float Ptr, a:Float Ptr, b:Float Ptr, c:Float Ptr)
	Function lib3ds_vector_min(c:Float Ptr, a:Float Ptr)
	Function lib3ds_vector_max(c:Float Ptr, a:Float Ptr)
	Function lib3ds_vector_transform(c:Float Ptr, m:Float Ptr, a:Float Ptr)
	
	Function lib3ds_quat_identity(c:Float Ptr)
	Function lib3ds_quat_copy(dest:Float Ptr, src:Float Ptr)
	Function lib3ds_quat_axis_angle(c:Float Ptr, axis:Float Ptr, angle#)
	Function lib3ds_quat_neg(c:Float Ptr)
	Function lib3ds_quat_cnj(c:Float Ptr)
	Function lib3ds_quat_mul(c:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_quat_scalar(c:Float Ptr, k#)
	Function lib3ds_quat_normalize(c:Float Ptr)
	Function lib3ds_quat_inv(c:Float Ptr)
	Function lib3ds_quat_dot#(a:Float Ptr, b:Float Ptr)
	Function lib3ds_quat_norm#(c:Float Ptr)
	Function lib3ds_quat_ln(c:Float Ptr)
	Function lib3ds_quat_ln_dif(c:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_quat_exp(c:Float Ptr)
	Function lib3ds_quat_slerp(c:Float Ptr, a:Float Ptr, b:Float Ptr, t#)
	Function lib3ds_quat_squad(c:Float Ptr, a:Float Ptr, p:Float Ptr, q:Float Ptr, b:Float Ptr, t#)
	Function lib3ds_quat_tangent(c:Float Ptr, p:Float Ptr, q:Float Ptr, n:Float Ptr)

	Function lib3ds_matrix_zero(m:Float Ptr)
	Function lib3ds_matrix_identity(m:Float Ptr)
	Function lib3ds_matrix_copy(dest:Float Ptr, src:Float Ptr)
	Function lib3ds_matrix_neg(m:Float Ptr)
	Function lib3ds_matrix_transpose(m:Float Ptr)
	Function lib3ds_matrix_add(m:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_matrix_sub(m:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_matrix_mult(m:Float Ptr, a:Float Ptr, b:Float Ptr)
	Function lib3ds_matrix_scalar(m:Float Ptr, k#)
	Function lib3ds_matrix_det#(m:Float Ptr)
	Function lib3ds_matrix_inv(m:Float Ptr)
	Function lib3ds_matrix_translate(m:Float Ptr, x#, y#, z#)
	Function lib3ds_matrix_scale(m:Float Ptr, x#, y#, z#)
	Function lib3ds_matrix_rotate_quat(m:Float Ptr, q:Float Ptr)
	Function lib3ds_matrix_rotate(m:Float Ptr, angle#, ax:Float Ptr, ay:Float Ptr, az:Float Ptr)
	Function lib3ds_matrix_camera(m:Float Ptr, pos:Float Ptr, tgt:Float Ptr, roll#)
End Extern