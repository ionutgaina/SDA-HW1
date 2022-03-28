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
    if (station == NULL || station->platforms == NULL)
        return;

    if (weight < 0)
        return;

    if (station->platforms_no > platform && platform >= 0)
        if (station->platforms[platform]->locomotive_power != -1)
        {
            if (station->platforms[platform]->train_cars == NULL)
                return;

            TrainCar *aux = station->platforms[platform]->train_cars;
            TrainCar *prev;

            while (aux != NULL)
            {
                if (aux->weight == weight)
                {
                    if (aux == station->platforms[platform]->train_cars)
                    {
                        station->platforms[platform]->train_cars = aux->next;
                        free(aux);
                        aux = station->platforms[platform]->train_cars;
                    }
                    else if (aux->next == NULL)
                    {
                        prev->next = NULL;
                        free(aux);
                        aux = NULL;
                    }
                    else
                    {
                        prev->next = aux->next;
                        free(aux);
                        aux = prev->next;
                    }
                }
                else
                {
                    prev = aux;
                    aux = aux->next;
                }
            }
        }
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
typedef struct TrainPosition
{
    TrainCar *prev;
    TrainCar *current;
} TrainPosition;

TrainPosition *return_points(TrainCar *train_car, int pos)
{
    TrainPosition *train_point = malloc(sizeof(TrainPosition));
    int c = 1;
    train_point->current = train_car;
    train_point->prev = NULL;
    while (train_point->current != NULL && c != pos)
    {
        train_point->prev = train_point->current;
        train_point->current = train_point->current->next;
        c++;
    }
    if (c != pos)
    {
        free(train_point);
        return NULL;
    }
    return train_point;
}

void free_train_position(TrainPosition *car)
{
    if (car != NULL)
        free(car);
}

void move_train_car(Train *train_a, Train *train_b, int pos_a, int pos_b)
{
    TrainPosition *traincar_a = return_points(train_a->train_cars, pos_a);
    TrainPosition *traincar_b = return_points(train_b->train_cars, pos_b);

    if (traincar_a == NULL || (traincar_b == NULL && pos_b != 1))
    {
        free_train_position(traincar_a);
        free_train_position(traincar_b);
        return;
    }

    if (traincar_a->current == NULL)
    {
        free_train_position(traincar_a);
        free_train_position(traincar_b);
        return;
    }

    if (traincar_b->prev == NULL)
        train_b->train_cars = traincar_a->current;
    else
        traincar_b->prev->next = traincar_a->current;

    if (traincar_a->prev == NULL)
    {
        train_a->train_cars = traincar_a->current->next;
    }
    else
    {
        traincar_a->prev->next = traincar_a->current->next;
    }

    traincar_a->current->next = traincar_b->current;

    free_train_position(traincar_a);
    free_train_position(traincar_b);
}

void move_train_cars(TrainStation *station, int platform_a, int pos_a,
                     int cars_no, int platform_b, int pos_b)
{
    if (station == NULL)
        return;
    if (station->platforms == NULL)
        return;
    Train *locomotive_a = station->platforms[platform_a];
    Train *locomotive_b = station->platforms[platform_b];
    if (locomotive_a == NULL || locomotive_b == NULL)
        return;
    if (locomotive_a->train_cars == NULL)
        return;
    if (pos_a < 1 || pos_b < 1 || cars_no < 0)
        return;

    TrainPosition *traincar_limit = return_points(locomotive_a->train_cars, pos_a + cars_no - 1);
    if (traincar_limit == NULL)
    {
        free_train_position(traincar_limit);
        return;
    }
    if (traincar_limit->current == NULL)
    {
        free_train_position(traincar_limit);
        return;
    }
    for (int i = 0; i < cars_no; i++)
    {
        move_train_car(locomotive_a, locomotive_b, pos_a, pos_b + i);
    }
    free_train_position(traincar_limit);
}

/* Gaseste trenul cel mai rapid.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */

int weight_train(Train *locomotive)
{
    int weight = 0;
    int locomotive_power = locomotive->locomotive_power;

    TrainCar *current = locomotive->train_cars;

    while (current != NULL)
    {
        weight += current->weight;
        current = current->next;
    }

    return locomotive_power - weight;
}

int find_express_train(TrainStation *station)
{
    if (station == NULL)
        return -1;
    if (station->platforms == NULL)
        return -1;

    int platform, train_weight, weight;
    platform = -1;

    for (int i = 0; i < station->platforms_no; i++)
    {
        if (station->platforms[i] != NULL)
        {
            if (station->platforms[i]->locomotive_power != -1)
            {
                if (platform == -1)
                {
                    weight = train_weight = weight_train(station->platforms[i]);
                    platform = i;
                }

                train_weight = weight_train(station->platforms[i]);
                if (train_weight >= weight)
                {
                    platform = i;
                    weight = train_weight;
                }
            }
        }
    }
    if (platform == -1)
        return -1;
    return platform;
}

/* Gaseste trenul supraincarcat.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station)
{
    if (station == NULL)
        return -1;
    if (station->platforms == NULL)
        return -1;

    int platform, train_weight, weight;
    platform = -1;

    for (int i = 0; i < station->platforms_no; i++)
    {
        if (station->platforms[i] != NULL)
        {
            if (station->platforms[i]->locomotive_power != -1)
            {
                if (platform == -1)
                {
                    weight = train_weight = weight_train(station->platforms[i]);
                    platform = i;
                }

                train_weight = weight_train(station->platforms[i]);
                if (train_weight <= weight)
                {
                    platform = i;
                    weight = train_weight;
                }
            }
        }
    }
    if (platform == -1 || weight >= 0)
        return -1;
    return platform;
}

/* Gaseste trenul cu incarcatura optima.
 *
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station)
{
    if (station == NULL)
        return -1;
    if (station->platforms == NULL)
        return -1;

    int platform, train_weight, weight;
    platform = -1;

    for (int i = 0; i < station->platforms_no; i++)
    {
        if (station->platforms[i] != NULL)
        {
            if (station->platforms[i]->locomotive_power != -1)
            {
                if (platform == -1)
                {
                    weight = train_weight = weight_train(station->platforms[i]);
                    platform = i;
                }

                train_weight = weight_train(station->platforms[i]);
                if (train_weight <= weight && train_weight > 0)
                {
                    platform = i;
                    weight = train_weight;
                }
            }
        }
    }
    if (platform == -1)
        return -1;
    return platform;
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
    // if (station == NULL)
    //     return -1;
    // if (station->platforms == NULL)
    //     return -1;

    // int platform, train_weight, weight;
    // platform = -1;

    // TrainCar *current, *aux_car, *aux2_car;
    // for (int i = 0; i < station->platforms_no; i++)
    // {
    //     if (station->platforms[i] != NULL)
    //     {
    //         if (station->platforms[i]->train_cars != NULL)
    //         {
    //             for (int c = 0; c < cars_no; c++)
    //             {
    //                 train_weight = 0;
    //                 current = station->platforms[i]->train_cars;

    //                 for (int j = 0; j < c && current != NULL; j++)
    //                 {
    //                     current = current->next;
    //                 }

    //                 aux_car = current;
    //                 for (int j = c; j < cars_no + c && current != NULL; j++)
    //                 {
    //                     train_weight += current->weight;
    //                     current = current->next;
    //                 }
    //                 if (current == NULL )
    //                     aux_car == NULL;

    //                 if (platform == -1 && aux_car != NULL)
    //                             weight = train_weight;
    
    //                 if (train_weight >= weight && aux_car != NULL)
    //                 {
    //                     aux2_car = aux_car;
    //                     start_car = &aux2_car;
    //                     weight = train_weight;
    //                     platform = i;
    //                 }
    //             }
    //         }
    //     }
    // }
    // if (platform == -1)
    //     {
    //         start_car = NULL;
    //         return -1;
    //     }
    // return platform;
}

/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 *
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */



void order_train(TrainStation *station, int platform)
{
    // if (station == NULL || station->platforms == NULL)
    //     return;
    // if (station->platforms[platform]->train_cars == NULL )
    //     return;

    // TrainCar *current1, *current2;

    // current1 = station->platforms[platform]->train_cars;
    // current2 = current1;

    // int c, max ;
    // max = 0;
    // while ( current1 != NULL )
    // {   
    //     while ( current2 != NULL )
    //     {   

    //         current2 = current2->next;
    //     }
    // }
}

/* Scoate un vagon din trenul supraincarcat.
 *
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station)
{
}
