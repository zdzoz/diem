// Created by: Oskar Zdziarski 2023

#include "app.h"

int main(void)
{
    init_app();

    int res = run_app();

    free_app();
    return res;
}
