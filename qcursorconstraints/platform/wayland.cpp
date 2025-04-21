#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>
#include <cstring>
#include <wayland-client.h>
#include "wayland-pointer-constraints-unstable-v1-client-protocol.h"

#include "../qcursorconstraints.h"

// wl_proxy* (*wl_proxy_marshal_flags)(struct wl_proxy *proxy, uint32_t opcode, const struct wl_interface *interface, uint32_t version, uint32_t flags, ...) = NULL;
// uint32_t (*wl_proxy_get_version)(struct wl_proxy *proxy) = NULL;
// int (*wl_proxy_add_listener)(struct wl_proxy *proxy, void (**implementation)(void), void *data) = NULL;
// int (*wl_display_roundtrip)(struct wl_display *display) = NULL;

static wl_display *display = NULL;
static wl_seat* seat = NULL;
static wl_pointer* pointer = NULL;
static wl_compositor* compositor = NULL;
static zwp_pointer_constraints_v1* pointer_constraints = NULL;

static bool cursorLocked = false;
static zwp_locked_pointer_v1* lockedPointer;
static zwp_confined_pointer_v1* confinedPointer;

static void seat_handle_capabilities(void *, struct wl_seat *seat,
                                     uint32_t capabilities) {
    if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
        // Grab pointer
        pointer = wl_seat_get_pointer(seat);
    }
}

static void seat_handle_name(void *, struct wl_seat*, const char*) { }

const struct wl_seat_listener seat_listener = {
    .capabilities = seat_handle_capabilities,
    .name = seat_handle_name,
};

void handle_global(void *, wl_registry *registry,
                   uint32_t name, const char *interface, uint32_t version)
{
    if (strcmp(interface, zwp_pointer_constraints_v1_interface.name) == 0) {
        pointer_constraints = (zwp_pointer_constraints_v1*) wl_registry_bind(registry, name, &zwp_pointer_constraints_v1_interface, version);
    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        seat = (wl_seat*) wl_registry_bind(registry, name, &wl_seat_interface, version);
        wl_seat_add_listener(seat, &seat_listener, NULL);
    } else if (strcmp(interface, wl_compositor_interface.name) == 0) {
        compositor = (wl_compositor*) wl_registry_bind(registry, name, &wl_compositor_interface, version);
    }
}

const struct wl_registry_listener registry_listener = {
    .global = handle_global,
    .global_remove = NULL,
};

static void init_wayland() {
    QPlatformNativeInterface* native = QGuiApplication::platformNativeInterface();

    // You *HAVE* to use Qt's existing connection, not your own, otherwise wl protocol will not recognize your surface.
    display = (struct wl_display*)native->nativeResourceForWindow("display", NULL);
    // Add registry
    wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(display);
}

static bool unlockCursor_wayland(QWindow* window) {
    if (!cursorLocked) return false;

    if (lockedPointer) {
        zwp_locked_pointer_v1_destroy(lockedPointer);
        lockedPointer = NULL;
    }

    if (confinedPointer) {
        zwp_confined_pointer_v1_destroy(confinedPointer);
        confinedPointer = NULL;
    }

    cursorLocked = false;
    return true;
}

static bool lockCursor_wayland(QWindow* window, QPoint pos) {
    if (cursorLocked) unlockCursor_wayland(window);
    QPlatformNativeInterface* native = QGuiApplication::platformNativeInterface();

    wl_surface* surface = (wl_surface*) native->nativeResourceForWindow("surface", window);
    lockedPointer = zwp_pointer_constraints_v1_lock_pointer(pointer_constraints, surface, pointer, NULL, ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_PERSISTENT);
    cursorLocked = true;
    return true;
}

static bool confineCursor_wayland(QWindow* window, QRect rect) {
    if (cursorLocked) unlockCursor_wayland(window);
    QPlatformNativeInterface* native = QGuiApplication::platformNativeInterface();

    wl_region* region = wl_compositor_create_region(compositor);
    wl_region_add(region, rect.x(), rect.y(), rect.width(), rect.height());
    wl_display_roundtrip(display);

    wl_surface* surface = (wl_surface*) native->nativeResourceForWindow("surface", window);
    confinedPointer = zwp_pointer_constraints_v1_confine_pointer(pointer_constraints, surface, pointer, region, ZWP_POINTER_CONSTRAINTS_V1_LIFETIME_PERSISTENT);
    cursorLocked = true;
    return true;
}

QCursorConstraintsImpl QCursorConstraints::waylandImpl {
    /* .init = */ &init_wayland,
    /* .lockCursor = */ &lockCursor_wayland,
    /* .confineCursor = */ &confineCursor_wayland,
    /* .unlockCursor = */ &unlockCursor_wayland,
};
