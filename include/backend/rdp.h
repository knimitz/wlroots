#ifndef BACKEND_RDP_H
#define BACKEND_RDP_H
#include <wlr/backend/rdp.h>
#include <wlr/backend/interface.h>
#include <wlr/types/wlr_output.h>
#include <freerdp/freerdp.h>
#include <freerdp/listener.h>
#include <freerdp/update.h>
#include <freerdp/input.h>
#include <freerdp/codec/color.h>
#include <freerdp/codec/rfx.h>
#include <freerdp/codec/nsc.h>
#include <freerdp/locale/keyboard.h>

#define MAX_FREERDP_FDS 64

struct wlr_rdp_output {
	struct wlr_output wlr_output;
	struct wlr_rdp_backend *backend;

	void *egl_surface;
	struct wl_event_source *frame_timer;
	int frame_delay; // ms
};

enum wlr_rdp_peer_flags {
	RDP_PEER_ACTIVATED = 1 << 0,
	RDP_PEER_OUTPUT_ENABLED = 1 << 1,
};

struct wlr_rdp_peer_context {
	rdpContext _p;

	struct wlr_rdp_backend *backend;
	struct wl_event_source *events[MAX_FREERDP_FDS];
	freerdp_peer *peer;
	uint32_t flags;
	RFX_CONTEXT *rfx_context;
	wStream *encode_stream;
	RFX_RECT *rfx_rects;
	NSC_CONTEXT *nsc_context;

	struct wlr_rdp_output *output;

	struct wl_list link;
};

struct wlr_rdp_backend {
	struct wlr_backend backend;
	struct wlr_egl egl;
	struct wlr_renderer *renderer;
	struct wl_display *display;
	struct wl_listener display_destroy;

	const char *tls_cert_path;
	const char *tls_key_path;

	freerdp_listener *listener;
	struct wl_event_source *listener_events[MAX_FREERDP_FDS];

	struct wl_list clients;
};

struct wlr_rdp_backend *rdp_backend_from_backend(
	struct wlr_backend *wlr_backend);
bool rdp_configure_listener(struct wlr_rdp_backend *backend);
int rdp_peer_init(freerdp_peer *client, struct wlr_rdp_backend *backend);
struct wlr_rdp_output *wlr_rdp_output_create(struct wlr_rdp_backend *backend,
		unsigned int width, unsigned int height);

#endif
