
/*Note to self: How to make a grid */

void raylib_grid (void)
{
  printf ("Hello world!");
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow (screenWidth, screenHeight,
              "raylib [shapes] example - easings rectangle array");

  Rectangle recs[MAX_RECS_X * MAX_RECS_Y] = {0};

  for (int y = 0; y < MAX_RECS_Y; y++)
  {
    for (int x = 0; x < MAX_RECS_X; x++)
    {
      recs[y * MAX_RECS_X + x].x = RECS_SIZE / 2.0f + RECS_SIZE * x;
      recs[y * MAX_RECS_X + x].y = RECS_SIZE / 2.0f + RECS_SIZE * y;
      recs[y * MAX_RECS_X + x].width = RECS_SIZE;
      recs[y * MAX_RECS_X + x].height = RECS_SIZE;
    }
  }

  float rotation = 0.0f;
  int framesCounter = 0;
  int state = 0; // Rectangles animation state: 0-Playing, 1-Finished

  SetTargetFPS (60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose ()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------

    // When animation has finished, press space to restart
    framesCounter = 0;

    for (int i = 0; i < MAX_RECS_X * MAX_RECS_Y; i++)
    {
      recs[i].height = RECS_SIZE / 2;
      recs[i].width = RECS_SIZE / 2;
    }

    state = 0;

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing ();

    ClearBackground (RAYWHITE);

    if (state == 0)
    {
      for (int i = 0; i < MAX_RECS_X * MAX_RECS_Y; i++)
      {
        DrawRectanglePro (recs[i], (Vector2){recs[i].width / 2, recs[i].height / 2},
                          rotation, RED);
      }
    }
    else if (state == 1)
      DrawText ("PRESS [SPACE] TO PLAY AGAIN!", 240, 200, 20, GRAY);

    EndDrawing ();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow (); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
