#include "SailDetails.h"
#include <ctime>
#include "Utils.h"
int SailDetails::uniqueID = 0;

SailDetails::SailDetails(std::string _departure) :departure(std::move(_departure)){}

SailDetails::SailDetails(const int _containers, const int _timings, std::string _departure, std::string _destination)
: timings(_timings),containers(_containers), departure(std::move(_departure)), destination(std::move(_destination)) {}

SailDetails::SailDetails(const SailDetails& other) {
    timings = other.timings;
    containers = other.containers;
    departure = other.departure;
    destination = other.destination;
}

SailDetails::SailDetails(SailDetails&& other) noexcept {
    timings = other.timings;
    containers = other.containers;
    departure = std::move(other.departure);
    destination = std::move(other.destination);
}

SailDetails& SailDetails::operator=(const SailDetails& other) {
    if (this != &other) {
        timings = other.timings;
        containers = other.containers;
        departure = other.departure;
        destination = other.destination;
    }
    return *this;
}

bool SailDetails::operator<=(const SailDetails& other) const {
    std::tm tm1 = datetime(departure);
    std::tm tm2 = datetime(other.get_departure());
    const std::time_t time1 = std::mktime(&tm1);
    const std::time_t time2 = std::mktime(&tm2);
    return time1 <= time2;
}


bool SailDetails::operator==(const SailDetails& other) const {
    std::tm tm1 = datetime(departure);
    std::tm tm2 = datetime(other.get_departure());
    const std::time_t time1 = std::mktime(&tm1);
    const std::time_t time2 = std::mktime(&tm2);
    return time1 == time2;
}

void SailDetails::set_avg_timings(const int _timings) {
    ++avg;
    this->timings = (this->timings + _timings) / avg;
}

int SailDetails::get_timings() const {
    return timings;
}

int SailDetails::get_containers() const {
    return containers;
}

const std::string& SailDetails::get_departure() const {
    return departure;
}

const std::string& SailDetails::get_destination() const {
    return destination;
}
void SailDetails::next_unique_id() {
    ++uniqueID;
}