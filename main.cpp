#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan.hpp>

int main() {
  glfwInit();

  /*std::shared_ptr<void> glfw_deleter(nullptr,
                                     [](const void *) { glfwTerminate(); });*/

  const auto app_info =
      vk::ApplicationInfo("Example", VK_MAKE_VERSION(0, 1, 0));

  uint32_t required_ext_count = 0u;
  const auto required_ext =
      glfwGetRequiredInstanceExtensions(&required_ext_count);

  std::vector<const char *> ext;
  for (uint32_t i = 0u; i != required_ext_count; ++i)
    ext.emplace_back(required_ext[i]);

  // Layer : Vulkan の API 呼び出しをフックする仕組み
  std::vector<const char *> layers;
  layers.emplace_back("VK_LAYER_LUNARG_standard_validation");

  auto instance =
      vk::createInstanceUnique(vk::InstanceCreateInfo()
                                   .setPApplicationInfo(&app_info)
                                   .setEnabledExtensionCount(ext.size())
                                   .setPpEnabledExtensionNames(ext.data())
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

  return 0;
}
