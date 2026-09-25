#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SWITCH "PC9"
#define LED    "PA8"

void write_gpio(const char *path, const char *value)
{
    FILE *fp = fopen(path, "w");

    if (fp == NULL)
    {
        perror(path);
        exit(1);
    }

    fprintf(fp, "%s", value);
    fclose(fp);
}

int read_gpio(const char *path)
{
    FILE *fp = fopen(path, "r");
    int value;

    if (fp == NULL)
    {
        perror(path);
        exit(1);
    }

    fscanf(fp, "%d", &value);
    fclose(fp);

    return value;
}

int main()
{
    char direction_path[100];
    char value_path[100];

    /* Set PC9 as INPUT */
    snprintf(direction_path, sizeof(direction_path),
             "/sys/class/gpio/%s/direction", SWITCH);

    write_gpio(direction_path, "in");

    /* Set PA8 as OUTPUT */
    snprintf(direction_path, sizeof(direction_path),
             "/sys/class/gpio/%s/direction", LED);

    write_gpio(direction_path, "out");

    /* LED initially OFF */
    snprintf(value_path, sizeof(value_path),
             "/sys/class/gpio/%s/value", LED);

    write_gpio(value_path, "0");

    printf("Switch and LED program started...\n");

    while (1)
    {
        char switch_path[100];

        snprintf(switch_path, sizeof(switch_path),
                 "/sys/class/gpio/%s/value", SWITCH);

        int switch_state = read_gpio(switch_path);

        if (switch_state == 1)
        {
            /* Switch pressed */
            write_gpio(value_path, "1");
            printf("Switch Pressed - LED ON\n");
        }
        else
        {
            /* Switch released */
            write_gpio(value_path, "0");
            printf("Switch Released - LED OFF\n");
        }

        usleep(100000);   // 100 ms
    }

    return 0;
}

