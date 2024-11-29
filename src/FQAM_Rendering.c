#include "raylib.h"
/*
    Copyright (C) 2024, Chuck Garcia

    This file is part of libfqam and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/


#include "FQAM_Rendering.h"
#include "assertf.h"
#include "raylib.h"
#include <stdio.h>

// // Renders Stage. Returns on render completion with an error code fqam_error on event
// // of error.
// FQAM_Error FQAM_show_diagram_one (void)
// {
//   const int screenWidth = 800;
//   const int screenHeight = 600;
//   InitWindow (screenWidth, screenHeight, "Raylib basic window");
//   SetTargetFPS (60);
//   while (!WindowShouldClose ())
//   {
//     BeginDrawing ();
//     ClearBackground (RAYWHITE);
//     DrawText ("It works!", 20, 20, 20, BLACK);
//     EndDrawing ();
//   }
//   CloseWindow ();
//   return 1; // Success
// }

// #include "raylib.h"

// #define RECS_WIDTH 25
// #define RECS_HEIGHT 25

// #define SCREEN_WIDTH 800
// #define SCREEN_HEIGHT 450

// #define MAX_RECS_X 4
// #define MAX_RECS_Y 2

// #define PLAY_TIME_IN_FRAMES 240 // At 60 fps = 4 secondsP

// // void init_grid (Rectangle *recs);

// FQAM_Error FQAM_show_diagram_two(void)
// {

//   const int screenWidth = SCREEN_WIDTH;
//   const int screenHeight = SCREEN_HEIGHT;

//   Rectangle recs[MAX_RECS_X * MAX_RECS_Y] = {0};

//   InitWindow (screenWidth, screenHeight,
//               "raylib [shapes] example - easings rectangle array");
//   init_grid (recs);

//   float rotation = 0.0f;
//   int framesCounter = 0;

//   SetTargetFPS (60);

//   while (!WindowShouldClose ())
//   {

//     BeginDrawing ();

//     ClearBackground (RAYWHITE);

//     for (int i = 0; i < MAX_RECS_X * MAX_RECS_Y; i++)
//     {
//       DrawRectanglePro (recs[i], (Vector2){recs[i].width, recs[i].height}, rotation,
//                         RED);
//     }

//     EndDrawing ();
//   }

//   CloseWindow ();

//   return 0;
// }

// // void init_grid (Rectangle *recs)
// // {
// //   float spacingX = (SCREEN_WIDTH - (MAX_RECS_X * RECS_WIDTH)) / MAX_RECS_X;
// //   float spacingY = (SCREEN_HEIGHT - (MAX_RECS_Y * RECS_HEIGHT)) / MAX_RECS_Y;

// //   assertf (spacingX > 0, "Error: Unhandled negative spacing error");
// //   assertf (spacingY > 0, "Error: Unhandled negative spacing error");

// //   for (int y = 0; y < MAX_RECS_Y; y++)
// //   {
// //     for (int x = 0; x < MAX_RECS_X; x++)
// //     {
// //       recs[y * MAX_RECS_X + x].x = ((RECS_WIDTH + spacingX) * x) + RECS_WIDTH * 2;
// //       recs[y * MAX_RECS_X + x].y = ((RECS_HEIGHT + spacingY) * y) + RECS_HEIGHT * 2;
// //       recs[y * MAX_RECS_X + x].width = RECS_WIDTH;
// //       recs[y * MAX_RECS_X + x].height = RECS_HEIGHT;
// //     }
// //   }
// // }