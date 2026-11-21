#define VMEM_IMPLEMENTATION
#define VVOX_IMPLEMENTATION
#define VMATH_IMPLEMENTATION
#define VSHAPE_IMPLEMENTATION
#include <stdio.h>
#include <stddef.h>
#include "../include/vmesh.h"
#include "../include/vstruct.h"
#include "../include/vmath.h"
#include "../include/vvox.h"



typedef struct VMESH_Mesh Mesh;
typedef struct VMESH_Vertex Vertex;

int main() {
  printf("Testing voxelization..");
  fflush(stdout);
  
  struct VST_StringView path =  VST_StringViewFromStr("models/box.obj");
  VFS_Node file = VFS_NodeFromPath(path);
  struct VMESH_Mesh mesh = VMESH_MeshFromObjNode(file, NULL);
  f32 voxel_size = 0.01;

  u32 voxel_count[3];
  b8 *voxels = VVOX_VerticesToVoxels(
      mesh.vertices, mesh.vertex_count, sizeof(Vertex), offsetof(Vertex, pos),
      mesh.indices, mesh.index_count, voxel_size, voxel_count);

  /*  
  const u32 z = 1; // x voxel_count[2]/12;
  printf("max_depth: %d\n", voxel_count[2]);
  for (i32 j = voxel_count[1] - 1; j >= 0; j--) {
    for (u32 i = 0; i < voxel_count[0]; i++) {
      printf(
          "%c",
          voxels[z * voxel_count[0] * voxel_count[1] + j * voxel_count[0] + i]
              ? 'c'
              : ' ');
    }
    printf("\n");
  }
  */
  free(voxels);
  VMESH_MeshDefaultDestroy(mesh);
  VFS_NodesDestroy(file);
  printf("\tOK\n");
  return 0;
}
