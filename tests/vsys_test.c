#define VLOG_LOG
#include <volk.h>
#define VSYS_IMPLEMENTATION
#include "../include/vsys.h"
#define VMATH_IMPLEMENTATION
#include "../include/vmath.h"
#define VMEM_IMPLEMENTATION
#include "../include/vmem.h"
#include <vulkan/vulkan.h>

f32 window_size[2];

#define ARRLEN(a) (sizeof(a)/sizeof(*a))

VKAPI_ATTR VkBool32 VKAPI_CALL
LOG_DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                  void *pUserData) {
  VLOG_Info("%s", pCallbackData->pMessage);
  return VK_FALSE;
}  


void window_resize_callback(struct VSYS_Window*window, f32 size[2]) {
  printf("width:  %f\n", size[0]);
  printf("height: %f\n", size[1]);
}  

int main() {
  if (volkInitialize()) {
    VLOG_Error("volk unable to initialize");
    abort();
  }
  
  VSYS_Initialize();  
  VM2_Set(window_size, 800, 600);

  struct VSYS_Window *window = VSYS_WindowCreate(window_size, VSYS_WINDOW_CREATION_FLAG_VULKAN);  
  uint32_t vk_version;
  vkEnumerateInstanceVersion(&vk_version);

  VLOG_Wait("Creating and loading a vulkan instance");
  VkApplicationInfo app_info = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO};
  app_info.pNext = NULL;
  app_info.pApplicationName = "Vulkan + SDL3";
  app_info.apiVersion = vk_version;
  app_info.pEngineName = "Vector";

  char *instance_extension_custom_names[] = {
      VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
  };

  uint32_t instance_extension_default_count;

  const char *const *instance_extension_default_names =
      VSYS_VulkanInstanceDefaultExtensionsGet(
          &instance_extension_default_count);

  VLOG_Info("Instance default extension count %d",
            instance_extension_default_count);

  uint32_t instance_extension_custom_count = ARRLEN(instance_extension_custom_names);
  

  uint32_t instanceExtensionCount =
      instance_extension_default_count + instance_extension_custom_count;

  const char** instanceExtensionNames =
      malloc(instanceExtensionCount * sizeof(char *));
  memcpy(&instanceExtensionNames[0],
         instance_extension_default_names,
         instance_extension_default_count * sizeof(char *));

  memcpy(
      &instanceExtensionNames[instance_extension_default_count],
      instance_extension_custom_names,
      instance_extension_custom_count * sizeof(char *));

  const char *InstanceRequestedLayers[] = {
      "VK_LAYER_KHRONOS_validation",
  };

  VkDebugUtilsMessengerCreateInfoEXT debug_info = {
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
  debug_info.pNext = NULL;
  debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
  debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debug_info.pfnUserCallback = LOG_DebugCallback;

  VkInstanceCreateInfo instance_info = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};

  instance_info.pNext = &debug_info;
  instance_info.pApplicationInfo = &app_info;
  instance_info.enabledExtensionCount = instanceExtensionCount;
  instance_info.ppEnabledExtensionNames =
      instanceExtensionNames;
  instance_info.enabledLayerCount = ARRLEN(InstanceRequestedLayers);
  instance_info.ppEnabledLayerNames = InstanceRequestedLayers;


  VkInstance vk_instance;
  vkCreateInstance(&instance_info, NULL, &vk_instance);
  volkLoadInstance(vk_instance);
  VLOG_Ok("Vulkan instance loaded");

  VLOG_Wait("Creating vulkan surface");
  VkSurfaceKHR vk_surface =
      VSYS_VulkanInstanceSurfaceCreate(window, vk_instance, NULL, NULL);  
  VLOG_Ok("Vulkan surface created");

  
  VSYS_WindowFlagsEnable(window, VSYS_WINDOW_FLAG_RESIZABLE);
  VSYS_WindowTitleSet(window, "New name");
  VSYS_WindowResizeCallbackSet(window, window_resize_callback);
  VSYS_WindowFrameTimeTargetSet(window, 1./60);


  
  usize frame = 0;
  while (!VSYS_WindowShouldCloseGet(window)) {
    while (!VSYS_WindowFrameTimeWaitedGet(window))
      ;    
    
    VSYS_InputUpdate(window);
    if (VSYS_WindowKeyDownGet(window, VSYS_KEY_ESCAPE)) {
      break;
    }
    if (VSYS_WindowKeyUpGet(window, VSYS_KEY_SPACE)) {
      printf("%zu:\tUnspaced\t", frame);
    } else {
      printf("%zu:\tSpaced\t", frame);
    }
    f32 window_size[2];
    VSYS_WindowSizeGet(window, window_size);
    printf("%3.1f:%3.1f\n", window_size[0], window_size[1]);
    frame++;
    
  }

  VSYS_WindowDestroy(window);
  VSYS_Finalize();
  
  volkFinalize();
  return 0;
}  
