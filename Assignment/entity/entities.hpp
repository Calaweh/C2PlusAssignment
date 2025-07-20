#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

namespace App
{

    struct Event
    {
        enum class Status
        {
            Upcoming,
            Ongoing,
            Completed,
            Cancelled
        };

        std::string Id;
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
            : Id(std::move(id)), name(std::move(name)), description(std::move(description)),
              organizerName(std::move(organizerName)), staffId(std::move(staffId)), maxStall(maxStall),
              status(status), startDateTime(startDateTime), endDateTime(endDateTime) {}
    };

    struct Vendor;

    struct Booking
    {
        enum class Status
        {
            Pending,
            Confirmed,
            Cancelled
        };

        std::string Id;
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
            : Id(std::move(id)), eventID(std::move(eventID)), customerID(std::move(customerID)),
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
        std::string Id;
        std::string stallName; // Exmaple: Section A
        double rentalFee;
        Status status;
        std::string vendorId;

        Stall(std::string id, std::string stallName, double rentalFee, Status status, std::string vendorId)
            : Id(std::move(id)), stallName(std::move(stallName)), rentalFee(rentalFee), status(status), vendorId(std::move(vendorId)) {}
    };

    class Identity
    {
    public:
        std::string Id;
        std::string pwd;
        std::string name;
        std::string email;
        std::string phoneNumber;

        Identity(std::string id, std::string pwd, std::string name, std::string email, std::string phoneNumber)
            : Id(std::move(id)), pwd(std::move(pwd)), name(std::move(name)), email(std::move(email)), phoneNumber(std::move(phoneNumber)) {}

        virtual ~Identity() = default;
    };

    struct Address
    {
    public:
        std::string Id;
        std::string addressLine;
        std::string zipCode;
        std::string city;
        std::string state;
        std::string country;

        Address(std::string id, std::string addressLine, std::string zipCode,
                std::string city, std::string state, std::string country)
            : Id(std::move(id)), addressLine(std::move(addressLine)), zipCode(std::move(zipCode)),
              city(std::move(city)), state(std::move(state)), country(std::move(country)) {}
    };

    class Customer : public Identity
    {
    public:
        std::vector<Address> addresses;
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

    struct Category
    {
        std::string Id;
        std::string name;
        std::string description;
        std::string parentCategoryId;
    };

    struct Item
    {

        struct Transaction
        {
            enum class Status
            {
                Pending,
                Completed,
                Failed
            };

            std::string Id;
            std::string orderID;
            std::chrono::system_clock::time_point updateDateTime;
            int quantity;
            Status status;
        };

        enum class Status
        {
            Available,
            Unavailable,
            Discontinued
        };

        struct Specification
        {
            std::string Id;
            std::string name;
            std::string description;
            double additionalPrice;
        };

        std::string Id;
        std::string name;
        std::string description;
        double price;
        int stockQuantity;
        Category category;
        std::string vendorID;
        Status status;
        std::vector<std::string> specifications;

        Item(std::string id, std::string name, std::string description, double price, int stockQuantity,
             Category category, std::string vendorID, Status status, std::vector<std::string> specifications)
            : Id(std::move(id)), name(std::move(name)), description(std::move(description)), price(price),
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

        std::string Id;
        Type type;
        std::chrono::system_clock::time_point generatedDateTime;
        std::string content;
        std::string IdOfGeneratedBy;

        Report(std::string id, Type type, std::chrono::system_clock::time_point generatedDateTime, std::string content, std::string IdOfGeneratedBy)
            : Id(std::move(id)), type(type), generatedDateTime(generatedDateTime), content(std::move(content)), IdOfGeneratedBy(std::move(IdOfGeneratedBy)) {}
    };

    struct MonitoringRecord
    {
        enum class Type
        {
            Attendance,
            Performance,
            Feedback
        };

        std::string Id;
        std::vector<std::unique_ptr<Identity>> involvedIdentities;
        std::chrono::system_clock::time_point startTime;
        std::chrono::system_clock::time_point endTime;
        std::string observation;

        MonitoringRecord(std::string id, std::vector<std::unique_ptr<Identity>> involvedIdentities,
                         std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime,
                         std::string observation)
            : Id(std::move(id)), involvedIdentities(std::move(involvedIdentities)), startTime(startTime),
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

        std::string Id;
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
            : Id(std::move(id)), name(std::move(name)), description(std::move(description)),
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

        std::string Id;
        std::string customerID;
        std::vector<std::string> orderIDs;
        std::chrono::system_clock::time_point orderDateTime;
        double totalAmount;
        Status status;

        OrderList(std::string id, std::string customerID, std::vector<std::string> orderIDs,
                  std::chrono::system_clock::time_point orderDateTime, double totalAmount, Status status)
            : Id(std::move(id)), customerID(std::move(customerID)), orderIDs(std::move(orderIDs)),
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

        std::string Id;
        int quantity;
        std::string itemID;
        Status status;

        Order(std::string id, int quantity, std::string itemID, Status status)
            : Id(std::move(id)), quantity(quantity), itemID(std::move(itemID)), status(status) {}
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
        // enum class Method
        // {
        //     Standard,
        //     Express,
        //     SameDay
        // };

        std::string Id;
        Address address;
        Status status;
        // Method method;
        std::chrono::system_clock::time_point scheduledDateTime;
        std::chrono::system_clock::time_point actualDateTime;
        double deliveryFee;
        std::vector<std::string> orderIDs;

        Delivery(std::string id, Address address, Status status,
                 std::chrono::system_clock::time_point scheduledDateTime,
                 std::chrono::system_clock::time_point actualDateTime, double deliveryFee,
                 std::vector<std::string> orderIDs)
            : Id(std::move(id)), address(std::move(address)), status(status),
              scheduledDateTime(scheduledDateTime), actualDateTime(actualDateTime), deliveryFee(deliveryFee),
              orderIDs(std::move(orderIDs)) {}
    };
}