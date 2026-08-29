#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FLIGHTS 10
#define MAX_CREW 3
#define MAX_LOCATION_LEN 10

// Operational Constraint Parameters (in hours)
#define MAX_DUTY_HOURS 10.0
#define MIN_REST_HOURS 8.0

typedef struct {
    int flight_id;
    char origin[MAX_LOCATION_LEN];
    char destination[MAX_LOCATION_LEN];
    double departure_time; // Hours from baseline (0.0)
    double arrival_time;   // Hours from baseline (0.0)
} Flight;

typedef struct {
    int crew_id;
    char name[30];
    char current_location[MAX_LOCATION_LEN];
    double available_time;  // Time crew becomes available after rest
    double current_duty_start; // Duty start time for the current shift
    double total_duty_hours;   // Accumulator for current shift duty
    int assigned_flights_count;
} CrewMember;

// Flight comparison for chronological sorting
int compare_flights(const void *a, const void *b) {
    Flight *f1 = (Flight *)a;
    Flight *f2 = (Flight *)b;
    if (f1->departure_time < f2->departure_time) return -1;
    if (f1->departure_time > f2->departure_time) return 1;
    return 0;
}

// Check if a crew member can legally take a flight under hard constraints
bool is_feasible(const CrewMember *crew, const Flight *flight) {
    // 1. Location Continuity Constraint
    if (strcmp(crew->current_location, flight->origin) != 0) {
        return false;
    }

    // 2. Temporal & Minimum Rest Constraint
    if (flight->departure_time < crew->available_time) {
        return false;
    }

    // 3. Maximum Duty Period Constraint
    double flight_duration = flight->arrival_time - flight->departure_time;
    double new_duty_start = crew->current_duty_start;

    // If rest period was taken, start a new duty shift
    if (crew->assigned_flights_count == 0 || (flight->departure_time - crew->available_time >= MIN_REST_HOURS)) {
        new_duty_start = flight->departure_time;
    }

    double projected_duty = flight->arrival_time - new_duty_start;
    if (projected_duty > MAX_DUTY_HOURS) {
        return false;
    }

    return true;
}

// Greedy Assignment Algorithm
void schedule_crew(Flight flights[], int num_flights, CrewMember crew[], int num_crew) {
    // Sort flights chronologically by departure time
    qsort(flights, num_flights, sizeof(Flight), compare_flights);

    printf("=====================================================================\n");
    printf("                  AIRLINE CREW SCHEDULE ASSIGNMENTS                  \n");
    printf("=====================================================================\n\n");

    for (int i = 0; i < num_flights; i++) {
        Flight *f = &flights[i];
        int best_crew_idx = -1;
        double min_idle_time = 1e9;

        // Find the best feasible crew member (Greedy heuristic: minimum idle time)
        for (int j = 0; j < num_crew; j++) {
            if (is_feasible(&crew[j], f)) {
                double idle_time = f->departure_time - crew[j].available_time;
                if (idle_time < min_idle_time) {
                    min_idle_time = idle_time;
                    best_crew_idx = j;
                }
            }
        }

        // Apply Assignment or Report Unassigned
        if (best_crew_idx != -1) {
            CrewMember *c = &crew[best_crew_idx];

            // Update duty shift start time
            if (c->assigned_flights_count == 0 || (f->departure_time - c->available_time >= MIN_REST_HOURS)) {
                c->current_duty_start = f->departure_time;
            }

            // Update crew state
            strcpy(c->current_location, f->destination);
            c->available_time = f->arrival_time + MIN_REST_HOURS; // Enforce mandatory rest after flight
            c->total_duty_hours = f->arrival_time - c->current_duty_start;
            c->assigned_flights_count++;

            printf("[ASSIGNED] Flight %d (%s -> %s | Departure: %.1fh, Arrival: %.1fh)\n",
                   f->flight_id, f->origin, f->destination, f->departure_time, f->arrival_time);
            printf("           -> Assigned to: Crew %d (%s) | Idle Wait: %.1f hrs | Current Shift Duty: %.1f hrs\n\n",
                   c->crew_id, c->name, min_idle_time, c->total_duty_hours);
        } else {
            printf("[UNASSIGNED] Flight %d (%s -> %s | Departure: %.1fh) - No feasible crew available!\n\n",
                   f->flight_id, f->origin, f->destination, f->departure_time);
        }
    }
}

int main() {
    // Initial Flight Schedule Database
    Flight flights[MAX_FLIGHTS] = {
        {101, "JFK", "ORD", 1.0, 3.5},
        {102, "ORD", "LAX", 5.0, 9.0},
        {103, "JFK", "LHR", 2.0, 9.0},
        {104, "LAX", "SFO", 18.0, 19.5}, // Requires rest period after flight 102
        {105, "ORD", "DFW", 4.5, 7.0},
        {106, "LHR", "JFK", 18.0, 26.0},
        {107, "DFW", "LAX", 8.5, 11.5},
        {108, "SFO", "JFK", 28.0, 34.0},
        {109, "JFK", "MIA", 12.0, 15.0},
        {110, "MIA", "JFK", 24.0, 27.0}
    };

    // Initial Crew Roster & Base Locations
    CrewMember crew[MAX_CREW] = {
        {1, "Capt. Smith", "JFK", 0.0, 0.0, 0.0, 0},
        {2, "Capt. Davis", "ORD", 0.0, 0.0, 0.0, 0},
        {3, "Capt. Miller", "JFK", 0.0, 0.0, 0.0, 0}
    };

    schedule_crew(flights, MAX_FLIGHTS, crew, MAX_CREW);

    return 0;
}
