#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <vulkan/vulkan.hpp>

int main() {
  glfwInit();

  const auto app_info =
      vk::ApplicationInfo("Example", VK_MAKE_VERSION(0, 1, 0));

  uint32_t required_ext_count = 0u;
  const auto required_ext =
      glfwGetRequiredInstanceExtensions(&required_ext_count);

  std::vector<const char *> extensions = {};
  for (uint32_t i = 0u; i != required_ext_count; ++i)
    extensions.emplace_back(required_ext[i]);

  // Layer : Vulkan の API 呼び出しをフックする仕組み
  std::vector<const char *> layers;
  layers.emplace_back("VK_LAYER_LUNARG_standard_validation");

  auto instance = vk::createInstanceUnique(
      vk::InstanceCreateInfo()
          .setPApplicationInfo(&app_info)
          .setEnabledExtensionCount(extensions.size())
          .setPpEnabledExtensionNames(extensions.data())
          .setPpEnabledLayerNames(layers.data()));

  auto devices = instance->enumeratePhysicalDevices();

  if (devices.empty()) {
    std::cerr << "利用可能な物理デバイスが存在しません" << std::endl;
    exit(1);
  }

  std::cout << "[Available physical devices]" << std::endl;
  std::cout << devices.size() << " device(s) found" << std::endl;

  for (const auto &device : devices) {
    // vk::PhysicalDeviceProperties : 物理デバイスの基本的な情報
    const auto props = device.getProperties();

    // vk::PhysicalDeviceProperties::deviceName : デバイス名の文字列
    std::cout << "  [" << props.deviceName << "]" << std::endl
              << "    DeviceType: ";

    // vk::PhysicalDeviceProperties::deviceType : デバイスの種類
    switch (props.deviceType) {
    case vk::PhysicalDeviceType::eIntegratedGpu:
      std::cout << "Integrated GPU";
      break;
    case vk::PhysicalDeviceType::eDiscreteGpu:
      std::cout << "Discrete GPU";
      break;
    case vk::PhysicalDeviceType::eVirtualGpu:
      std::cout << "Virtual GPU";
      break;
    case vk::PhysicalDeviceType::eCpu:
      std::cout << "CPU";
      break;
    case vk::PhysicalDeviceType::eOther:
      std::cout << "Other";
      break;
    }

    // vk::PhysicalDeviceProperties::
    //   apiVersion : デバイスのサポートする Vulkan のバージョン
    //   vendorID : デバイスのベンダーを識別する ID
    //   deviceID : 同一ベンダーの異なるデバイスを識別する ID
    std::cout << std::endl
              << "    API Version: " << VK_VERSION_MAJOR(props.apiVersion)
              << "." << VK_VERSION_MINOR(props.apiVersion) << "."
              << VK_VERSION_PATCH(props.apiVersion) << std::endl
              << "    Vendor ID: " << props.vendorID << std::endl
              << "    Device ID: " << props.deviceID << std::endl;
  }

  // GLFW の提供するサーフェスに出力できない物理デバイスを除外する
  devices.erase(
      std::remove_if(
          devices.begin(), devices.end(),
          [&](const auto &device) -> bool {
            auto avail_dexts = device.enumerateDeviceExtensionProperties();
            for (const char *ext : extensions)
              if (std::find_if(avail_dexts.begin(), avail_dexts.end(),
                               [&](const auto &dext) {
                                 return !strcmp(dext.extensionName, ext);
                               }) == avail_dexts.end())
                return true;

            const auto avail_dlayers = device.enumerateDeviceLayerProperties();
            for (const char *layer : layers)
              if (std::find_if(avail_dlayers.begin(), avail_dlayers.end(),
                               [&](const auto &dlayer) {
                                 return !strcmp(dlayer.layerName, layer);
                               }) == avail_dlayers.end())
                return true;

            const auto queue_props = device.getQueueFamilyProperties();
            bool has_compatible_queue = false;
            for (uint32_t i = 0; i < queue_props.size(); ++i)
              if (glfwGetPhysicalDevicePresentationSupport(*instance, device,
                                                           i)) {
                has_compatible_queue = true;
                break;
              }

            return !has_compatible_queue;
          }),
      devices.end());

  // GLFW に、今から作るウィンドウで OpenGL を使う意思がないことを伝える
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  // glfwCreateWindow : ウィンドウの生成
  GLFWwindow *window = glfwCreateWindow(500, 500, "", nullptr, nullptr);
  if (!window) {
    std::cerr << "Cannot create window" << std::endl;
    glfwTerminate();
    return 1;
  }

  VkSurfaceKHR surface;
  // glfwCreateWindowSurface : GLFW が提供する VkSurfaceKHR を取得する
  if (glfwCreateWindowSurface(instance.get(), window, nullptr, &surface) !=
      VK_SUCCESS) {
    std::cerr << "Cannot create surface" << std::endl;
    return 1;
  }

  return 0;
}
