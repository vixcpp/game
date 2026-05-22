/**
 *
 *  @file SDLInputMap.cpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/game
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */

#include <vix/game/backends/sdl/SDLInputMap.hpp>

namespace vix::game::sdl
{
  InputKey sdl_key_to_input_key(SDL_Keycode key) noexcept
  {
    switch (key)
    {
    case SDLK_a:
      return InputKey::A;
    case SDLK_b:
      return InputKey::B;
    case SDLK_c:
      return InputKey::C;
    case SDLK_d:
      return InputKey::D;
    case SDLK_e:
      return InputKey::E;
    case SDLK_f:
      return InputKey::F;
    case SDLK_g:
      return InputKey::G;
    case SDLK_h:
      return InputKey::H;
    case SDLK_i:
      return InputKey::I;
    case SDLK_j:
      return InputKey::J;
    case SDLK_k:
      return InputKey::K;
    case SDLK_l:
      return InputKey::L;
    case SDLK_m:
      return InputKey::M;
    case SDLK_n:
      return InputKey::N;
    case SDLK_o:
      return InputKey::O;
    case SDLK_p:
      return InputKey::P;
    case SDLK_q:
      return InputKey::Q;
    case SDLK_r:
      return InputKey::R;
    case SDLK_s:
      return InputKey::S;
    case SDLK_t:
      return InputKey::T;
    case SDLK_u:
      return InputKey::U;
    case SDLK_v:
      return InputKey::V;
    case SDLK_w:
      return InputKey::W;
    case SDLK_x:
      return InputKey::X;
    case SDLK_y:
      return InputKey::Y;
    case SDLK_z:
      return InputKey::Z;

    case SDLK_0:
      return InputKey::Num0;
    case SDLK_1:
      return InputKey::Num1;
    case SDLK_2:
      return InputKey::Num2;
    case SDLK_3:
      return InputKey::Num3;
    case SDLK_4:
      return InputKey::Num4;
    case SDLK_5:
      return InputKey::Num5;
    case SDLK_6:
      return InputKey::Num6;
    case SDLK_7:
      return InputKey::Num7;
    case SDLK_8:
      return InputKey::Num8;
    case SDLK_9:
      return InputKey::Num9;

    case SDLK_ESCAPE:
      return InputKey::Escape;
    case SDLK_TAB:
      return InputKey::Tab;
    case SDLK_CAPSLOCK:
      return InputKey::CapsLock;
    case SDLK_LSHIFT:
      return InputKey::LeftShift;
    case SDLK_RSHIFT:
      return InputKey::RightShift;
    case SDLK_LCTRL:
      return InputKey::LeftControl;
    case SDLK_RCTRL:
      return InputKey::RightControl;
    case SDLK_LALT:
      return InputKey::LeftAlt;
    case SDLK_RALT:
      return InputKey::RightAlt;
    case SDLK_SPACE:
      return InputKey::Space;
    case SDLK_RETURN:
      return InputKey::Enter;
    case SDLK_BACKSPACE:
      return InputKey::Backspace;

    case SDLK_INSERT:
      return InputKey::Insert;
    case SDLK_DELETE:
      return InputKey::Delete;
    case SDLK_HOME:
      return InputKey::Home;
    case SDLK_END:
      return InputKey::End;
    case SDLK_PAGEUP:
      return InputKey::PageUp;
    case SDLK_PAGEDOWN:
      return InputKey::PageDown;

    case SDLK_LEFT:
      return InputKey::ArrowLeft;
    case SDLK_RIGHT:
      return InputKey::ArrowRight;
    case SDLK_UP:
      return InputKey::ArrowUp;
    case SDLK_DOWN:
      return InputKey::ArrowDown;

    case SDLK_F1:
      return InputKey::F1;
    case SDLK_F2:
      return InputKey::F2;
    case SDLK_F3:
      return InputKey::F3;
    case SDLK_F4:
      return InputKey::F4;
    case SDLK_F5:
      return InputKey::F5;
    case SDLK_F6:
      return InputKey::F6;
    case SDLK_F7:
      return InputKey::F7;
    case SDLK_F8:
      return InputKey::F8;
    case SDLK_F9:
      return InputKey::F9;
    case SDLK_F10:
      return InputKey::F10;
    case SDLK_F11:
      return InputKey::F11;
    case SDLK_F12:
      return InputKey::F12;

    default:
      return InputKey::Unknown;
    }
  }

  InputButton sdl_button_to_input_button(std::uint8_t button) noexcept
  {
    switch (button)
    {
    case SDL_BUTTON_LEFT:
      return InputButton::Left;

    case SDL_BUTTON_RIGHT:
      return InputButton::Right;

    case SDL_BUTTON_MIDDLE:
      return InputButton::Middle;

    case SDL_BUTTON_X1:
      return InputButton::Button4;

    case SDL_BUTTON_X2:
      return InputButton::Button5;

    default:
      return InputButton::Unknown;
    }
  }

} // namespace vix::game::sdl
