#ifndef VDL_H
#define VDL_H
#include <stdlib.h>

struct VDL_Reference {
  char *name;
  void *pointer;
};

struct VDL_Library {
  char *name;
  void *handle;
  struct VDL_Reference *references;
  size_t reference_count;
};

struct VDL_LoaderDescription {
  char *libraries_path;
  char *library_file_name;
  char *library_compile_flags;
  char **library_compile_command;
};

struct VDL_Library *VDL_LibrariesLoad(struct VDL_LoaderDescription description,
                                      const char **reference_names,
                                      size_t reference_count,
                                      size_t *library_count);

void VDL_LibraryPrint(struct VDL_Library *library);

void VDL_LibraryUnload(struct VDL_Library library);


//#define VDL_IMPLEMENTATION
#ifdef VDL_IMPLEMENTATION

#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

size_t iVDL_LibrariesCount(struct VDL_LoaderDescription description) {
  DIR *libraries_dir = opendir(description.libraries_path);
  size_t library_count = 0;
  struct dirent *entry = NULL;
  while ((entry = readdir(libraries_dir)) != 0) {
    if (entry->d_type == DT_DIR) {
      if (!strcmp(entry->d_name, ".."))
        continue;
      if (!strcmp(entry->d_name, "."))
        continue;
      library_count++;
    }
  }
  closedir(libraries_dir);
  return library_count;
}

char **iVDL_LibrariesNamesGet(struct VDL_LoaderDescription description,
                              size_t *library_count) {
  *library_count = iVDL_LibrariesCount(description);
  char **names = (char**)calloc(sizeof(char *), *library_count);
  DIR *libraries_dir = opendir(description.libraries_path);

  struct dirent *entry = NULL;
  size_t library_index = 0;
  while ((entry = readdir(libraries_dir)) != 0) {
    if (entry->d_type == DT_DIR) {
      if (!strcmp(entry->d_name, ".."))
        continue;
      if (!strcmp(entry->d_name, "."))
        continue;
      names[library_index] = strdup(entry->d_name);
      library_index++;
    }
  }
  closedir(libraries_dir);
  return names;
}

void iVDL_LibrariesCompile(struct VDL_LoaderDescription description,
                           char **library_names, size_t library_count) {
  for (size_t i = 0; i < library_count; i++) {
    char full_path[256] = {0};
    strncat(full_path, description.libraries_path, 255);
    strncat(full_path, "/", 255);
    strncat(full_path, library_names[i], 255);
    pid_t pid = fork();
    if (pid == 0) { // child process
      puts("");
      printf("WAIT:  changing directory to %s\n", full_path);
      if (chdir(full_path)) {
        printf("ERROR: no such directory %s\n", full_path);
      }
      puts("OK:    directory changed");
      puts("");
      execvp(description.library_compile_command[0], description.library_compile_command);

      printf("execvp error");
    } else {
      int status = 0;
      waitpid(pid, &status, 0);
      if (status) {
        printf("compilation of %s failed\n", library_names[i]);
      } else {
        printf("Library %s compiled successfully\n", library_names[i]);
      }
    }
  }
  puts("");
}

struct VDL_Library *VDL_LibrariesLoad(struct VDL_LoaderDescription description,
                                      const char **reference_names,
                                      size_t reference_count,
                                      size_t *library_count) {

  printf("Getting library names\n");
  char **names = iVDL_LibrariesNamesGet(description, library_count);
  iVDL_LibrariesCompile(description, names, *library_count);

  struct VDL_Library *libraries = (struct VDL_Library*)
      calloc(sizeof(struct VDL_Library), *library_count);

  for (size_t i = 0; i < *library_count; i++) {
    char final_path[256] = {0};
    strncat(final_path, description.libraries_path, 255);
    strncat(final_path, "/", 255);
    strncat(final_path, names[i], 255);
    strncat(final_path, "/", 255);
    strncat(final_path, description.library_file_name, 255);

    libraries[i].name = strdup(names[i]);
    libraries[i].reference_count = reference_count;

    libraries[i].references = (struct VDL_Reference *)calloc(
        sizeof(struct VDL_Reference), reference_count);    
    printf("Loading library '%s' at %s\n", libraries[i].name, final_path);
    libraries[i].handle = dlopen(final_path, RTLD_LAZY | RTLD_LOCAL);
    if (!libraries[i].handle) {
      printf("MOD %s IS NOT LOADED\n", libraries[i].name);
      abort();
    }
    dlerror();
    for (size_t j = 0; j < libraries[i].reference_count; j++) {
      const char *pointer_name = reference_names[j];
      struct VDL_Reference *reference = libraries[i].references + j;
      reference->name = strdup(pointer_name);
      reference->pointer = dlsym(libraries[i].handle, pointer_name);
    }
  }

  printf("%zu mods loaded\n", *library_count);
  return libraries;
}

void VDL_LibraryUnload(struct VDL_Library *library) {
  dlclose(library->handle);
  free(library->name);
  library->name = NULL;
  for (size_t i = 0; i < library->reference_count; i++) {
    library->references[i].pointer = NULL;
    free(library->references[i].name);
  }
  free(library->references);
  library->reference_count = 0;
};  

void VDL_LibraryPrint(struct VDL_Library library) {
  printf("Library %s\n", library.name);
  for (size_t i = 0; i < library.reference_count; i++) {
    printf("%p <- %s\n", library.references[i].pointer,
           library.references[i].name);
  }
  puts("");
}

#endif
#endif
