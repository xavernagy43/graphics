#include "previous_runs.h"
#include "menu.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void previous_runs_show(void) {
    menu_draw_background();

    FILE* f = fopen(RUNS_FILENAME, "rb");
    float y = 0.7f;
    void* font = GLUT_BITMAP_HELVETICA_18;
    char buf[128];

    glColor3f(1,1,0.2f);
    glRasterPos2f(-0.4f, y);
    const char* title = "Previous Runs";
    for (const char* c = title; *c; ++c) glutBitmapCharacter(font, *c);

    y -= 0.12f;
    font = GLUT_BITMAP_HELVETICA_12;

    if (!f) {
        glColor3f(1,1,1);
        glRasterPos2f(-0.4f, y);
        const char* msg = "There is no play history";
        for (const char* c = msg; *c; ++c) glutBitmapCharacter(font, *c);
    } else {
        RunEntry runs[MAX_RUNS];
        int count = fread(runs, sizeof(RunEntry), MAX_RUNS, f);
        fclose(f);

        glColor3f(1,1,1);
        for (int i = 0; i < count; ++i) {
            y -= 0.07f;
            sprintf(buf, "%2d. %s   Score: %d", i+1, runs[i].date, runs[i].score);
            glRasterPos2f(-0.4f, y);
            for (const char* c = buf; *c; ++c) glutBitmapCharacter(font, *c);
        }
    }

    // Draw "Press Esc..." near the bottom
    glColor3f(0.8f, 0.8f, 0.8f);
    glRasterPos2f(-0.4f, -0.85f);
    const char* escmsg = "Press Esc to return to the main menu...";
    for (const char* c = escmsg; *c; ++c) glutBitmapCharacter(font, *c);
}

void previous_runs_save(int score) {
    RunEntry runs[MAX_RUNS] = {0};
    int count = 0;

    // Read old runs
    FILE* f = fopen(RUNS_FILENAME, "rb");
    if (f) {
        count = fread(runs, sizeof(RunEntry), MAX_RUNS, f);
        fclose(f);
    }

    // New run at the beginning
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    RunEntry new_run;
    strftime(new_run.date, sizeof(new_run.date), "%Y-%m-%d %H:%M", tm_info);
    new_run.score = score;

    // Shift old runs if needed
    if (count >= MAX_RUNS) {
        count = MAX_RUNS - 1; // drop the last
    }
    if (count > 0) {
        memmove(&runs[1], &runs[0], count * sizeof(RunEntry));
    }
    runs[0] = new_run;
    count++;
    if (count > MAX_RUNS) count = MAX_RUNS;

    // Write back
    f = fopen(RUNS_FILENAME, "wb");
    if (f) {
        fwrite(runs, sizeof(RunEntry), count, f);
        fclose(f);
    }
}

void previous_runs_trim(void) {
    RunEntry runs[MAX_RUNS];
    int count = 0;

    FILE* f = fopen(RUNS_FILENAME, "rb");
    if (f) {
        count = fread(runs, sizeof(RunEntry), MAX_RUNS, f);
        fclose(f);
    }

    if (count > MAX_RUNS) {
        f = fopen(RUNS_FILENAME, "wb");
        if (f) {
            fwrite(runs, sizeof(RunEntry), MAX_RUNS, f);
            fclose(f);
        }
    }
}