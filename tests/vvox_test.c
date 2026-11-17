#define VMESH_IMPLEMENTATION
#define VVOX_IMPLEMENTATION
#define VSHAPE_IMPLEMENTATION
#include "../include/vvox.h"
#include "../include/vmesh.h"
#define VMATH_IMPLEMENTATION
#include "../include/vmath.h"
#include <float.h>
#include <stddef.h>


typedef struct VMESH_Mesh Mesh;
typedef struct VMESH_Vertex Vertex;

int main() {
  Mesh *mesh = malloc(sizeof(Mesh));
  char *source = NULL;
  size_t source_size = 0;
  VMESH_FileLoad("models/box.obj", &source, &source_size);

  VMESH_MeshLoadObj(mesh, source, source_size);  
    f32 voxel_size = 0.01;

    u32 voxel_count[3];
    b8* voxels = VVOX_VerticesToVoxels(mesh->vertices, mesh->vertex_count, sizeof(Vertex), offsetof(Vertex, pos),
			   mesh->indices,  mesh->index_count,
					voxel_size, voxel_count);

    const u32 z = 1;//x voxel_count[2]/12;
    printf("max_depth: %d\n", voxel_count[2]);
    for (i32 j = voxel_count[1] - 1; j >= 0; j--) {
	for (u32 i = 0; i < voxel_count[0]; i++){
	    printf("%c", voxels[z*voxel_count[0]*voxel_count[1] + j*voxel_count[0]+i] ? 'c' : ' ');
	}
	printf("\n");
    }

    
    free(voxels);
    VMESH_MeshDefaultDestroy(mesh);
    free(source);
    free(mesh);
    return 0;
}
