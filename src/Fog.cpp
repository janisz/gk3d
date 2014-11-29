#include "Fog.h"

#include <GL/gl.h>
#include <iostream>

bool g_fog_enabled = false;
float g_fog_min_distance = 1.0f;
float g_fog_max_distance = 5.0f;
float g_fog_density = 0.035f;
GLuint g_fog_filter= 0;


extern bool g_key[256];

void Fog() {
    glClearColor(0.5f,0.5f,0.5f,1.0f);          // We'll Clear To The Color Of The Fog ( Modified )

    GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };   // Storage For Three Types Of Fog
    GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};      // Fog Color

    glFogi(GL_FOG_MODE, fogMode[g_fog_filter]);        // Fog Mode
    glFogfv(GL_FOG_COLOR, fogColor);            // Set Fog Color
    glFogf(GL_FOG_DENSITY, g_fog_density);              // How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
    glFogf(GL_FOG_START, g_fog_min_distance);             // Fog Start Depth
    glFogf(GL_FOG_END, g_fog_max_distance);               // Fog End Depth
    if (g_fog_enabled) {
        glEnable(GL_FOG);                   // Enables GL_FOG
    } else {
        glDisable(GL_FOG);
    }
}

void printFogGlobals() {
    std::cout<<"\rFog Mode: "<<g_fog_filter<<"\tStart: "<<g_fog_min_distance<<"\tStop: "<<g_fog_max_distance<<"\tDens: "<<g_fog_density;
    std::flush(std::cout);
}

void ConfigureFogGlobals() {
    if (g_key['0']) {
        printFogGlobals();
        g_fog_enabled = !g_fog_enabled;
    }
    for (unsigned int i=0;i<3;i++) {
        if (g_key[i+1+'0']) {
            printFogGlobals();
            g_fog_filter = i;
        }
    }
    if (g_key['w'] || g_key['W']) {
        printFogGlobals();
        g_fog_min_distance += 0.01;
    }
    else if (g_key['s'] || g_key['S']) {
        printFogGlobals();
        g_fog_min_distance -= 0.01;
    }
    else if (g_key['a'] || g_key['A']) {
        printFogGlobals();
        g_fog_max_distance -= 0.01;
    }
    else if (g_key['d'] || g_key['D']) {
        printFogGlobals();
        g_fog_max_distance += 0.01;
    }
    else if (g_key['+']) {
        printFogGlobals();
        g_fog_density += 0.01;
    }
    else if (g_key['-']) {
        printFogGlobals();
        g_fog_density -= 0.01;
    }
}