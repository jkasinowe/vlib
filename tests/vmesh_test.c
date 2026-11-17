#define VMESH_IMPLEMENTATION
#include "../include/vmesh.h"
#include <stdio.h>

int main() {
  struct VMESH_Mesh mesh;
  char* path = "./models/box.obj";
  char *source = NULL;
  size_t source_size = 0;
  
  VMESH_FileLoad(path, &source, &source_size);
  VMESH_MeshLoadObj(&mesh, source, source_size);
  struct VMESH_Vertex *vertices = mesh.vertices;
  uint32_t *index = mesh.indices;
  size_t show_count = mesh.index_count > 100 ? 100 : mesh.index_count;
  for (size_t i = 0; i < show_count; i++) {
    size_t j = index[i];
    printf("%zu->%zu:\t%.3f %.3f %.3f\t: %.3f %.3f\t: %.3f %.3f %.3f\n", i, j,
           vertices[j].pos[0], vertices[j].pos[1], vertices[j].pos[2],
           vertices[j].tex[0], vertices[j].tex[1],           
           vertices[j].normal[0], vertices[j].normal[1], vertices[j].normal[2]);    
  }
  printf("OK\n");
  VMESH_MeshDefaultDestroy(&mesh);
  free(source);
  return 0;
}
