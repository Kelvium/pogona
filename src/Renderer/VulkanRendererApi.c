#include <config.h>

#ifdef POGONA_VULKAN_SUPPORT

#include <pch.h>
#include <pogona/Renderer/Vulkan/device.h>
#include <pogona/Renderer/Vulkan/instance.h>
#include <pogona/Renderer/Vulkan/physicalDevice.h>
#include <pogona/Renderer/VulkanRendererApi.h>
#include <pogona/logger.h>

VulkanRendererApiError vulkanRendererApiCreate(
		VulkanRendererApi* self, Window* window)
{
	self->window = window;
	self->vulkanGlobals = calloc(1, sizeof(VulkanGlobals));

	volkInitialize();

	VulkanRendererApiError error;
	error = vulkanCreateInstance(self);
	if (error != VULKAN_RENDERER_API_OK) {
		LOGGER_ERROR("could not create an instance: %d\n", error);
		return error;
	}
	volkLoadInstance(self->vulkanGlobals->instance);

	error = vulkanPickPhysicalDevice(self);
	if (error != VULKAN_RENDERER_API_OK) {
		LOGGER_ERROR("could not pick a physical device: %d\n", error);
		return error;
	}

	error = vulkanCreateDevice(self);
	if (error != VULKAN_RENDERER_API_OK) {
		LOGGER_ERROR("could not create a device: %d\n", error);
		return error;
	}
	volkLoadDevice(self->vulkanGlobals->device);
	return VULKAN_RENDERER_API_OK;
}

VulkanRendererApiError vulkanRendererApiDestroy(VulkanRendererApi* self)
{
	vkDestroyDevice(self->vulkanGlobals->device, NULL);
	LOGGER_TRACE("destroyed the device\n");

	vkDestroyInstance(self->vulkanGlobals->instance, NULL);
	LOGGER_TRACE("destroyed the instance\n");

	free(self->vulkanGlobals);
	return VULKAN_RENDERER_API_OK;
}

#endif
