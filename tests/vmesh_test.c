#define VMESH_IMPLEMENTATION
#include <stdio.h>
#include "../include/vmesh.h"
#include <assert.h>

int main() {
  printf("Testing obj loader..");
  fflush(stdout);
  struct VMESH_Mesh mesh;
  char* path = "./models/box.obj";
  char *source = NULL;
  size_t source_size = 0;
  
  VMESH_FileLoad(path, &source, &source_size);
  VMESH_MeshLoadObj(&mesh, source, source_size);
  struct VMESH_Vertex *vertices = mesh.vertices;
  uint32_t *indices = mesh.indices;

  
  assert(vertices[indices[10]].pos[0] + 1 < 0.00001);
  printf("\tOK\n");
  VMESH_MeshDefaultDestroy(&mesh);
  free(source);
  return 0;
}
