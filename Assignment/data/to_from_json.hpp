#include "../entity/entities.hpp" 
#include <nlohmann/json.hpp>
#include <iomanip>
#include <sstream>

namespace App
{

    using json = nlohmann::json;

    NLOHMANN_JSON_SERIALIZE_ENUM(Event::Status, {
                                                    {Event::Status::Upcoming, "Upcoming"},
                                                    {Event::Status::Ongoing, "Ongoing"},
                                                    {Event::Status::Completed, "Completed"},
                                                    {Event::Status::Cancelled, "Cancelled"},
                                                })

    NLOHMANN_JSON_SERIALIZE_ENUM(Booking::Status, {
                                                      {Booking::Status::Pending, "Pending"},
                                                      {Booking::Status::Confirmed, "Confirmed"},
                                                      {Booking::Status::Cancelled, "Cancelled"},
                                                  })

    NLOHMANN_JSON_SERIALIZE_ENUM(Stall::Status, {
                                                    {Stall::Status::Available, "Available"},
                                                    {Stall::Status::Booked, "Booked"},
                                                    {Stall::Status::UnderMaintenance, "UnderMaintenance"},
                                                })

    NLOHMANN_JSON_SERIALIZE_ENUM(Item::Transaction::Status, {
                                                                {Item::Transaction::Status::Pending, "Pending"},
                                                                {Item::Transaction::Status::Completed, "Completed"},
                                                                {Item::Transaction::Status::Failed, "Failed"},
                                                            })

    NLOHMANN_JSON_SERIALIZE_ENUM(Item::Status, {
                                                   {Item::Status::Available, "Available"},
                                                   {Item::Status::Unavailable, "Unavailable"},
                                                   {Item::Status::Discontinued, "Discontinued"},
                                               })

    NLOHMANN_JSON_SERIALIZE_ENUM(Report::Type, {
                                                   {Report::Type::Sales, "Sales"},
                                                   {Report::Type::Inventory, "Inventory"},
                                                   {Report::Type::CustomerFeedback, "CustomerFeedback"},
                                                   {Report::Type::EventPerformance, "EventPerformance"},
                                               })

    NLOHMANN_JSON_SERIALIZE_ENUM(MonitoringRecord::Type, {
                                                             {MonitoringRecord::Type::Attendance, "Attendance"},
                                                             {MonitoringRecord::Type::Performance, "Performance"},
                                                             {MonitoringRecord::Type::Feedback, "Feedback"},
                                                         })

    NLOHMANN_JSON_SERIALIZE_ENUM(MarketingCampaign::Type, {
                                                              {MarketingCampaign::Type::Email, "Email"},
                                                              {MarketingCampaign::Type::SocialMedia, "SocialMedia"},
                                                              {MarketingCampaign::Type::EventPromotion, "EventPromotion"},
                                                          })

    NLOHMANN_JSON_SERIALIZE_ENUM(MarketingCampaign::Status, {
                                                                {MarketingCampaign::Status::Planned, "Planned"},
                                                                {MarketingCampaign::Status::Ongoing, "Ongoing"},
                                                                {MarketingCampaign::Status::Completed, "Completed"},
                                                                {MarketingCampaign::Status::Cancelled, "Cancelled"},
                                                                {MarketingCampaign::Status::Expired, "Expired"},
                                                            })

    NLOHMANN_JSON_SERIALIZE_ENUM(OrderList::Status, {
                                                        {OrderList::Status::Pending, "Pending"},
                                                        {OrderList::Status::Confirmed, "Confirmed"},
                                                        {OrderList::Status::Shipped, "Shipped"},
                                                        {OrderList::Status::Delivered, "Delivered"},
                                                        {OrderList::Status::Cancelled, "Cancelled"},
                                                    })

    NLOHMANN_JSON_SERIALIZE_ENUM(Order::Status, {
                                                    {Order::Status::Pending, "Pending"},
                                                    {Order::Status::Confirmed, "Confirmed"},
                                                    {Order::Status::Shipped, "Shipped"},
                                                    {Order::Status::Delivered, "Delivered"},
                                                    {Order::Status::Cancelled, "Cancelled"},
                                                })

    NLOHMANN_JSON_SERIALIZE_ENUM(Delivery::Status, {
                                                       {Delivery::Status::Pending, "Pending"},
                                                       {Delivery::Status::InTransit, "InTransit"},
                                                       {Delivery::Status::Delivered, "Delivered"},
                                                       {Delivery::Status::Cancelled, "Cancelled"},
                                                   })

}

namespace nlohmann
{

    template <>
    struct adl_serializer<std::chrono::system_clock::time_point>
    {
        static void to_json(json &j, const std::chrono::system_clock::time_point &tp)
        {
            std::time_t time = std::chrono::system_clock::to_time_t(tp);
            std::tm tm_utc;
#ifdef _WIN32
            gmtime_s(&tm_utc, &time);
#else
            gmtime_r(&time, &tm_utc);
#endif
            std::stringstream ss;
            ss << std::put_time(&tm_utc, "%Y-%m-%dT%H:%M:%SZ");
            j = ss.str();
        }

        static void from_json(const json &j, std::chrono::system_clock::time_point &tp)
        {
            const std::string s = j.get<std::string>();
            std::tm tm = {};
            std::stringstream ss(s);
            ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
            std::time_t time;
#if defined(_WIN32)
            time = _mkgmtime(&tm);
#endif
            tp = std::chrono::system_clock::from_time_t(time);
        }
    };

    template <>
    struct adl_serializer<App::Address>
    {
        static void to_json(json &j, const App::Address &a)
        {
            j = {
                {"Id", a.Id}, {"addressLine", a.addressLine}, {"zipCode", a.zipCode}, {"city", a.city}, {"state", a.state}, {"country", a.country}};
        }

        static App::Address from_json(const json &j)
        {
            return App::Address(
                j.at("Id").get<std::string>(), j.at("addressLine").get<std::string>(),
                j.at("zipCode").get<std::string>(), j.at("city").get<std::string>(),
                j.at("state").get<std::string>(), j.at("country").get<std::string>());
        }
    };

    template <>
    struct adl_serializer<App::Booking>
    {
        static void to_json(json &j, const App::Booking &b)
        {
            j = {
                {"Id", b.Id},
                {"eventID", b.eventID},
                {"customerID", b.customerID},
                {"bookingTime", b.bookingTime},
                {"pendingTime", b.pendingTime},
                {"bookingFee", b.bookingFee},
                {"status", b.status}};
        }

        static App::Booking from_json(const json &j)
        {
            return App::Booking(j.at("Id").get<std::string>(),
                                j.at("eventID").get<std::string>(),
                                j.at("customerID").get<std::string>(),
                                j.at("bookingTime").get<std::chrono::system_clock::time_point>(),
                                j.at("pendingTime").get<std::chrono::system_clock::time_point>(),
                                j.at("bookingFee").get<double>(),
                                j.at("status").get<App::Booking::Status>());
        }
    };

    template <>
    struct adl_serializer<App::Stall>
    {
        static void to_json(json &j, const App::Stall &s)
        {
            j = {
                {"Id", s.Id},
                {"stallName", s.stallName},
                {"rentalFee", s.rentalFee},
                {"status", s.status},
                {"vendorId", s.vendorId},
            };
        }

        static App::Stall from_json(const json &j)
        {
            return App::Stall(j.at("Id").get<std::string>(),
                              j.at("stallName").get<std::string>(),
                              j.at("rentalFee").get<double>(),
                              j.at("status").get<App::Stall::Status>(),
                              j.at("vendorId").get<std::string>());
        }
    };

    template <>
    struct adl_serializer<App::Category>
    {
        static void to_json(json &j, const App::Category &c)
        {
            j = {
                {"Id", c.Id},
                {"name", c.name},
                {"description", c.description},
                {"parentCategoryId", c.parentCategoryId},
            };
        }

        static App::Category from_json(const json &j)
        {
            return App::Category(j.at("Id").get<std::string>(),
                                 j.at("name").get<std::string>(),
                                 j.at("description").get<std::string>(),
                                 j.at("parentCategoryId").get<std::string>());
        }
    };

    template <>
    struct adl_serializer<App::Item::Transaction>
    {
        static void to_json(json &j, const App::Item::Transaction &t)
        {
            j = {
                {"Id", t.Id},
                {"orderID", t.orderID},
                {"updateDateTime", t.updateDateTime},
                {"quantity", t.quantity},
                {"status", t.status},
            };
        }

        static App::Item::Transaction from_json(const json &j)
        {
            return App::Item::Transaction(j.at("Id").get<std::string>(),
                                          j.at("orderID").get<std::string>(),
                                          j.at("updateDateTime").get<std::chrono::system_clock::time_point>(),
                                          j.at("quantity").get<int>(),
                                          j.at("status").get<App::Item::Transaction::Status>());
        }
    };

    template <>
    struct adl_serializer<App::Item::Specification>
    {
        static void to_json(json &j, const App::Item::Specification &s)
        {
            j = {
                {"Id", s.Id},
                {"name", s.name},
                {"description", s.description},
                {"additionalPrice", s.additionalPrice},
            };
        }

        static App::Item::Specification from_json(const json &j)
        {
            return App::Item::Specification(j.at("Id").get<std::string>(),
                                            j.at("name").get<std::string>(),
                                            j.at("description").get<std::string>(),
                                            j.at("additionalPrice").get<double>());
        }
    };

    template <>
    struct adl_serializer<App::Item>
    {
        static void to_json(json &j, const App::Item &i)
        {
            j = {
                {"Id", i.Id},
                {"name", i.name},
                {"description", i.description},
                {"price", i.price},
                {"stockQuantity", i.stockQuantity},
                {"category", i.category},
                {"vendorID", i.vendorID},
                {"status", i.status},
                {"specifications", i.specifications},
            };
        }

        static App::Item from_json(const json &j)
        {
            return App::Item(j.at("Id").get<std::string>(),
                             j.at("name").get<std::string>(),
                             j.at("description").get<std::string>(),
                             j.at("price").get<double>(),
                             j.at("stockQuantity").get<int>(),
                             j.at("category").get<App::Category>(),
                             j.at("vendorID").get<std::string>(),
                             j.at("status").get<App::Item::Status>(),
                             j.at("specifications").get<std::vector<std::string>>());
        }
    };

    template <>
    struct adl_serializer<App::Report>
    {
        static void to_json(json &j, const App::Report &r)
        {
            j = {
                {"Id", r.Id},
                {"type", r.type},
                {"generatedDateTime", r.generatedDateTime},
                {"content", r.content},
                {"IdOfGeneratedBy", r.IdOfGeneratedBy},
            };
        }

        static App::Report from_json(const json &j)
        {
            return App::Report(j.at("Id").get<std::string>(),
                               j.at("type").get<App::Report::Type>(),
                               j.at("generatedDateTime").get<std::chrono::system_clock::time_point>(),
                               j.at("content").get<std::string>(),
                               j.at("IdOfGeneratedBy").get<std::string>());
        }
    };

    template <>
    struct adl_serializer<App::MarketingCampaign>
    {
        static void to_json(json &j, const App::MarketingCampaign &mc)
        {
            j = {
                {"Id", mc.Id},
                {"name", mc.name},
                {"description", mc.description},
                {"startDateTime", mc.startDateTime},
                {"endDateTime", mc.endDateTime},
                {"isPercentageBased", mc.isPercentageBased},
                {"discountValue", mc.discountValue},
                {"status", mc.status} 
            };
        }

        static App::MarketingCampaign from_json(const json &j)
        {
            return App::MarketingCampaign(
                j.at("Id").get<std::string>(),
                j.at("name").get<std::string>(),
                j.at("description").get<std::string>(),
                j.at("startDateTime").get<std::chrono::system_clock::time_point>(),
                j.at("endDateTime").get<std::chrono::system_clock::time_point>(),
                j.at("isPercentageBased").get<bool>(),
                j.at("discountValue").get<double>(),
                j.at("status").get<App::MarketingCampaign::Status>());
        }
    };

    template <>
    struct adl_serializer<App::OrderList>
    {
        static void to_json(json &j, const App::OrderList &ol)
        {
            j = {
                {"Id", ol.Id},
                {"customerID", ol.customerID},
                {"orderIDs", ol.orderIDs},
                {"orderDateTime", ol.orderDateTime},
                {"totalAmount", ol.totalAmount},
                {"status", ol.status}};
        }

        static App::OrderList from_json(const json &j)
        {
            return App::OrderList(
                j.at("Id").get<std::string>(),
                j.at("customerID").get<std::string>(),
                j.at("orderIDs").get<std::vector<std::string>>(),
                j.at("orderDateTime").get<std::chrono::system_clock::time_point>(),
                j.at("totalAmount").get<double>(),
                j.at("status").get<App::OrderList::Status>());
        }
    };

    template <>
    struct adl_serializer<App::Order>
    {
        static void to_json(json &j, const App::Order &o)
        {
            j = {
                {"Id", o.Id},
                {"quantity", o.quantity},
                {"itemID", o.itemID},
                {"status", o.status}};
        }

        static App::Order from_json(const json &j)
        {
            return App::Order(
                j.at("Id").get<std::string>(),
                j.at("quantity").get<int>(),
                j.at("itemID").get<std::string>(),
                j.at("status").get<App::Order::Status>());
        }
    };

    template <>
    struct adl_serializer<App::Delivery>
    {
        static void to_json(json &j, const App::Delivery &d)
        {
            j = {
                {"Id", d.Id},
                {"address", d.address}, 
                {"status", d.status},
                {"scheduledDateTime", d.scheduledDateTime},
                {"actualDateTime", d.actualDateTime},
                {"deliveryFee", d.deliveryFee},
                {"orderIDs", d.orderIDs}};
        }

        static App::Delivery from_json(const json &j)
        {
            return App::Delivery(
                j.at("Id").get<std::string>(),
                j.at("address").get<App::Address>(), 
                j.at("status").get<App::Delivery::Status>(),
                j.at("scheduledDateTime").get<std::chrono::system_clock::time_point>(),
                j.at("actualDateTime").get<std::chrono::system_clock::time_point>(),
                j.at("deliveryFee").get<double>(),
                j.at("orderIDs").get<std::vector<std::string>>());
        }
    };

    template <>
    struct adl_serializer<App::Identity>
    {
        static void to_json(json &j, const App::Identity &i)
        {
            j = {
                {"Id", i.Id}, {"pwd", i.pwd}, {"name", i.name}, {"email", i.email}, {"phoneNumber", i.phoneNumber}};
        }

        static App::Identity from_json(const json &j)
        {
            return App::Identity(
                j.at("Id").get<std::string>(),
                j.at("pwd").get<std::string>(),
                j.at("name").get<std::string>(),
                j.at("email").get<std::string>(),
                j.at("phoneNumber").get<std::string>());
        }
    };

    template <>
    struct adl_serializer<App::Customer>
    {
        static void to_json(json &j, const App::Customer &c)
        {
            j = static_cast<const App::Identity &>(c);
            j["addresses"] = c.addresses;
        }

        static App::Customer from_json(const json &j)
        {
            auto base = j.get<App::Identity>();
            auto addresses = j.at("addresses").get<std::vector<App::Address>>();
            return App::Customer(
                base.Id, base.pwd, base.name, base.email, base.phoneNumber, addresses);
        }
    };

    template <>
    struct adl_serializer<App::Vendor>
    {
        static void to_json(json &j, const App::Vendor &v)
        {
            j = static_cast<const App::Identity &>(v);
        }
        static App::Vendor from_json(const json &j)
        {
            auto base = j.get<App::Identity>();
            return App::Vendor(base.Id, base.pwd, base.name, base.email, base.phoneNumber);
        }
    };

    template <>
    struct adl_serializer<App::Staff>
    {
        static void to_json(json &j, const App::Staff &s)
        {
            j = static_cast<const App::Identity &>(s);
        }
        static App::Staff from_json(const json &j)
        {
            auto base = j.get<App::Identity>();
            return App::Staff(base.Id, base.pwd, base.name, base.email, base.phoneNumber);
        }
    };

    template <>
    struct adl_serializer<App::Event>
    {
        static void to_json(json &j, const App::Event &e)
        {
            j = {
                {"Id", e.Id}, {"name", e.name}, {"description", e.description}, {"organizerName", e.organizerName}, {"staffId", e.staffId}, {"maxStall", e.maxStall}, {"status", e.status}, {"startDateTime", e.startDateTime}, {"endDateTime", e.endDateTime}};
        }
        static App::Event from_json(const json &j)
        {
            return App::Event(
                j.at("Id").get<std::string>(), j.at("name").get<std::string>(),
                j.at("description").get<std::string>(), j.at("organizerName").get<std::vector<std::string>>(),
                j.at("staffId").get<std::vector<std::string>>(), j.at("maxStall").get<int>(),
                j.at("status").get<App::Event::Status>(),
                j.at("startDateTime").get<std::chrono::system_clock::time_point>(),
                j.at("endDateTime").get<std::chrono::system_clock::time_point>());
        }
    };

    template <>
    struct adl_serializer<App::MonitoringRecord>
    {
        static void to_json(json &j, const App::MonitoringRecord &mr)
        {
            j = {
                {"Id", mr.Id}, {"startTime", mr.startTime}, {"endTime", mr.endTime}, {"observation", mr.observation}};

            json identities_array = json::array();
            for (const auto &identity_ptr : mr.involvedIdentities)
            {
                json identity_obj;

                if (auto *customer = dynamic_cast<App::Customer *>(identity_ptr.get()))
                {
                    identity_obj["type"] = "Customer";
                    identity_obj["data"] = *customer;
                }
                else if (auto *vendor = dynamic_cast<App::Vendor *>(identity_ptr.get()))
                {
                    identity_obj["type"] = "Vendor";
                    identity_obj["data"] = *vendor;
                }
                else if (auto *staff = dynamic_cast<App::Staff *>(identity_ptr.get()))
                {
                    identity_obj["type"] = "Staff";
                    identity_obj["data"] = *staff;
                }
                if (!identity_obj.empty())
                    identities_array.push_back(identity_obj);
            }
            j["involvedIdentities"] = identities_array;
        }

        static App::MonitoringRecord from_json(const json &j)
        {
            std::vector<std::unique_ptr<App::Identity>> identities;
            const json &identities_array = j.at("involvedIdentities");
            for (const auto &identity_obj : identities_array)
            {
                std::string type = identity_obj.at("type").get<std::string>();
                const json &data = identity_obj.at("data");

                if (type == "Customer")
                {
                    identities.push_back(std::make_unique<App::Customer>(data.get<App::Customer>()));
                }
                else if (type == "Vendor")
                {
                    identities.push_back(std::make_unique<App::Vendor>(data.get<App::Vendor>()));
                }
                else if (type == "Staff")
                {
                    identities.push_back(std::make_unique<App::Staff>(data.get<App::Staff>()));
                }
            }

            return App::MonitoringRecord(
                j.at("Id").get<std::string>(),
                std::move(identities),
                j.at("startTime").get<std::chrono::system_clock::time_point>(),
                j.at("endTime").get<std::chrono::system_clock::time_point>(),
                j.at("observation").get<std::string>());
        }
    };

}