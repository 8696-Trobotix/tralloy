/*

This is an example.
Erase and replace with your own code (and license).
FTXUI Documentation: https://arthursonzogni.github.io/FTXUI/index.html

*/

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

#include <iostream>

#include <fstream>
#include <sstream>
#include <atomic>
#include <chrono>
#include <thread>
#include <string>

#include "addons/include/tralloy_data.h"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/canvas.hpp"

using namespace ftxui;
using namespace tralloy;

int main() {
    int visit {};
    if (data::exists("visit")) {
        std::string stemp;
        int btemp;
        if (data::load("visit", &stemp, &btemp)) visit = std::stoi(stemp);
    }
    std::stringstream sstemp;
    sstemp << visit + 1;
    std::string stemp;
    sstemp >> stemp;
    data::store("visit", stemp.c_str(), stemp.length());
    
    auto screen = ScreenInteractive::Fullscreen();
    int loadScrFrame {0}, loadScrDuration {100};
    std::atomic <bool> loadScrRefresh {true};
    auto loadScr {Renderer([&] {
        int x {Terminal::Size().dimx}, y {Terminal::Size().dimy - 3}, // Three additional rows for the separator and text.
            sq {std::min(x, y) * (x > y ? 4 : 2)}; // Each braille cell is 2x4.
        auto tralloy {Canvas(sq, sq)};
        tralloy.DrawPointCircleFilled(sq / 2, 
                                sq / 2, 
                                sq / 2 - 3, 
                                Color::RGB(0, (double)(loadScrFrame) / loadScrDuration * 255, 0)
        );
        auto brGold {Color::RGB((double)(loadScrFrame) / loadScrDuration * 255, 
                                (double)(loadScrFrame) / loadScrDuration * 255, 
                                0
        )};
        tralloy.DrawPointCircle(sq / 2, 
                                sq / 2, 
                                sq / 2 - 1, 
                                brGold
        );
        float bracketLines [][4] {
            {0.40, 0.10, 0.24, 0.16}, 
            {0.24, 0.16, 0.24, 0.48}, 
            {0.24, 0.48, 0.16, 0.50}, 
            {0.16, 0.50, 0.24, 0.52}, 
            {0.24, 0.52, 0.24, 0.84}, 
            {0.24, 0.84, 0.40, 0.90}
        };
        for (float *brLine : bracketLines) tralloy.DrawBlockLine(brLine[0] * sq, 
                                                                 brLine[1] * sq, 
                                                                 brLine[2] * sq, 
                                                                 brLine[3] * sq, 
                                                                 brGold
        );
        for (float *brLine : bracketLines) tralloy.DrawBlockLine((1 - brLine[0]) * sq, 
                                                                 (1 - brLine[1]) * sq, 
                                                                 (1 - brLine[2]) * sq, 
                                                                 (1 - brLine[3]) * sq, 
                                                                 brGold
        );
        return vbox({
            canvas(tralloy) | center, 
            separator(), 
            text(visit == 0 ? "Welcome!" : "Visit #" + std::to_string(visit)) | center, 
            text("Made with 💚  from Trobotix (8696)") | center | color(Color::RGB(255, 255, 0)) | bgcolor(Color::Green)
        }) | center;
    }) | Maybe([&] {
        if (loadScrFrame > loadScrDuration) {
            loadScrRefresh = false;
            return false;
        }
        return true;
    })};
    std::thread loadScrThread([&] {
        while (loadScrRefresh) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0.05s);
            screen.Post([&] {
                ++loadScrFrame;
            });
            screen.Post(Event::Custom);
        }
        if (loadScrFrame > loadScrDuration) screen.Exit();
    });

    auto finalComponent {Renderer([&] {
        return loadScr->Render();
    })};
    screen.Loop(finalComponent);
    loadScrRefresh = false;
    loadScrThread.join();
    return 0;
}

// Copyright (c) 2022, Trobotix 8696
// All rights reserved.
// Use of this source code is governed by the BSD 3-Clause License that can be found in the LICENSE file.
