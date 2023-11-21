#pragma once
#include "stdafx.h"
#include "Physio.h"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(unsigned char key, int x, int y)
{
    Physio::OnKeyDown(key);
}

static void mouse_button_callback(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        Physio::OnMouseButtonDown(button, x, y);
    }
}