/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include <stdio.h>
#include "raylib.h"
#include "FQAM_Rendering.h"

// Renders Stage. Returns on render completion with an error code fqam_error on event of error.
FQAM_Error FQAM_show_diagram (void)
{
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Raylib basic window");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("It works!", 20, 20, 20, BLACK);
    EndDrawing();
  }
  CloseWindow();
  return 1; // Success
}