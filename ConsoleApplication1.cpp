#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Event {
    int customer_no;
    double interarrival_time;
    double service_time;
    double service_start;
    double service_end;
    int no_in_system;
    int no_in_queue;
    double waiting_time;
    double time_in_system;
    double clock_time;
    double idle_time;
};

int main() {
    std::ifstream infile("input_data.txt"); // Assuming input data is stored in a file named "input_data.txt"
    if (!infile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<Event> events;

    double total_service_time = 0.0;
    double total_waiting_time = 0.0;
    double last_departure_time = 0.0;
    double interarrival_time, service_time;
    int customer_no = 1; // Counter for customer number
    double prev_clock_time = 0.0; // Initialize previous clock time
    int total_customers_who_waited = 0;
    int total_customers = 0;

    while (infile >> interarrival_time >> service_time) {
        Event event;
        event.customer_no = customer_no++;
        event.interarrival_time = interarrival_time;
        event.service_time = service_time;
        event.clock_time = prev_clock_time + event.interarrival_time; // Set event's clock time

        // Update service start and end times
        event.service_start = std::max(event.clock_time, last_departure_time);
        event.service_end = event.service_start + service_time;

        // Update queue and service times
        int no_in_queue = 0;
        for (const auto& e : events) {
            if (e.service_end > event.clock_time) {
                no_in_queue++;
            }
        }
        event.no_in_queue = no_in_queue;
        event.no_in_system = event.no_in_queue + 1;
        
        // Calculate waiting time for this event
        double wait_time = (event.service_start - event.clock_time);
        total_waiting_time += wait_time;
        if (wait_time > 0) {
            total_customers_who_waited++;
        }
        event.waiting_time = wait_time;

        // Calculate idle time
        double idle_time = event.clock_time - last_departure_time;
        event.idle_time = std::max(0.0, idle_time); // Ensure idle time is non-negative

        // Calculate time in system for this event and update total
        double time_in_system = event.service_time + event.waiting_time;
        event.time_in_system = time_in_system;

        // Update last departure time
        last_departure_time = event.service_end;

        events.push_back(event);

        // Update total service time
        total_service_time += service_time;

        // Update previous clock time for the next event
        prev_clock_time = event.clock_time;

        total_customers++;
    }

    infile.close();

    // Calculate average waiting time
    double avg_waiting_time = total_waiting_time / events.size();

    // Calculate average service time
    double avg_service_time = total_service_time / events.size();

    // Output table header
    std::cout << std::setw(12) << std::left << "Customer No"
        << std::setw(12) << "Interarrival"
        << std::setw(12) << "ClockTime"
        << std::setw(12) << "ServiceTime"
        << std::setw(12) << "ServiceStart"
        << std::setw(12) << "ServiceEnd"
        << std::setw(12) << "NoinSystem"
        << std::setw(12) << "NoinQueue"
        << std::setw(12) << "WaitingTime"
        << std::setw(12) << "TimeinSystem"
        << std::setw(12) << "IdleTime"
        << std::endl;

    double total_time_in_system = 0.0;
    double total_idle_time = 0.0;

    // Output table rows
    for (const auto& event : events) {
        std::cout << std::setw(12) << event.customer_no
            << std::setw(12) << event.interarrival_time
            << std::setw(12) << event.clock_time
            << std::setw(12) << event.service_time
            << std::setw(12) << event.service_start
            << std::setw(12) << event.service_end
            << std::setw(12) << event.no_in_system
            << std::setw(12) << event.no_in_queue
            << std::setw(12) << event.waiting_time
            << std::setw(12) << event.time_in_system
            << std::setw(12) << event.idle_time
            << std::endl;

        total_time_in_system += event.time_in_system;
        total_idle_time += event.idle_time;
    }

    // Calculate the total simulation time
    double total_simulation_time = last_departure_time;

    // Output average and probability metrics
    std::cout << "\nAverage Waiting Time in Queue: " << (total_customers_who_waited > 0 ? total_waiting_time / total_customers_who_waited : 0) << std::endl;
    std::cout << "Probability a Customer Waits: " << double(total_customers_who_waited) / total_customers << std::endl;
    std::cout << "Proportion of Idle Time: " << total_idle_time / total_simulation_time << std::endl;
    std::cout << "Proportion of Busy Time: " << total_service_time / total_simulation_time << std::endl;
    std::cout << "Average Service Time: " << total_service_time / total_customers << std::endl;
    std::cout << "Average Waiting Time: " << total_waiting_time / total_customers << std::endl;
    std::cout << "Average Time in System: " << total_time_in_system / total_customers << std::endl;

    return 0;
}
