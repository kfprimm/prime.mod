
#include <lib3ds.h>

unsigned        lib3ds_file_get_user_id(Lib3dsFile *file) { return file->user_id; }
void*           lib3ds_file_get_user_ptr(Lib3dsFile *file) { return file->user_ptr; }
char*           lib3ds_file_get_name(Lib3dsFile *file) { return file->name; }
unsigned        lib3ds_file_get_object_flags(Lib3dsFile *file) { return file->object_flags; }
int             lib3ds_file_get_color(Lib3dsFile *file) { return file->color; }
float*          lib3ds_file_get_matrix(Lib3dsFile *file) { return file->matrix; }
unsigned short  lib3ds_file_get_nvertices(Lib3dsFile *file) { return file->nvertices; }
float*          lib3ds_file_get_vertices(Lib3dsFile *file) { return file->vertices; }
float*          lib3ds_file_get_texcos(Lib3dsFile *file) { return file->texcos; }
unsigned short* lib3ds_file_get_vflags(Lib3dsFile *file) { return file->vflags; }
unsigned short  lib3ds_file_get_nfaces(Lib3dsFile *file) { return file->nfaces; }
Lib3dsFace*     lib3ds_file_get_faces(Lib3dsFile *file) { return file->faces; }           
char*           lib3ds_file_get_box_front(Lib3dsFile *file) { return file->box_front; }
char*           lib3ds_file_get_box_back(Lib3dsFile *file) { return file->box_back; }
char*           lib3ds_file_get_box_left(Lib3dsFile *file) { return file->box_left; }
char*           lib3ds_file_get_box_right(Lib3dsFile *file) { return file->box_right; }
char*           lib3ds_file_get_box_top(Lib3dsFile *file) { return file->box_top; }
char*           lib3ds_file_get_box_bottom(Lib3dsFile *file) { return file->box_bottom; }
int             lib3ds_file_get_map_type(Lib3dsFile *file) { return file->map_type; }
float*          lib3ds_file_get_map_pos(Lib3dsFile *file) { return file->map_pos; }
float*          lib3ds_file_get_map_matrix(Lib3dsFile *file) { return file->map_matrix; }
float           lib3ds_file_get_map_scale(Lib3dsFile *file) { return file->map_scale; }
float*          lib3ds_file_get_map_tile(Lib3dsFile *file) { return file->map_tile; }
float*          lib3ds_file_get_map_planar_size(Lib3dsFile *file) { return file->map_planar_size; }
float           lib3ds_file_get_map_cylinder_height(Lib3dsFile *file) { return file->map_cylinder_height; }
