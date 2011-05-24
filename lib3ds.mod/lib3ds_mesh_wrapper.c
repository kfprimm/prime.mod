
#include <lib3ds.h>

unsigned        lib3ds_mesh_get_user_id(Lib3dsMesh *mesh) { return mesh->user_id; }
void*           lib3ds_mesh_get_user_ptr(Lib3dsMesh *mesh) { return mesh->user_ptr; }
char*           lib3ds_mesh_get_name(Lib3dsMesh *mesh) { return mesh->name; }
unsigned        lib3ds_mesh_get_object_flags(Lib3dsMesh *mesh) { return mesh->object_flags; }
int             lib3ds_mesh_get_color(Lib3dsMesh *mesh) { return mesh->color; }
float*          lib3ds_mesh_get_matrix(Lib3dsMesh *mesh) { return mesh->matrix; }
unsigned short  lib3ds_mesh_get_nvertices(Lib3dsMesh *mesh) { return mesh->nvertices; }
float*          lib3ds_mesh_get_vertices(Lib3dsMesh *mesh) { return mesh->vertices; }
float*          lib3ds_mesh_get_texcos(Lib3dsMesh *mesh) { return mesh->texcos; }
unsigned short* lib3ds_mesh_get_vflags(Lib3dsMesh *mesh) { return mesh->vflags; }
unsigned short  lib3ds_mesh_get_nfaces(Lib3dsMesh *mesh) { return mesh->nfaces; }
Lib3dsFace*     lib3ds_mesh_get_faces(Lib3dsMesh *mesh) { return mesh->faces; }           
char*           lib3ds_mesh_get_box_front(Lib3dsMesh *mesh) { return mesh->box_front; }
char*           lib3ds_mesh_get_box_back(Lib3dsMesh *mesh) { return mesh->box_back; }
char*           lib3ds_mesh_get_box_left(Lib3dsMesh *mesh) { return mesh->box_left; }
char*           lib3ds_mesh_get_box_right(Lib3dsMesh *mesh) { return mesh->box_right; }
char*           lib3ds_mesh_get_box_top(Lib3dsMesh *mesh) { return mesh->box_top; }
char*           lib3ds_mesh_get_box_bottom(Lib3dsMesh *mesh) { return mesh->box_bottom; }
int             lib3ds_mesh_get_map_type(Lib3dsMesh *mesh) { return mesh->map_type; }
float*          lib3ds_mesh_get_map_pos(Lib3dsMesh *mesh) { return mesh->map_pos; }
float*          lib3ds_mesh_get_map_matrix(Lib3dsMesh *mesh) { return mesh->map_matrix; }
float           lib3ds_mesh_get_map_scale(Lib3dsMesh *mesh) { return mesh->map_scale; }
float*          lib3ds_mesh_get_map_tile(Lib3dsMesh *mesh) { return mesh->map_tile; }
float*          lib3ds_mesh_get_map_planar_size(Lib3dsMesh *mesh) { return mesh->map_planar_size; }
float           lib3ds_mesh_get_map_cylinder_height(Lib3dsMesh *mesh) { return mesh->map_cylinder_height; }
