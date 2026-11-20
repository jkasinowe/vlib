#ifndef VMESH_H
#define VMESH_H
#include "vmem.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include "vlog.h"
#include "vstruct.h"
#include "vfs.h"

// define VMESH_DEBUG to show debug info

struct VMESH_Vertex {
  float pos[3];
  float normal[3];
  float tex[2];
};

struct VMESH_Mesh {
  struct VMESH_Vertex *vertices;
  uint32_t *indices;
  uint32_t index_count;
  uint32_t vertex_count;
};

struct VMESH_Mesh VMESH_MeshFromObjNode(VFS_Node node,
                                        VMEM_Allocator allocator);

struct VMESH_Mesh VMESH_MeshFromObjSource(struct VST_StringView source,
                                          VMEM_Allocator allocator);
                             

void VMESH_MeshDefaultDestroy(struct VMESH_Mesh *mesh);

#define VMESH_IMPLEMENTATION
#ifdef VMESH_IMPLEMENTATION
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDEX_SIZE (4)
#define KEY_SIZE = (INDEX_SIZE * INDEX_SIZE * INDEX_SIZE)
#define LOOKUP_TABLE_SIZE (KEY_SIZE * INDEX_SIZE);

typedef struct {
  uint32_t pos;
  uint32_t normal;
  uint32_t tex;
} Index;

void iVMESH_ParseSpace(char *source, uint32_t source_size, size_t *index) {
  while (*index < source_size && isspace(source[*index]) &&
         source[*index] != '\n') {
    (*index)++;
  }
}

void iVMESH_ParseSpaceAndNewline(char *source, uint32_t source_size,
                                 size_t *index) {
  while (*index < source_size && isspace(source[*index])) {
    (*index)++;
  }
}

uint32_t iVMESH_ParseInt(char *source, size_t source_size, size_t *index) {
  iVMESH_ParseSpace(source, source_size, index);
  char *end_ptr = NULL;
  uint32_t i = strtol(source + *index, &end_ptr, 10);
  *index = end_ptr - source;
  iVMESH_ParseSpace(source, source_size, index);
  return i;
}

float iVMESH_ParseFloat(char *source, size_t source_size, size_t *index) {
  iVMESH_ParseSpace(source, source_size, index);
  char *end_ptr = NULL;
  float f = strtof(source + *index, &end_ptr);
  *index = end_ptr - source;
  iVMESH_ParseSpace(source, source_size, index);
  return f;
}

enum iVMESH_InputType {
  IVMESH_INPUT_TYPE_EOF,
  IVMESH_INPUT_TYPE_VERTEX,
  IVMESH_INPUT_TYPE_NORMAL,
  IVMESH_INPUT_TYPE_TEXTURE,
  IVMESH_INPUT_TYPE_FACE,
  IVMESH_INPUT_TYPE_COMMENT,
};

enum iVMESH_InputType iVMESH_ParseInputType(struct VST_StringView line) {
  if (VST_StringViewStartsWithStr(line, "vn"))
    return IVMESH_INPUT_TYPE_NORMAL;

  if (VST_StringViewStartsWithStr(line, "vt"))
    return IVMESH_INPUT_TYPE_TEXTURE;
  
  if (VST_StringViewStartsWithStr(line, "v"))
    return IVMESH_INPUT_TYPE_VERTEX;

  if (VST_StringViewStartsWithStr(line, "f"))
    return IVMESH_INPUT_TYPE_FACE;

  if (VST_StringViewStartsWithStr(line, "o") ||
      VST_StringViewStartsWithStr(line, "model") ||
      VST_StringViewStartsWithStr(line, "#") ||
      VST_StringViewStartsWithStr(line, "s"))
    return IVMESH_INPUT_TYPE_COMMENT;
  
  VLOG_Error("Couldn't parse input type at %zu", *index);
  abort();
}

void iVMESH_CountVNTF(struct VST_StringView source, uint32_t *vert_count,
                      uint32_t *face_count, uint32_t *uv_count,
                      uint32_t *normal_count) {
  VLOG_Wait("Calculating vertex and index count");
  struct VST_StringView source_view = source;

  uint32_t lface_count = 0;
  uint32_t lvert_count = 0;
  uint32_t luv_count = 0;
  uint32_t lnormal_count = 0;

  struct VST_StringView end_of_line = VST_StringViewFromStr("\n");
  struct VST_StringView space = VST_StringViewFromStr(" ");
  struct VST_StringView tab = VST_StringViewFromStr("\t");  
  
  usize line_count = VST_StringViewSplitByCount(source_view, end_of_line);
  for (usize i = 0; i < line_count; i++) {
    struct VST_StringView line =
        VST_StringViewShiftBy(&source_view, end_of_line);
    
    switch (iVMESH_ParseInputType(line)) {
    case IVMESH_INPUT_TYPE_EOF:
      break;
    case IVMESH_INPUT_TYPE_FACE:
      lface_count += 1;
      break;
    case IVMESH_INPUT_TYPE_NORMAL:
      lnormal_count += 1;
      break;
    case IVMESH_INPUT_TYPE_TEXTURE:
      luv_count += 1;
      break;
    case IVMESH_INPUT_TYPE_VERTEX:
      lvert_count += 1;
      break;
    default:
      break;
    }
  }
  *face_count = lface_count;
  *normal_count = lnormal_count;
  *uv_count = luv_count;
  *vert_count = lvert_count;
  VLOG_Ok("Vertex and index count calculated");
}

Index *iVMESH_ParseObjIndices(struct VST_StringView source, float **positions,
                              float **normals, float **uvs) {
  
  Index *indices = NULL;
  
  VLOG_Wait("Gettting vertices");
  fflush(stdout);

  *positions = NULL;
  *normals = NULL;
  *uvs = NULL;
  
  struct VST_StringView end_of_line = VST_StringViewFromStr("\n");
  struct VST_StringView space = VST_StringViewFromStr(" ");
  struct VST_StringView tab = VST_StringViewFromStr("\t");  
  
  struct VST_StringView *lines = VST_StringViewSplitByList(source, end_of_line);

  for (usize i = 0; i < VST_ListLength(lines); i++) {
    struct VST_StringView line = lines[i];
    struct VST_StringView type_view = VST_StringViewShiftBy(&line, space);
    
    enum iVMESH_InputType type = iVMESH_ParseInputType(type_view);
    
    switch (type) {
    case IVMESH_INPUT_TYPE_COMMENT:
      break;
    case IVMESH_INPUT_TYPE_VERTEX: {
      struct VST_StringView *floats = VST_StringViewSplitByList(line, space);

      for (usize i = 0; i < VST_ListLength(floats); i++) {
	VST_ListPush(
      }        
      
      break;
    }
    case IVMESH_INPUT_TYPE_NORMAL: {
      for (size_t float_count = 0; float_count < 3; float_count++) {
        iVMESH_ParseSpace(source, source_size, &char_index);
        (*normals)[cur_normal * 3 + float_count] =
            iVMESH_ParseFloat(source, source_size, &char_index);
      }
      cur_normal++;
      iVMESH_ParseSpaceAndNewline(source, source_size, &char_index);
      break;
    }
    case IVMESH_INPUT_TYPE_TEXTURE: {
      for (size_t float_count = 0; float_count < 2; float_count++) {
        iVMESH_ParseSpace(source, source_size, &char_index);
        (*uvs)[cur_uv * 2 + float_count] =
            iVMESH_ParseFloat(source, source_size, &char_index);
      }
      cur_uv++;
      iVMESH_ParseSpaceAndNewline(source, source_size, &char_index);
      break;
    }
    case IVMESH_INPUT_TYPE_FACE: {
      for (size_t index_count = 0; index_count < 3; index_count++) {
        indices[cur_index].pos =
            iVMESH_ParseInt(source, source_size, &char_index);

        iVMESH_ParseSpace(source, source_size, &char_index);

        if (source[char_index] == '/') {
          if (source[char_index + 1] != '/') {
            char_index++;
            indices[cur_index].tex =
                iVMESH_ParseInt(source, source_size, &char_index);
          } else {
            char_index++;
          }

          if (source[char_index] != '/') {
            VLOG_Error("missing second '/' in f/f/f at %zu\n", char_index);
            abort();
          }

          char_index++;
          indices[cur_index].normal =
              iVMESH_ParseInt(source, source_size, &char_index);
        }

        cur_index++;

        if (source[char_index] == '\n') {
          char_index++;
          break;
        }
      }

      iVMESH_ParseSpaceAndNewline(source, source_size, &char_index);
      break;
    }
    }
  }

  free(lines);
  return indices;
}

uint32_t iVMESH_TableFind(Index *table, Index *index, uint32_t face_count) {
  for (size_t i = 0; i < face_count * 3; i++) {
    if (table[i].normal == 0xFCFCFCFC && table[i].pos == 0xFCFCFCFC &&
        table[i].tex == 0xFCFCFCFC) {

      table[i].normal = index->normal;
      table[i].pos = index->pos;
      table[i].tex = index->tex;

      return i;
    }

    if (table[i].normal == index->normal && table[i].pos == index->pos &&
        table[i].tex == index->tex) {
      return i;
    }
  }
  VLOG_Error("Found invalid table entry");
  abort();
  return 0;
}


struct VMESH_Mesh VMESH_MeshFromObjNode(VFS_Node node,
                                        VMEM_Allocator allocator) {
  VST_String source = VFS_NodeStringGet(node);
  struct VMESH_Mesh mesh = VMESH_MeshFromObjSource(VST_StringViewFromString(source), allocator);
  VST_StringsDestroy(source);
  return mesh;
}

struct VMESH_Mesh VMESH_MeshFromObjSource(struct VST_StringView source,
                                          VMEM_Allocator allocator) {
  struct VMESH_Mesh mesh = {0};
  uint32_t vert_count, face_count, uv_count, normal_count;
  iVMESH_CountVNTF(source, &vert_count, &face_count, &uv_count,
                   &normal_count);

  float *positions, *normals, *uvs;

  Index *indices = iVMESH_ParseObjIndices(
					  source, face_count, &positions, vert_count, &normals,
      normal_count, &uvs, uv_count); // get indices
  Index *lookup_table = malloc(face_count * 3 * sizeof(Index));
  memset(lookup_table, 0xFC, face_count * 3 * sizeof(Index));
  uint32_t *mapped_indices = malloc(face_count * 3 * sizeof(uint32_t));
  memset(mapped_indices, 0xFC, face_count * 3 * sizeof(uint32_t));

  for (size_t i = 0; i < face_count * 3; i++) {
    mapped_indices[i] = iVMESH_TableFind(lookup_table, indices + i, face_count);
    // real indices map from indices triplets to just vertices
  }

  uint32_t out_vertex_count;
  for (out_vertex_count = 0; out_vertex_count < face_count * 3;
       out_vertex_count++) {
    if (lookup_table[out_vertex_count].pos == 0xFCFCFCFC) {
      break;
    }
  }

  uint32_t *mesh_indices = malloc(face_count * 3 * sizeof(uint32_t));
  for (size_t i = 0; i < face_count * 3; i++) {
    mesh_indices[i] = mapped_indices[i];
  }

  struct VMESH_Vertex *vertices =
      calloc(face_count * 3, sizeof(struct VMESH_Vertex));
  VLOG_Wait("Recalculating vertices");
  fflush(stdout);
  for (size_t i = 0; i < face_count * 3; i++) {
    uint32_t j = mesh_indices[i];
    uint32_t p = lookup_table[j].pos - 1;
    uint32_t n = lookup_table[j].normal - 1;
    uint32_t t = lookup_table[j].tex - 1;

    vertices[j].pos[0] = positions[3 * p + 0];
    vertices[j].pos[1] = positions[3 * p + 1];
    vertices[j].pos[2] = positions[3 * p + 2];

    vertices[j].normal[0] = normals[3 * n + 0];
    vertices[j].normal[1] = normals[3 * n + 1];
    vertices[j].normal[2] = normals[3 * n + 2];
    if (uv_count) {
      vertices[j].tex[0] = uvs[2 * t + 0];
      vertices[j].tex[1] = uvs[2 * t + 1];
    }
  }
  VLOG_Ok("Vertices recalculated");

  mesh->vertices = vertices;
  mesh->indices = mesh_indices;
  mesh->vertex_count = out_vertex_count;
  mesh->index_count = face_count * 3;

  VLOG_Wait("Tidying up");
  fflush(stdout);
  free(mapped_indices);
  free(lookup_table);
  free(indices);
  free(positions);
  if (normal_count) {
    free(normals);
  }
  if (uv_count) {
    free(uvs);
  }
  VLOG_Ok("Tidied up");
}

void iVMESH_CountVerticesAndFaces(char *path, uint32_t *vertices,
                                  uint32_t *faces, uint32_t *normals) {
  FILE *file = fopen(path, "r");
  char c;
  *faces = 0;
  *vertices = 0;
  *normals = 0;

  while ((c = fgetc(file)) != EOF) {
    if (c == '#') {
    }
    if (c == 'f')      
      *faces += 1;
    if (c == 'v') {
      c = fgetc(file);
      if (c == 'n')
        *normals += 1;
      else
        *vertices += 1;
    }
    while (c != '\n' && c != EOF) {
      c = fgetc(file);
    }
    if (c == EOF)
      break;
  }
  fclose(file);
}

void VMESH_MeshDefaultDestroy(struct VMESH_Mesh *mesh) {
  free(mesh->indices);
  free(mesh->vertices);
}

#endif
#ifdef __cplusplus
}
#endif
#endif
