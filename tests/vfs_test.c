#define VMEM_IMPLEMENTATION
#include "../include/vfs.h"

int main() {
  struct VST_StringView path = VST_StringViewFromStr("models/box.obj");
  VFS_Node file = VFS_NodeFromPath(path);
  VST_String string = VFS_NodeStringGet(file);

  VST_StringPrint(string);
  
  VST_StringsDestroy(string);
  VFS_NodesDestroy(file);
  return 0;
}  
