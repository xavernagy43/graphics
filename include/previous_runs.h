#ifndef PREVIOUS_RUNS_H
#define PREVIOUS_RUNS_H

#define RUNS_FILENAME "runs.dat"
#define MAX_RUNS 20

typedef struct {
    char date[32];
    int score;
} RunEntry;

void previous_runs_show(void);
void previous_runs_save(int score);
void previous_runs_trim(void);

#endif