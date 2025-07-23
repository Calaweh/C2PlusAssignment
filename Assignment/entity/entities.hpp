#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <regex>
#include <stdexcept>

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
        std::vector<std::string> organizerIds;
        std::vector<std::string> staffIds;
        int maxStall;
        Status status;
        std::chrono::system_clock::time_point startDateTime;
        std::chrono::system_clock::time_point endDateTime;
    };

    struct Booking
    {
        enum class Status
        {
            Pending,
            Confirmed,
            Cancelled
        };

        std::string Id;
        std::string eventId;
        std::string stallId;
        std::string vendorId;
        std::chrono::system_clock::time_point bookingTime;
        std::optional<std::chrono::system_clock::time_point> pendingTime;
        double bookingFee;
        double rentalFee;
        Status status;
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
        Status status;
    };

    class Identity
    {
    public:
        std::string Id;
        std::string pwd;
        std::string name;
        std::string email;
        std::string phoneNumber;

        Identity(std::string Id, std::string pwd, std::string name, std::string email, std::string phoneNumber)
            : Id(std::move(Id)), pwd(std::move(pwd)), name(std::move(name)), email(std::move(email)), phoneNumber(std::move(phoneNumber)) {}

        Identity() = default;

        virtual ~Identity() = default;

        void setEmail(std::string email)
        {
            std::regex emailPattern("^\\w+@\\w+\\.\\w+$");

            if (regex_match(email, emailPattern))
            {
                this->email = email;
                return;
            }
            throw std::invalid_argument("Invalid Email, Please try again!");
        }

        void setPwd(std::string pwd)
        {
            std::regex pwdPattern("^(?=.*\d)(?=.*[a-zA-Z])(?=.*[!@#$%^&*()_+\-=\[\]{};':\"\\|,.<>\/?]).{8,}$");

            if (regex_match(pwd, pwdPattern))
            {
                this->pwd = pwd;
                return;
            }
            throw std::invalid_argument("Invalid Password: \n*at least one numerical character\n*at least one alphabat\n*at least one special character\n*more than 8 characters");
        }

        void setPhoneNumber(std::string phoneNumber)
        {
            if (phoneNumber.length() >= 9 && phoneNumber.length() <= 12)
            {
                this->phoneNumber = phoneNumber;
                return;
            }
            throw std::invalid_argument("Invalid Phone Number\n*9~12 digits");
        }
    };

    class Vendor : public Identity
    {
    public:
        Vendor(std::string Id, std::string pwd, std::string name, std::string email, std::string phoneNumber)
            : Identity(std::move(Id), std::move(pwd), std::move(name), std::move(email), std::move(phoneNumber)) {}

        Vendor() = default;

    };

    class Staff : public Identity
    {
    public:
        Staff(std::string Id, std::string pwd, std::string name, std::string email, std::string phoneNumber)
            : Identity(std::move(Id), std::move(pwd), std::move(name), std::move(email), std::move(phoneNumber)) {}

        Staff() = default;
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

        // struct Transaction
        // {
        //     enum class Status
        //     {
        //         Pending,
        //         Completed,
        //         Failed
        //     };

        //     std::string Id;
        //     std::string orderId;
        //     std::chrono::system_clock::time_point updateDateTime;
        //     int quantity;
        //     Status status;
        // };

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
        std::string categoryId;
        std::string vendorId;
        Status status;
        std::vector<std::string> specificationIds;

        Item(std::string Id, std::string name, std::string description, double price, std::string categoryId, std::string vendorId, Status status, std::vector<std::string> specificationIds) : Id(Id), name(name), description(description), price(price), categoryId(categoryId), vendorId(vendorId), status(status), specificationIds(specificationIds) {}

        Item() = default;
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

        Report(std::string Id, Type type, std::chrono::system_clock::time_point generatedDateTime, std::string content, std::string IdOfGeneratedBy)
            : Id(std::move(Id)), type(type), generatedDateTime(generatedDateTime), content(std::move(content)), IdOfGeneratedBy(std::move(IdOfGeneratedBy)) {}
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

        MonitoringRecord(std::string Id, std::vector<std::unique_ptr<Identity>> involvedIdentities,
                         std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime,
                         std::string observation)
            : Id(std::move(Id)), involvedIdentities(std::move(involvedIdentities)), startTime(startTime),
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
        double maxDiscountValue;
        double minOrderAmount;
        Status status;

        MarketingCampaign(std::string Id, std::string name, std::string description, std::chrono::system_clock::time_point startDateTime,
                          std::chrono::system_clock::time_point endDateTime, bool isPercentageBased, double discountValue,
                          double maxDiscountValue, double minOrderAmount, Status status) : Id(Id), name(name), description(description), startDateTime(startDateTime), endDateTime(endDateTime),
                                                                                           isPercentageBased(isPercentageBased), discountValue(discountValue), maxDiscountValue(maxDiscountValue),
                                                                                           minOrderAmount(minOrderAmount), status(status) {}

        MarketingCampaign() = default;
    };

}

// struct Address
// {
// public:
//     std::string Id;
//     std::string addressLine;
//     std::string zipCode;
//     std::string city;
//     std::string state;
//     std::string country;

//     Address(std::string Id, std::string addressLine, std::string zipCode,
//             std::string city, std::string state, std::string country)
//         : Id(std::move(Id)), addressLine(std::move(addressLine)), zipCode(std::move(zipCode)),
//           city(std::move(city)), state(std::move(state)), country(std::move(country)) {}

//     Address() = default;
// };

// class Customer : public Identity
// {
// public:
//     std::vector<Address> addresses;
//     Customer(std::string Id, std::string pwd, std::string name, std::string email, std::string phoneNumber,
//              std::vector<Address> addresses)
//         : Identity(std::move(Id), std::move(pwd), std::move(name), std::move(email), std::move(phoneNumber)), addresses(std::move(addresses)) {}
// };

// struct OrderList // Used for Cart purpose
// {
//     enum class Status
//     {
//         Pending,
//         Confirmed,
//         Shipped,
//         Delivered,
//         Cancelled
//     };

//     std::string Id;
//     std::string customerId;
//     std::vector<std::string> orderIds;
//     std::chrono::system_clock::time_point orderDateTime;
//     double totalAmount;
//     Status status;

//     OrderList(std::string Id, std::string customerId, std::vector<std::string> orderIds,
//               std::chrono::system_clock::time_point orderDateTime, double totalAmount, Status status)
//         : Id(std::move(Id)), customerId(std::move(customerId)), orderIds(std::move(orderIds)),
//           orderDateTime(orderDateTime), totalAmount(totalAmount), status(status) {}
// };

// struct Order
// {
//     enum class Status
//     {
//         Pending,
//         Confirmed,
//         Shipped,
//         Delivered,
//         Cancelled
//     };

//     std::string Id;
//     int quantity;
//     std::string itemId;
//     Status status;

//     Order(std::string Id, int quantity, std::string itemId, Status status)
//         : Id(std::move(Id)), quantity(quantity), itemId(std::move(itemId)), status(status) {}
// };

// struct Delivery
// {

//     enum class Status
//     {
//         Pending,
//         InTransit,
//         Delivered,
//         Cancelled
//     };
//     // enum class Method
//     // {
//     //     Standard,
//     //     Express,
//     //     SameDay
//     // };

//     std::string Id;
//     Address address;
//     Status status;
//     // Method method;
//     std::chrono::system_clock::time_point scheduledDateTime;
//     std::chrono::system_clock::time_point actualDateTime;
//     double deliveryFee;
//     std::vector<std::string> orderIds;

//     Delivery(std::string Id, Address address, Status status,
//              std::chrono::system_clock::time_point scheduledDateTime,
//              std::chrono::system_clock::time_point actualDateTime, double deliveryFee,
//              std::vector<std::string> orderIds)
//         : Id(std::move(Id)), address(std::move(address)), status(status),
//           scheduledDateTime(scheduledDateTime), actualDateTime(actualDateTime), deliveryFee(deliveryFee),
//           orderIds(std::move(orderIds)) {}
// };