#include <assert.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <X11/Xlib.h>

int main(int argc, char const *argv[]){

  if (argc > 1) {
    Window root;
    cairo_surface_t *surface;
    int scr;

    Display *xorg = XOpenDisplay(argv[1]);
    scr = DefaultScreen(xorg);
    root = DefaultRootWindow(xorg);

    // get the root surface on given display
    surface = cairo_xlib_surface_create(xorg, root, DefaultVisual(xorg, scr),
                                                      DisplayWidth(xorg, scr),
                                                      DisplayHeight(xorg, scr));
    // outputs PNG images
    if ((cairo_surface_write_to_png(surface, "/root/.katrologger/screenshots/screenshot.png") == 0 )){
      cairo_surface_destroy(surface); // free the memory
      return 0;
    } else {
      printf("[x] permission error for cronjob user\n"); exit(1);
    }
  }
}
