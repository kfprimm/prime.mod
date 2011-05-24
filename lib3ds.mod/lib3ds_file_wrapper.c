
#include <lib3ds.h>

unsigned         lib3ds_file_get_user_id            (Lib3dsFile *file) { return file->user_id; }
void*            lib3ds_file_get_user_ptr           (Lib3dsFile *file) { return file->user_ptr; }
unsigned         lib3ds_file_get_mesh_version       (Lib3dsFile *file) { return file->mesh_version; }
unsigned         lib3ds_file_get_keyf_revision      (Lib3dsFile *file) { return file->keyf_revision; }
char*            lib3ds_file_get_name               (Lib3dsFile *file) { return file->name; }
float            lib3ds_file_get_master_scale       (Lib3dsFile *file) { return file->master_scale; }
float*           lib3ds_file_get_construction_plane (Lib3dsFile *file) { return file->construction_plane; }
float*           lib3ds_file_get_ambient            (Lib3dsFile *file) { return file->ambient; }
Lib3dsShadow     lib3ds_file_get_shadow             (Lib3dsFile *file) { return file->shadow; }
Lib3dsBackground lib3ds_file_get_background         (Lib3dsFile *file) { return file->background; }
Lib3dsAtmosphere lib3ds_file_get_atmosphere         (Lib3dsFile *file) { return file->atmosphere; }
Lib3dsViewport   lib3ds_file_get_viewport           (Lib3dsFile *file) { return file->viewport; }
Lib3dsViewport   lib3ds_file_get_viewport_keyf      (Lib3dsFile *file) { return file->viewport_keyf; }
int              lib3ds_file_get_frames             (Lib3dsFile *file) { return file->frames; }
int              lib3ds_file_get_segment_from       (Lib3dsFile *file) { return file->segment_from; }
int              lib3ds_file_get_segment_to         (Lib3dsFile *file) { return file->segment_to; }
int              lib3ds_file_get_current_frame      (Lib3dsFile *file) { return file->current_frame; }
int              lib3ds_file_get_materials_size     (Lib3dsFile *file) { return file->materials_size; }
int              lib3ds_file_get_nmaterials         (Lib3dsFile *file) { return file->nmaterials; }
Lib3dsMaterial** lib3ds_file_get_materials          (Lib3dsFile *file) { return file->materials; }       
int              lib3ds_file_get_cameras_size       (Lib3dsFile *file) { return file->cameras_size; }
int              lib3ds_file_get_ncameras           (Lib3dsFile *file) { return file->ncameras; }
Lib3dsCamera**   lib3ds_file_get_camera             (Lib3dsFile *file) { return file->cameras; }
int              lib3ds_file_get_lights_size        (Lib3dsFile *file) { return file->lights_size; }
int              lib3ds_file_get_nlights            (Lib3dsFile *file) { return file->nlights; }
Lib3dsLight**    lib3ds_file_get_lights             (Lib3dsFile *file) { return file->lights; }
int              lib3ds_file_get_meshes_size        (Lib3dsFile *file) { return file->meshes_size; }
int              lib3ds_file_get_nmeshes            (Lib3dsFile *file) { return file->nmeshes; }
Lib3dsMesh**     lib3ds_file_get_meshes             (Lib3dsFile *file) { return file->meshes; }
Lib3dsNode*      lib3ds_file_get_nodes              (Lib3dsFile *file) { return file->nodes; }
