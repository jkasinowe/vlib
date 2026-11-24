#define VLIB_IMPLEMENTATION
#include "../include/vfs.h"
#include <stdio.h>

int main() {
  printf("Testing file loading.. ");
  fflush(stdout);
  struct VST_StringView path = VST_StringViewFromStr("models/box.obj");
  VFS_Node file = VFS_NodeFromPath(path);
  VST_String string = VFS_NodeStringGet(file);

  VST_StringPrint(string);
  
  VST_StringsDestroy(string);
  VFS_NodesDestroy(file);
  printf("OK\n");

  file = VFS_NodeFromPath(
      VST_StringViewFromStr("/home/jkasinowe/Downloads/shakespear.txt"));


  VST_String shakespear_original = VFS_NodeStringGet(file);

  struct VST_StringView from_a = VST_StringViewFromStr(" ");
  struct VST_StringView to_b = VST_StringViewFromStr("-");
  VST_String shakespear_temp =
      VST_StringReplace(shakespear_original, from_a, to_b);
  from_a = VST_StringViewFromStr("\n");
  to_b = VST_StringViewFromStr("\\n");
  VST_String shakespear_processed = VST_StringReplace(shakespear_temp, from_a, to_b);

  VST_StringPrint(shakespear_processed);
  VFS_NodesDestroy(file);
  VST_StringsDestroy(shakespear_original, shakespear_temp, shakespear_processed);

  return 0;
}  
