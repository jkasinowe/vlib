#ifndef VFS_H
#define VFS_H
#include <stdlib.h>


struct VFS_Node;

size_t VFS_FileSizeGet();

#endif
#ifdef VFS_IMPLEMENTATION

enum VFS_NodeType {
  VFS_NODE_FILE,
  VFS_NODE_FOLDER
};  

struct VFS_Node {
  
};  


#endif
