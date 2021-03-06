#include <string>
#include <stdexcept>

#include "xSDL.hpp"

#define ERR(ex_type, msg) \
  do { \
    char LOCAL_BUF[100]; \
    std::string LOCAL_ERR_MSG{__FILE__}; \
    LOCAL_ERR_MSG += ": "; \
    snprintf(LOCAL_BUF, 100, "L(%d): ", __LINE__); \
    LOCAL_ERR_MSG += LOCAL_BUF; \
    LOCAL_ERR_MSG += (msg); \
    throw ex_type(LOCAL_ERR_MSG); \
  } while (0)

namespace xSDL {

////
// SDL

SDL::
SDL(int flags) {
  if (SDL_Init(flags) < 0) {
    throw SDLInitError(SDL_GetError());
  }
}

SDL::
~SDL() {
  SDL_Quit();
}

////
// surface

Surface::
Surface(Surface *src, const Rect& rect)
  : surf(SDL_CreateRGBSurface(0, rect.w, rect.h,
                              src->surf->format->BitsPerPixel,
                              src->surf->format->Rmask,
                              src->surf->format->Gmask,
                              src->surf->format->Bmask,
                              src->surf->format->Amask))
{
  if (!surf) {
    ERR(ResourceCreateError, SDL_GetError());
  }
  if (SDL_BlitSurface(src->surf, &rect, surf, nullptr) < 0) {
    SDL_FreeSurface(surf);
    ERR(ResourceCreateError, SDL_GetError());
  }
}

int
Surface::
width() const noexcept {
  return surf->w;
}

int
Surface::
height() const noexcept {
  return surf->h;
}

void
Surface::
free() noexcept {
  if (surf) {
    SDL_FreeSurface(surf);
    surf = nullptr;
  }
}

////
// renderer

Renderer::
Renderer(Window *win, int flags)
  : rend {SDL_CreateRenderer(win->win, -1, flags)}
{
  if (!rend) {
    ERR(ResourceCreateError, SDL_GetError());
  }
}

void
Renderer::
copy(Texture *texture,
     const Rect* src, const Rect* dest,
     double angle, const Point *center_rot,
     RenderFlip flip)
{
  if (SDL_RenderCopyEx(rend, texture->tex, src, dest, angle, center_rot,
                       flip) < 0)
  {
    ERR(RenderError, SDL_GetError());
  }
}

void
Renderer::
fill_rectangle(const Rect& rect) {
  if (SDL_RenderFillRect(rend, &rect) < 0) {
    ERR(RenderError, SDL_GetError());
  }
}

void
Renderer::
present() noexcept {
  SDL_RenderPresent(rend);
}

void
Renderer::
clear() {
  if (SDL_RenderClear(rend) < 0) {
    ERR(RenderError, SDL_GetError());
  }
}

void
Renderer::
set_draw_color(Color c) {
  if (SDL_SetRenderDrawColor(rend, c.r, c.g, c.b, c.a) < 0) {
    ERR(ResourceAlterError, SDL_GetError());
  }
}

////
// Texture

Texture::
Texture(Renderer *rend, Surface *surf)
  : tex {SDL_CreateTextureFromSurface(rend->rend, surf->surf)}
{
  if (!tex) {
    ERR(ResourceCreateError, SDL_GetError());
  }
}

void
Texture::
set_alpha_mod(uint8_t alpha_mod) {
  if (SDL_SetTextureAlphaMod(tex, alpha_mod) < 0) {
    ERR(ResourceAlterError, SDL_GetError());
  }
}

void
Texture::
set_color_mod(Color color_mod) {
  if (SDL_SetTextureColorMod(tex, color_mod.r, color_mod.g, color_mod.b) < 0) {
    ERR(ResourceAlterError, SDL_GetError());
  }
}

void
Texture::
set_blend_mode(BlendMode mode) {
  if (SDL_SetTextureBlendMode(tex, mode) < 0) {
    ERR(ResourceAlterError, SDL_GetError());
  }
}

uint8_t
Texture::
get_alpha_mod() const {
  uint8_t mod;
  if (SDL_GetTextureAlphaMod(tex, &mod) < 0) {
    ERR(ResourceReadError, SDL_GetError());
  }
  return mod;
}

Color
Texture::
get_color_mod() const {
  uint8_t r, g, b;
  if (SDL_GetTextureColorMod(tex, &r, &g, &b) < 0) {
    ERR(ResourceReadError, SDL_GetError());
  }
  return {r, g, b};
}

BlendMode
Texture::
get_blend_mode() const {
  BlendMode mode;
  if (SDL_GetTextureBlendMode(tex, &mode) < 0) {
    ERR(ResourceReadError, SDL_GetError());
  }
  return mode;
}

////
// window

Window::
Window(const char *title, int x_pos, int y_pos, int width, int height,
       int flags)
  : win {SDL_CreateWindow(title, x_pos, y_pos, width, height, flags)}
{
  //if (!win) {
    ERR(ResourceCreateError, SDL_GetError());
  //}
}

Window::
Window(const char *title, int width, int height, int flags)
  : Window {title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, flags}
{}

} // xsdl
