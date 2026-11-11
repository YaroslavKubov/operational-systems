
#include <stdio.h>

#define MAX_N 8
#define Q 2

typedef struct {
    int id;
    int arrival;
    int burst;
    int remaining;
    int start;
    int finish;
    int done;
    int started;
} Proc;

static void reset(Proc procs[], int n) {
    for (int i = 0; i < n; i++) {
        procs[i].remaining = procs[i].burst;
        procs[i].start = -1;
        procs[i].finish = -1;
        procs[i].done = 0;
        procs[i].started = 0;
    }
}

static void report(Proc procs[], int n, const char *name) {
    double sw = 0, st = 0, sr = 0;
    printf("%s\n", name);

    for (int i = 0; i < n; i++) {
        int T = procs[i].finish - procs[i].arrival;
        int W = T - procs[i].burst;
        int R = procs[i].start - procs[i].arrival;

        sw += W;
        st += T;
        sr += R;

        printf("  P%d: start=%d finish=%d  W=%d T=%d R=%d\n",
               procs[i].id, procs[i].start, procs[i].finish, W, T, R);
    }

    printf("  Середнє: W=%.2f  T=%.2f  R=%.2f\n",
           sw / n, st / n, sr / n);
}

static void fcfs(Proc procs[], int n) {
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (time < procs[i].arrival)
            time = procs[i].arrival;

        procs[i].start = time;
        time += procs[i].burst;
        procs[i].finish = time;
    }

    report(procs, n, "FCFS");
}

static void sjf(Proc procs[], int n) {
    int time = 0;
    int left = n;

    while (left > 0) {
        int best = -1;

        for (int i = 0; i < n; i++) {
            if (!procs[i].done && procs[i].arrival <= time &&
                (best == -1 || procs[i].burst < procs[best].burst)) {
                best = i;
            }
        }

        if (best == -1) {
            int next = 1 << 30;

            for (int i = 0; i < n; i++) {
                if (!procs[i].done && procs[i].arrival < next)
                    next = procs[i].arrival;
            }

            time = next;
            continue;
        }

        procs[best].start = time;
        time += procs[best].burst;
        procs[best].finish = time;
        procs[best].done = 1;
        left--;
    }

    report(procs, n, "SJF (невитискальний)");
}

static void rr(Proc procs[], int n) {
    int queue[1000];
    int head = 0, tail = 0;
    int time = 0;
    int arrived = 0;
    int left = n;

    while (left > 0) {
        while (arrived < n && procs[arrived].arrival <= time) {
            queue[tail++] = arrived;
            arrived++;
        }

        if (head == tail) {
            if (arrived < n)
                time = procs[arrived].arrival;
            continue;
        }

        int i = queue[head++];

        if (!procs[i].started) {
            procs[i].start = time;
            procs[i].started = 1;
        }

        int run = procs[i].remaining < Q ?
                  procs[i].remaining : Q;

        procs[i].remaining -= run;
        time += run;

        while (arrived < n && procs[arrived].arrival <= time) {
            queue[tail++] = arrived;
            arrived++;
        }

        if (procs[i].remaining == 0) {
            procs[i].finish = time;
            procs[i].done = 1;
            left--;
        } else {
            queue[tail++] = i;
        }
    }

    report(procs, n, "Round-Robin (q=2)");
}

int main(void) {
    Proc procs[MAX_N] = {
        {1, 0, 5, 0, -1, -1, 0, 0},
        {2, 1, 3, 0, -1, -1, 0, 0},
        {3, 2, 1, 0, -1, -1, 0, 0},
        {4, 4, 2, 0, -1, -1, 0, 0},
    };

    int n = 4;

    reset(procs, n);
    fcfs(procs, n);

    reset(procs, n);
    sjf(procs, n);

    reset(procs, n);
    rr(procs, n);

    return 0;
}

