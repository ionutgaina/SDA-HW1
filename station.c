#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"

/* Creeaza o gara cu un numar fix de peroane.
 *
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation *open_train_station(int platforms_no)
{
    if (platforms_no <= 0)
        return NULL;
    TrainStation *station = malloc(sizeof(TrainStation));

    station->platforms_no = platforms_no;
    station->platforms = malloc(platforms_no * sizeof(Train *));
    for (int i = 0; i < platforms_no; i++)
    {
        station->platforms[i] = malloc(sizeof(Train));
        station->platforms[i]->locomotive_power = -1;
        station->platforms[i]->train_cars = NULL;
    }
    return station;
}

/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */

void close_train_car(TrainCar *train_cars)
{
    if (train_cars == NULL)
        return;
    else
        close_train_car(train_cars->next);
    free(train_cars);
}

void close_train_station(TrainStation *station)
{
    if (!station)
        return;

    if (!station->platforms)
    {
        free(station);
        return;
    }

    int platforms_no = station->platforms_no;

    for (int i = 0; i < platforms_no; i++)
    {
        if (station->platforms[i] != NULL)
        {
            if (station->platforms[i]->train_cars != NULL)
            {
                close_train_car(station->platforms[i]->train_cars);
            }
            free(station->platforms[i]);
        }
    }
    free(station->platforms);
    free(station);
}

/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f)
{
    if (station == NULL || station->platforms == NULL)
        return;

    int platforms_no = station->platforms_no;

    for (int i = 0; i < platforms_no; i++)
    {
        fprintf(f, "%d: ", i);

        if (station->platforms[i]->locomotive_power != -1)
        {
            fprintf(f, "(%d)", station->platforms[i]->locomotive_power);
            TrainCar *train = station->platforms[i]->train_cars;
            while (train != NULL)
            {
                fprintf(f, "-|%d|", train->weight);
                train = train->next;
            }
        }
        fprintf(f, "\n");
    }
}

/*
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power)
{
    if (station == NULL || station->platforms == NULL)
        return;

    if (station->platforms_no > platform && platform >= 0)
        if (station->platforms[platform]->locomotive_power == -1)
        {
            station->platforms[platform]->train_cars = NULL;
            station->platforms[platform]->locomotive_power = locomotive_power;
        }
}

/* Elibereaza un peron.
 *
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform)
{
    if (station == NULL || station->platforms == NULL)
        return;

    if (station->platforms_no > platform && platform >= 0)
    {
        if (station->platforms[platform]->train_cars != NULL)
            close_train_car(station->platforms[platform]->train_cars);
        station->platforms[platform]->train_cars = NULL;
        station->platforms[platform]->locomotive_power = -1;
    }
}

/* Adauga un vagon la capatul unui tren.
 *
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight)
{
    if (station == NULL || station->platforms == NULL)
        return;

    if (weight < 0)
        return;

    if (station->platforms_no > platform && platform >= 0)
        if (station->platforms[platform]->locomotive_power != -1)
        {
            TrainCar *new_traincar = malloc(sizeof(TrainCar));
            new_traincar->next = NULL;
            new_traincar->weight = weight;

            if (station->platforms[platform]->train_cars == NULL)
            {
                station->platforms[platform]->train_cars = new_traincar;
                return;
            }
            TrainCar *aux = station->platforms[platform]->train_cars;
            while (aux->next != NULL)
            {
                aux = aux->next;
            }
            if (aux->next == NULL)
            {
                aux->next = new_traincar;
                return;
            }
        }
}

/* Scoate vagoanele de o anumita greutate dintr-un tren.
 *
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight)
{
}

/* Muta o secventa de vagoane dintr-un tren in altul.
 *
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a,
                     int cars_no, int platform_b, int pos_b)
{
}

/* Gaseste trenul cel mai rapid.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station)
{
    return -1;
}

/* Gaseste trenul supraincarcat.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station)
{
    return -1;
}

/* Gaseste trenul cu incarcatura optima.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station)
{
    return -1;
}

/* Gaseste trenul cu incarcatura nedistribuita bine.
 *
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car)
{
    return -1;
}

/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 *
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform)
{
}

/* Scoate un vagon din trenul supraincarcat.
 *
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station)
{
}
