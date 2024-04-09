#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100

// Definição da estrutura de uma tarefa
typedef struct {
    char name[10];
    int period;
    int burst;
    int deadline;
    int remaining_burst;
    int missed_deadlines;
    int completed;
    int killed;
} Task;

// Função para ler as tarefas a partir do arquivo de entrada
int read_tasks(const char *filename, Task tasks[]) {
    FILE *file = fopen(filename, "r");
    int total_time = 0;
    int num_tasks = 0;

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fscanf(file, "%d", &total_time);

    while (fscanf(file, "%s %d %d", tasks[num_tasks].name, &tasks[num_tasks].period, &tasks[num_tasks].burst) != EOF) {
        tasks[num_tasks].deadline = tasks[num_tasks].period;
        tasks[num_tasks].remaining_burst = tasks[num_tasks].burst;
        tasks[num_tasks].missed_deadlines = 0;
        tasks[num_tasks].completed = 0;
        tasks[num_tasks].killed = 0;
        num_tasks++;
    }

    fclose(file);
    return total_time;
}

// Função para executar o algoritmo Rate-Monotonic (Rate)
void rate_scheduler(Task tasks[], int num_tasks, int total_time) {
    printf("EXECUTION BY RATE\n");

    int time = 0;
    int i, j;
    int current_task = -1;

    while (time < total_time) {
        for (i = 0; i < num_tasks; i++) {
            if (time % tasks[i].period == 0) {
                if (current_task == -1 || tasks[i].period < tasks[current_task].period) {
                    current_task = i;
                }
            }
        }

        if (current_task == -1) {
            printf("idle for 1 unit\n");
            time++;
        } else {
            printf("[%s] for %d units - ", tasks[current_task].name, tasks[current_task].remaining_burst);
            tasks[current_task].remaining_burst--;

            if (tasks[current_task].remaining_burst == 0) {
                if (time % tasks[current_task].period != 0) {
                    printf("K\n");
                    tasks[current_task].killed++;
                } else {
                    printf("F\n");
                    tasks[current_task].completed++;
                }
                tasks[current_task].deadline += tasks[current_task].period;
                tasks[current_task].remaining_burst = tasks[current_task].burst;
            } else {
                printf("H\n");
            }

            time++;
        }
        current_task = -1;
    }
}

// Função para executar o algoritmo Earliest-Deadline-First (EDF)
void edf_scheduler(Task tasks[], int num_tasks, int total_time) {
    printf("EXECUTION BY EDF\n");

    int time = 0;
    int i, j;
    int current_task = -1;

    while (time < total_time) {
        int min_deadline = total_time + 1;

        for (i = 0; i < num_tasks; i++) {
            if (tasks[i].deadline <= min_deadline && tasks[i].remaining_burst > 0) {
                min_deadline = tasks[i].deadline;
                current_task = i;
            }
        }

        if (current_task == -1) {
            printf("idle for 1 unit\n");
            time++;
        } else {
            printf("[%s] for %d units - ", tasks[current_task].name, tasks[current_task].remaining_burst);
            tasks[current_task].remaining_burst--;

            if (tasks[current_task].remaining_burst == 0) {
                if (time % tasks[current_task].period != 0) {
                    printf("K\n");
                    tasks[current_task].killed++;
                } else {
                    printf("F\n");
                    tasks[current_task].completed++;
                }
                tasks[current_task].deadline += tasks[current_task].period;
                tasks[current_task].remaining_burst = tasks[current_task].burst;
            } else {
                printf("H\n");
            }

            time++;
        }
        current_task = -1;
    }
}

// Função para imprimir as estatísticas finais
void print_stats(Task tasks[], int num_tasks) {
    printf("\nLOST DEADLINES\n");
    for (int i = 0; i < num_tasks; i++) {
        printf("[%s] %d\n", tasks[i].name, tasks[i].missed_deadlines);
    }

    printf("\nCOMPLETE EXECUTION\n");
    for (int i = 0; i < num_tasks; i++) {
        printf("[%s] %d\n", tasks[i].name, tasks[i].completed);
    }

    printf("\nKILLED\n");
    for (int i = 0; i < num_tasks; i++) {
        printf("[%s] %d\n", tasks[i].name, tasks[i].killed);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [input_file]\n", argv[0]);
        return 1;
    }

    Task tasks[MAX_TASKS];
    int total_time = read_tasks(argv[1], tasks);
    int num_tasks = sizeof(tasks) / sizeof(tasks[0]);

    rate_scheduler(tasks, num_tasks, total_time);
    print_stats(tasks, num_tasks);

    return 0;
}
