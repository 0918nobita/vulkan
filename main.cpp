#include <iostream>
#include <vulkan/vulkan.hpp>

int main() {
  const auto app_info =
      vk::ApplicationInfo("Example", VK_MAKE_VERSION(0, 1, 0));

  const std::vector<const char *> ext;

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

  std::cout << devices.size() << " device(s) found" << std::endl;

  for (const auto &device : devices) {
    const auto props = device.getProperties();
    std::cout << "[" << props.deviceName << "]" << std::endl
              << "  DeviceType: ";

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

    std::cout << std::endl
              << "  API Version: " << VK_VERSION_MAJOR(props.apiVersion) << "."
              << VK_VERSION_MINOR(props.apiVersion) << "."
              << VK_VERSION_PATCH(props.apiVersion) << std::endl;
  }

  return 0;
}
