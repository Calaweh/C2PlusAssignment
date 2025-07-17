#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

struct Event
{
    enum class Status
    {
        Upcoming,
        Ongoing,
        Completed,
        Cancelled
    };

    const std::string ID;
    std::string name;
    std::string description;
    std::vector<std::string> organizerName;
    std::vector<std::string> staffId;
    int maxStall;
    Status status;
    std::chrono::system_clock::time_point startDateTime;
    std::chrono::system_clock::time_point endDateTime;

    Event(std::string id, std::string name, std::string description,
          std::vector<std::string> organizerName, std::vector<std::string> staffId, int maxStall,
          Status status, std::chrono::system_clock::time_point startDateTime,
          std::chrono::system_clock::time_point endDateTime)
        : ID(std::move(id)), name(std::move(name)), description(std::move(description)),
          organizerName(std::move(organizerName)), staffId(std::move(staffId)), maxStall(maxStall),
          status(status), startDateTime(startDateTime), endDateTime(endDateTime) {}
};

struct Booking
{
    enum class Status
    {
        Pending,
        Confirmed,
        Cancelled
    };

    const std::string ID;
    std::string eventID;
    std::string customerID;
    std::chrono::system_clock::time_point bookingTime;
    std::chrono::system_clock::time_point pendingTime;
    double bookingFee;
    Status status;

    Booking(std::string id, std::string eventID, std::string customerID,
            std::chrono::system_clock::time_point bookingTime,
            std::chrono::system_clock::time_point pendingTime, double bookingFee,
            Status status)
        : ID(std::move(id)), eventID(std::move(eventID)), customerID(std::move(customerID)),
          bookingTime(bookingTime), pendingTime(pendingTime), bookingFee(bookingFee), status(status) {}
};

struct Stall
{
    enum class Status
    {
        Available,
        Booked,
        UnderMaintenance
    };
    const std::string ID;
    const std::string stallName; // Exmaple: Section A
    double rentalFee;
    Status status;
    Vendor vendor;

    Stall(std::string id, std::string stallName, double rentalFee, Status status, Vendor vendor)
        : ID(std::move(id)), stallName(std::move(stallName)), rentalFee(rentalFee), status(status), vendor(std::move(vendor)) {}
};

class Identity
{

protected:
    const std::string ID;
    std::string pwd;
    std::string name;
    std::string email;
    std::string phoneNumber;

public:
    Identity(std::string id, std::string pwd, std::string name, std::string email, std::string phoneNumber)
        : ID(std::move(id)), pwd(std::move(pwd)), name(std::move(name)), email(std::move(email)), phoneNumber(std::move(phoneNumber)) {}
};

struct Address
{
    const std::string ID;
    std::string addressLine;
    std::string zipCode;
    std::string city;
    std::string state;
    std::string country;

    Address(std::string id, std::string addressLine, std::string zipCode,
            std::string city, std::string state, std::string country)
        : ID(std::move(id)), addressLine(std::move(addressLine)), zipCode(std::move(zipCode)),
          city(std::move(city)), state(std::move(state)), country(std::move(country)) {}
};

class Customer : public Identity
{
    std::vector<Address> addresses;

public:
    Customer(std::string id, std::string pwd, std::string name, std::string email, std::string phoneNumber,
             std::vector<Address> addresses)
        : Identity(std::move(id), std::move(pwd), std::move(name), std::move(email), std::move(phoneNumber)), addresses(std::move(addresses)) {}
};

class Vendor : public Identity
{
public:
    Vendor(std::string id, std::string pwd, std::string name, std::string email, std::string phoneNumber)
        : Identity(std::move(id), std::move(pwd), std::move(name), std::move(email), std::move(phoneNumber)) {}
};

class Staff : public Identity
{
public:
    Staff(std::string id, std::string pwd, std::string name, std::string email, std::string phoneNumber)
        : Identity(std::move(id), std::move(pwd), std::move(name), std::move(email), std::move(phoneNumber)) {}
};

struct Item
{

    struct Category
    {
        const std::string ID;
        std::string name;
        std::string description;
        std::string parentCategoryId;
    };

    enum class ProductStatus
    {
        Available,
        Unavailable,
        Discontinued
    };

    struct Specification
    {
        const std::string ID;
        std::string name;
        std::string description;
        double additionalPrice;
    };

    const std::string ID;
    std::string name;
    std::string description;
    double price;
    int stockQuantity;
    Category category;
    std::string vendorID;
    ProductStatus status;
    std::vector<std::string> specifications;

    Item(std::string id, std::string name, std::string description, double price, int stockQuantity,
         Category category, std::string vendorID, ProductStatus status, std::vector<std::string> specifications)
        : ID(std::move(id)), name(std::move(name)), description(std::move(description)), price(price),
          stockQuantity(stockQuantity), category(std::move(category)), vendorID(std::move(vendorID)),
          status(status), specifications(std::move(specifications)) {}
};

struct Report
{
    enum class Type
    {
        Sales,
        Inventory,
        CustomerFeedback,
        EventPerformance
    };

    const std::string ID;
    Type type;
    std::chrono::system_clock::time_point generatedDateTime;
    std::string content;
    std::string IdOfGeneratedBy;

    Report(std::string id, Type type, std::chrono::system_clock::time_point generatedDateTime, std::string content, std::string IdOfGeneratedBy)
        : ID(std::move(id)), type(type), generatedDateTime(generatedDateTime), content(std::move(content)), IdOfGeneratedBy(std::move(IdOfGeneratedBy)) {}

};

struct MonitoringRecord
{
    enum class Type
    {
        Attendance,
        Performance,
        Feedback
    };

    const std::string ID;
    std::vector<std::unique_ptr<Identity>> involvedIdentities;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    std::string observation;

    MonitoringRecord(std::string id, std::vector<std::unique_ptr<Identity>> involvedIdentities,
                     std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime,
                     std::string observation)
        : ID(std::move(id)), involvedIdentities(std::move(involvedIdentities)), startTime(startTime),
          endTime(endTime), observation(std::move(observation)) {}
};

struct MarketingCampaign
{
    enum class Type
    {
        Email,
        SocialMedia,
        EventPromotion
    };

    enum class Status
    {
        Planned,
        Ongoing,
        Completed,
        Cancelled,
        Expired
    };

    const std::string ID;
    std::string name;
    std::string description;
    std::chrono::system_clock::time_point startDateTime;
    std::chrono::system_clock::time_point endDateTime;
    bool isPercentageBased;
    double discountValue;
    Status status;

    MarketingCampaign(std::string id, std::string name, std::string description,
                      std::chrono::system_clock::time_point startDateTime,
                      std::chrono::system_clock::time_point endDateTime, bool isPercentageBased,
                      double discountValue, Status status)
        : ID(std::move(id)), name(std::move(name)), description(std::move(description)),
          startDateTime(startDateTime), endDateTime(endDateTime), isPercentageBased(isPercentageBased),
          discountValue(discountValue), status(status) {}
};

struct OrderList // Used for Cart purpose
{
    enum class Status
    {
        Pending,
        Confirmed,
        Shipped,
        Delivered,
        Cancelled
    };

    const std::string ID;
    std::string customerID;
    std::vector<std::string> orderIDs;
    std::chrono::system_clock::time_point orderDateTime;
    double totalAmount;
    Status status;

    OrderList(std::string id, std::string customerID, std::vector<std::string> orderIDs,
              std::chrono::system_clock::time_point orderDateTime, double totalAmount, Status status)
        : ID(std::move(id)), customerID(std::move(customerID)), orderIDs(std::move(orderIDs)),
          orderDateTime(orderDateTime), totalAmount(totalAmount), status(status) {}
};

struct Order
{
    enum class Status
    {
        Pending,
        Confirmed,
        Shipped,
        Delivered,
        Cancelled
    };

    const std::string ID;
    int quantity;
    std::string itemID;
    Status status;

    Order(std::string id, int quantity, std::string itemID, Status status)
        : ID(std::move(id)), quantity(quantity), itemID(std::move(itemID)), status(status) {}
};

struct Delivery
{

    enum class Status
    {
        Pending,
        InTransit,
        Delivered,
        Cancelled
    };
    enum class Method
    {
        Standard,
        Express,
        SameDay
    };

    const std::string ID;
    Address address;
    Status status;
    Method method;
    std::chrono::system_clock::time_point scheduledDateTime;
    std::chrono::system_clock::time_point actualDateTime;
    double deliveryFee;
    std::vector<std::string> orderIDs;

    Delivery(std::string id, Address address, Status status, Method method,
             std::chrono::system_clock::time_point scheduledDateTime,
             std::chrono::system_clock::time_point actualDateTime, double deliveryFee,
             std::vector<std::string> orderIDs)
        : ID(std::move(id)), address(std::move(address)), status(status), method(method),
          scheduledDateTime(scheduledDateTime), actualDateTime(actualDateTime), deliveryFee(deliveryFee),
          orderIDs(std::move(orderIDs)) {}
};
