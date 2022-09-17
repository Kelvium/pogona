/*
 * Copyright (c) 2022, Nikita Romanyuk <kelvium@yahoo.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <config.h>

#ifdef POGONA_VULKAN_SUPPORT

#include "vulkan_core.h"
#include "vulkan_surface.h"

#ifdef POGONA_WAYLAND_SUPPORT
#include "../../window/wayland_window_api.h"
#endif

#include <pch.h>

#ifdef POGONA_WAYLAND_SUPPORT
static i32 sCreateWaylandSurface(WaylandWindowApi* api, VkSurfaceKHR* surface)
{
	struct wl_display* waylandDisplay = NULL;
	struct wl_surface* waylandSurface = NULL;
	waylandWindowApiGetDisplay(api, &waylandDisplay);
	waylandWindowApiGetSurface(api, &waylandSurface);
	assert(waylandDisplay);
	assert(surface);

	VkWaylandSurfaceCreateInfoKHR waylandSurfaceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
		.display = waylandDisplay,
		.surface = waylandSurface,
	};
	PVK_VERIFY(vkCreateWaylandSurfaceKHR(gVulkanCore.instance, &waylandSurfaceCreateInfo, NULL, surface));
	return 0;
}
#endif

i32 vulkanCreateSurface(Window* window, VkSurfaceKHR* surface)
{
	switch (window->apiType) {
#ifdef POGONA_WAYLAND_SUPPORT
	case WINDOW_API_TYPE_WAYLAND: {
		i32 error = sCreateWaylandSurface((WaylandWindowApi*) window->api, surface);
		if (error < 0) {
			LOGGER_ERROR("could not create wayland surface for vulkan\n");
			return -1;
		}
		break;
	}
#endif
	default:
		LOGGER_ERROR("window api is not supported with vulkan\n");
		return -1;
	}
	return 0;
}

#endif
