//#define VLOG_LOG
#define VLIB_IMPLEMENTATION
#include <stdio.h>
#include "../include/vmesh.h"
#include <assert.h>

void print_vec(float *vec, usize size) {
  for (uint32_t i = 0 ;i < size; i++) {
    printf("%.3f\t",  vec[i]);
  }    
}  

int main() {
  printf("Testing obj loader..");
  fflush(stdout);

  struct VST_StringView path = VST_StringViewFromStr("./models/box.obj");
  VFS_Node file = VFS_NodeFromPath(path);

  struct VMESH_Mesh mesh = VMESH_MeshFromObjNode(file, NULL);
  struct VMESH_Vertex *vertices = mesh.vertices;
  uint32_t *indices = mesh.indices;


  float expected_pos[] = {1.000, 1.000, 1.000};
  float expected_tex[] = {0.625,  0.750};
  float expected_normal[] = {-0.000, -0.000, 1.000};

  usize vertex_id = indices[9];
  struct VMESH_Vertex *vertex = &vertices[vertex_id];

  assert(VM3_Eq(expected_pos, vertex->pos));
  assert(VM2_Eq(expected_tex, vertex->tex));
  assert(VM3_Eq(expected_normal, vertex->normal));  

  assert(vertices[indices[10]].pos[0] + 1 < 0.00001);

  printf("\tOK\n");

  VFS_NodesDestroy(file);

  VMESH_MeshDefaultDestroy(mesh);
  return 0;
}
