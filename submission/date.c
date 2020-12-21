#include "date.h"
#include <stdlib.h>

#define MAX_DAYS 30
#define MAX_MONTH 12


struct Date_t {
    int day;
    int month;
    int year;
};

Date dateCreate(int day, int month, int year) {
    if (day < 1 || day > MAX_DAYS || month < 1 || month > MAX_MONTH) {
        return NULL;
    }

    Date date = malloc(sizeof(*date));
    if (!date) {
        return NULL;
    }
    date->day = day;
    date->month = month;
    date->year = year;
    return date;
}

void dateDestroy(Date date) {
    free(date);
    return;
}

Date dateCopy(Date date) {
    if (!date) {
        return NULL;
    }

    Date new_date = malloc(sizeof(*date));
    if (!new_date) {
        return NULL;
    }

    new_date->day = date->day;
    new_date->month = date->month;
    new_date->year = date->year;

    return new_date;
}

bool dateGet(Date date, int* day, int* month, int* year) {
    if (!date || !day || !month || !year) {
        return false;
    }

    *day = date->day;
    *month = date->month;
    *year = date->year;
    return true;
}

int dateCompare(Date date1, Date date2) {
    if (!date1 || !date2) {
        return 0;
    }

    if (date1->year != date2->year) {
        return date1->year - date2->year;
    }
    if (date1->month != date2->month) {
        return date1->month - date2->month;
    }
    return date1->day - date2->day;
}




void dateTick(Date date) {
    if (!date) {
        return;
    }
    if (date->day < MAX_DAYS) {
        date->day++;
        return;
    }
    date->day = 1;
    if (date->month < MAX_MONTH) {
        date->month++;
        return;
    }
    date->month = 1;
    date->year++;
    return;
}




