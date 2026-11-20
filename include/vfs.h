#ifndef VFS_H
#define VFS_H
#include <stdlib.h>
#include "vstruct.h"


typedef struct iVFS_Node *VFS_Node;
typedef struct iVFS_NodeFile *VFS_NodeFile;


void iVFS_NodesDestroy(VFS_Node first, ...);
#define VFS_NodesDestroy(first, ...)                                           \
  iVFS_NodesDestroy(first, ##__VA_ARGS__, (VFS_Node) - 1);

VFS_Node VFS_NodeFromPath(struct VST_StringView path);


#endif
#define VFS_IMPLEMENTATION
#ifdef VFS_IMPLEMENTATION
#include "vmem.h"
#include <stdio.h>

enum iVFS_NodeKind {
  IVFS_NODE_INVALID,
  IVFS_NODE_FILE,
  IVFS_NODE_FOLDER
};  

struct iVFS_NodeHeader {
  enum iVFS_NodeKind kind;
  VST_String path;
};

struct iVFS_Node {
  struct iVFS_NodeHeader header;
};

struct iVFS_NodeFile {
  struct iVFS_NodeHeader header;
};



VFS_Node iVFS_NodeFileFromPath(struct VST_StringView path) {
  VST_String path_string = VST_StringFromStringView(path);
  VFS_NodeFile file =
      (VFS_NodeFile)VMEM_Allocate(sizeof(struct iVFS_NodeFile), NULL);
  file->header.path = path_string;
  file->header.kind = IVFS_NODE_FILE;
  return (VFS_Node)file;
}

VFS_Node VFS_NodeFromPath(struct VST_StringView path) {
  return iVFS_NodeFileFromPath(path);
}



VST_String iVFS_NodeFileStringGet(VFS_NodeFile file) {
  VST_String file_path = file->header.path;
  VST_String output = VST_StringCreate();
  FILE *cfile = fopen(VST_StringPtrGet(file_path), "r");

  char buffer[256];

  while (fgets(buffer, sizeof(buffer) - 1, cfile) != NULL) {
    VST_StringAppendStr(output, buffer);
  }
  
  fclose(cfile);
  return output;
}


VST_String VFS_NodeStringGet(VFS_Node file) {
  struct iVFS_NodeHeader * header = (struct iVFS_NodeHeader*) file;

  switch (header->kind) {
  case IVFS_NODE_FILE:
    return iVFS_NodeFileStringGet((VFS_NodeFile)file);
  default:
    VLOG_Error("Getting string from this type not supported %d", header->kind);
  }    
}

void iVFS_NodeDestroy(VFS_Node node) {
  struct iVFS_NodeHeader *header = (struct iVFS_NodeHeader *)node;
  switch (header->kind) {
  case IVFS_NODE_FOLDER:
    break;
  case IVFS_NODE_FILE:
    break;
  default:
    VLOG_Error("Not a valid node kind");
  }

  VST_StringsDestroy(header->path);
  free(node);
}  

void iVFS_NodesDestroy(VFS_Node first, ...) {
  va_list files;
  va_start(files, first);
  VFS_Node node = first;
  while (node != (VFS_Node)-1) {
    VLOG_Info("Destroying node");
    iVFS_NodeDestroy(node);
    node = va_arg(files, VFS_Node);
  }
  va_end(files);
}  

#endif
