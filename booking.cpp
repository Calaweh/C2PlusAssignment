#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>  
#include <iomanip>    
#include <sstream>
#include <chrono>     

using namespace std;

// Enum for user roles
enum class Role { Staff, Vendor };

// Enum for booking status
enum class BookingStatus { Pending, Confirmed, Cancelled };

// Stall class to represent individual stalls
class Stall {
public:
    int row, col;
    string stallId;
    bool isOccupied;
    string vendorId;  
    string vendorName;

    Stall(int r, int c) : row(r), col(c), isOccupied(false), vendorId(""), vendorName("") {
        stallId = char('A' + r) + to_string(c + 1);
    }

    void occupy(const string& vId, const string& vName) {
        isOccupied = true;
        vendorId = vId;
        vendorName = vName;
    }

    void release() {
        isOccupied = false;
        vendorId = "";
        vendorName = "";
    }
};

// User class to represent Staff and Vendor
class User {
public:
    string id;  
    string username;
    Role role;

    User(string id, string username, Role role) : id(id), username(username), role(role) {}
};

// Booking class to represent a booking with stall information
class Booking {
public:
    string bookingId;  
    string vendorId;
    string stallId;
    BookingStatus status;
    double bookingFee;

    Booking(string bookingId, string vendorId, string stallId, BookingStatus status, double fee)
        : bookingId(bookingId), vendorId(vendorId), stallId(stallId), status(status), bookingFee(fee) {
    }

    void display() const {
        string statusStr = (status == BookingStatus::Pending) ? "Pending" :
            (status == BookingStatus::Confirmed) ? "Confirmed" : "Cancelled";
        cout << "Booking ID: " << bookingId << ", Vendor ID: " << vendorId
            << ", Stall: " << stallId << ", Fee: RM" << fixed << setprecision(2) << bookingFee
            << ", Status: " << statusStr << endl;
    }
};

// Market class to manage the 2D stall array
class Market {
private:
    static const int ROWS = 5;
    static const int COLS = 4;
    Stall* stalls[ROWS][COLS];

public:
    Market() {
        // Initialize all stalls
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                stalls[i][j] = new Stall(i, j);
            }
        }

        // Pre-occupy some stalls for demo
        stalls[0][1]->occupy("V9999", "Demo Vendor 1");
        stalls[1][2]->occupy("V9998", "Demo Vendor 2");
        stalls[2][0]->occupy("V9997", "Demo Vendor 3");
    }

    ~Market() {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                delete stalls[i][j];
            }
        }
    }

    void displayMarket() {
        cout << "\n========== FOOD MARKET LAYOUT ==========\n";
        cout << "   ";
        for (int j = 0; j < COLS; j++) {
            cout << setw(4) << j + 1;
        }
        cout << "\n";

        for (int i = 0; i < ROWS; i++) {
            cout << char('A' + i) << "  ";
            for (int j = 0; j < COLS; j++) {
                if (stalls[i][j]->isOccupied) {
                    cout << " \033[31m[X]\033[0m ";
                }
                else {
                    cout << " \033[32m[ ]\033[0m ";
                }
            }
            cout << "\n";
        }
        
        cout << "\nLegend: \033[32m[ ]\033[0m = Available, \033[31m[X]\033[0m = Occupied\n";
        displayStats();
    }

    void displayStats() {
        int available = 0, occupied = 0;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (stalls[i][j]->isOccupied) occupied++;
                else available++;
            }
        }

        cout << "\n--- Market Statistics ---\n";
        cout << "Available stalls: " << available << "\n";
        cout << "Occupied stalls: " << occupied << "\n";
        cout << "Total stalls: " << ROWS * COLS << "\n";
    }

    void displayDetailedStalls() {
        cout << "\n========== DETAILED STALL INFORMATION ==========\n";
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                cout << "Stall " << stalls[i][j]->stallId << ": ";
                if (stalls[i][j]->isOccupied) {
                    cout << "OCCUPIED by " << stalls[i][j]->vendorName
                        << " (ID: " << stalls[i][j]->vendorId << ")\n";
                }
                else {
                    cout << "AVAILABLE\n";
                }
            }
        }
    }

    bool isValidPosition(const string& stallId, int& row, int& col) {
        if (stallId.length() < 2) return false;

        char rowChar = toupper(stallId[0]);
        int colNum = stoi(stallId.substr(1));

        row = rowChar - 'A';
        col = colNum - 1;

        return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
    }

    bool isStallAvailable(const string& stallId) {
        int row, col;
        if (!isValidPosition(stallId, row, col)) return false;
        return !stalls[row][col]->isOccupied;
    }

    bool bookStall(const string& stallId, const string& vendorId, const string& vendorName) {
        int row, col;
        if (!isValidPosition(stallId, row, col)) {
            cout << "Invalid stall position: " << stallId << endl;
            return false;
        }

        if (stalls[row][col]->isOccupied) {
            cout << "Stall " << stallId << " is already occupied!" << endl;
            return false;
        }

        stalls[row][col]->occupy(vendorId, vendorName);
        return true;
    }

    bool releaseStall(const string& stallId) {
        int row, col;
        if (!isValidPosition(stallId, row, col)) return false;

        stalls[row][col]->release();
        return true;
    }

    string getStallInfo(const string& stallId) {
        int row, col;
        if (!isValidPosition(stallId, row, col)) return "Invalid stall";

        if (stalls[row][col]->isOccupied) {
            return "Occupied by " + stalls[row][col]->vendorName;
        }
        return "Available";
    }

    vector<string> getAvailableStalls() {
        vector<string> available;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (!stalls[i][j]->isOccupied) {
                    available.push_back(stalls[i][j]->stallId);
                }
            }
        }
        return available;
    }
};

// BookingSystem class to manage users and bookings with stall integration
class BookingSystem {
private:
    vector<User> users;
    vector<Booking> bookings;
    Market market;
    int nextStaffId = 1;
    int nextVendorId = 1;
    int nextBookingId = 1;

    // Generate formatted user ID
    string generateUserId(Role role) {
        if (role == Role::Staff) {
            return "S" + string(4 - to_string(nextStaffId).length(), '0') + to_string(nextStaffId++);
        }
        else {
            return "V" + string(4 - to_string(nextVendorId).length(), '0') + to_string(nextVendorId++);
        }
    }

    // Generate formatted booking ID
    string generateBookingId() {
        return "B" + string(4 - to_string(nextBookingId).length(), '0') + to_string(nextBookingId++);
    }

    // Extract role from user ID
    Role getRoleFromId(const string& userId) {
        if (!userId.empty() && userId[0] == 'S') {
            return Role::Staff;
        }
        else if (!userId.empty() && userId[0] == 'V') {
            return Role::Vendor;
        }
        return Role::Vendor; // Default fallback
    }

    // Calculate booking fee based on stall position
    double calculateBookingFee(const string& stallId) {
        double baseFee = 50.0;
        double rowPremium = 0.0;
        double cornerPremium = 0.0;
        
        if (stallId.length() >= 2) {
            char row = toupper(stallId[0]);
            int col = stoi(stallId.substr(1));
            
            // Row premiums
            switch (row) {
                case 'A': rowPremium = 30.0; break;
                case 'B': rowPremium = 20.0; break;
                case 'C': rowPremium = 10.0; break;
                case 'D': rowPremium = 5.0; break;
                case 'E': rowPremium = 0.0; break;
            }
            
            // Corner premium for columns 1 and 4
            if (col == 1 || col == 4) {
                cornerPremium = 15.0;
            }
        }
        
        return baseFee + rowPremium + cornerPremium;
    }

    // Generate invoice
    //void generateInvoice(const Booking& booking, const User& vendor) {
    //    cout << "\n========== BOOKING INVOICE ==========\n";
    //    
    //    // Get current date and time using a simple approach
    //    auto now = chrono::system_clock::now();
    //    auto time_t = chrono::system_clock::to_time_t(now);
    //    
    //    // Format date manually to avoid deprecation warnings
    //    struct tm timeinfo;
    //    #ifdef _WIN32
    //        localtime_s(&timeinfo, &time_t);
    //    #else
    //        timeinfo = *localtime(&time_t);
    //    #endif
    //    
    //    cout << "Date: " << (timeinfo.tm_year + 1900) << "-"
    //         << setfill('0') << setw(2) << (timeinfo.tm_mon + 1) << "-"
    //         << setfill('0') << setw(2) << timeinfo.tm_mday << " "
    //         << setfill('0') << setw(2) << timeinfo.tm_hour << ":"
    //         << setfill('0') << setw(2) << timeinfo.tm_min << ":"
    //         << setfill('0') << setw(2) << timeinfo.tm_sec << "\n";
    //         
    //    cout << "Invoice #: INV-" << booking.bookingId << "\n";
    //    cout << "----------------------------------------\n";
    //    cout << "Vendor: " << vendor.username << " (" << vendor.id << ")\n";
    //    cout << "Stall: " << booking.stallId << "\n";
    //    cout << "----------------------------------------\n";
    //    cout << "Base Fee: RM" << fixed << setprecision(2) << booking.bookingFee << "\n";
    //    double tax = booking.bookingFee * 0.06;
    //    double total = booking.bookingFee + tax;
    //    cout << "Service Tax (6%): RM" << tax << "\n";
    //    cout << "----------------------------------------\n";
    //    cout << "TOTAL AMOUNT: RM" << total << "\n";
    //    cout << "Status: " << (booking.status == BookingStatus::Pending ? "PENDING" : 
    //                          booking.status == BookingStatus::Confirmed ? "CONFIRMED" : "CANCELLED") << "\n";
    //    cout << "=====================================\n";
    //}

public:
    void addUser(string username, Role role) {
        string userId = generateUserId(role);
        users.push_back(User(userId, username, role));
        cout << "User created: " << userId << " - " << username << endl;
    }

    bool hasPermission(const User& user, const string& action, const Booking* booking = nullptr) const {
        if (user.role == Role::Staff) {
            return true;
        }
        else if (user.role == Role::Vendor) {
            if (action == "create") {
                return true;
            }
            if (action == "read" && booking && booking->vendorId == user.id) {
                return true;
            }
        }
        return false;
    }

    void createBooking(const User& user, const string& vendorId, const string& stallId) {
        if (!hasPermission(user, "create")) {
            cout << "Permission denied: User cannot create bookings." << endl;
            return;
        }

        if (!market.isStallAvailable(stallId)) {
            cout << "Stall " << stallId << " is not available!" << endl;
            return;
        }

        User* vendor = getUserById(vendorId);
        if (!vendor) {
            cout << "Vendor not found!" << endl;
            return;
        }

        // Calculate booking fee
        double bookingFee = calculateBookingFee(stallId);

        if (market.bookStall(stallId, vendorId, vendor->username)) {

            // Get current date and time using a simple approach
            auto now = chrono::system_clock::now();
            auto time_t = chrono::system_clock::to_time_t(now);

            // Format date manually to avoid deprecation warnings
            struct tm timeinfo;
            #ifdef _WIN32
            localtime_s(&timeinfo, &time_t);
            #else
            timeinfo = *localtime(&time_t);
            #endif

            cout << "Date: " << (timeinfo.tm_year + 1900) << "-"
                << setfill('0') << setw(2) << (timeinfo.tm_mon + 1) << "-"
                << setfill('0') << setw(2) << timeinfo.tm_mday << " "
                << setfill('0') << setw(2) << timeinfo.tm_hour << ":"
                << setfill('0') << setw(2) << timeinfo.tm_min << ":"
                << setfill('0') << setw(2) << timeinfo.tm_sec << "\n";

            string bookingId = generateBookingId();
            Booking newBooking(bookingId, vendorId, stallId, BookingStatus::Pending, bookingFee);
            bookings.push_back(newBooking);

            cout << "Booking created successfully!" << endl;
            cout << "Booking ID: " << bookingId << "\nStall: " << stallId << "\nDate: " << (timeinfo.tm_year + 1900) << "-"
                << setfill('0') << setw(2) << (timeinfo.tm_mon + 1) << "-"
                << setfill('0') << setw(2) << timeinfo.tm_mday << " "
                << setfill('0') << setw(2) << timeinfo.tm_hour << ":"
                << setfill('0') << setw(2) << timeinfo.tm_min << ":"
                << setfill('0') << setw(2) << timeinfo.tm_sec << "\n" << endl;

            // Generate and display invoice
            //generateInvoice(newBooking, *vendor);
        }
    }

    void viewBookings(const User& user) {
        bool hasBookings = false;
        cout << "\n========== BOOKINGS ==========\n";
        for (const auto& booking : bookings) {
            if (user.role == Role::Staff || (user.role == Role::Vendor && booking.vendorId == user.id)) {
                booking.display();
                hasBookings = true;
            }
        }
        if (!hasBookings) {
            cout << "No bookings found for this user." << endl;
        }
    }

    void updateBooking(const User& user, const string& bookingId, BookingStatus newStatus) {
        if (user.role != Role::Staff) {
            cout << "Permission denied: Only staff can update bookings." << endl;
            return;
        }

        for (auto& booking : bookings) {
            if (booking.bookingId == bookingId) {
                booking.status = newStatus;
                cout << "Booking ID " << bookingId << " updated to ";
                cout << ((newStatus == BookingStatus::Confirmed) ? "Confirmed" :
                    (newStatus == BookingStatus::Cancelled) ? "Cancelled" : "Pending") << endl;
                return;
            }
        }
        cout << "Booking ID " << bookingId << " not found." << endl;
    }

    void deleteBooking(const User& user, const string& bookingId) {
        if (user.role != Role::Staff) {
            cout << "Permission denied: Only staff can delete bookings." << endl;
            return;
        }

        for (auto it = bookings.begin(); it != bookings.end(); ++it) {
            if (it->bookingId == bookingId) {
                // Release the stall
                market.releaseStall(it->stallId);
                bookings.erase(it);
                cout << "Booking ID " << bookingId << " deleted and stall released." << endl;
                return;
            }
        }
        cout << "Booking ID " << bookingId << " not found." << endl;
    }

    User* getUserById(const string& id) {
        for (auto& user : users) {
            if (user.id == id) {
                return &user;
            }
        }
        return nullptr;
    }

    void displayUsers(Role role) {
        cout << (role == Role::Staff ? "Staff" : "Vendor") << " users:" << endl;
        for (const auto& user : users) {
            if (user.role == role) {
                cout << "ID: " << user.id << ", Username: " << user.username << endl;
            }
        }
    }

    void showAvailableStalls() {
        vector<string> available = market.getAvailableStalls();
        cout << "\n--- Available Stalls with Pricing ---\n";
        if (available.empty()) {
            cout << "No stalls available!" << endl;
        }
        else {
            for (const string& stallId : available) {
                double fee = calculateBookingFee(stallId);
                double total = fee * 1.06; // Including 6% tax
                cout << "Stall " << stallId << ": RM" << fixed << setprecision(2)
                    << fee << " (Total: RM" << total << " with tax)" << endl;
            }
        }
    }

    void showMarketLayout() {
        market.displayMarket();
    }

    void showDetailedStallInfo() {
        market.displayDetailedStalls();
    }

    void showMainMenu() {
        while (true) {
            cout << "\n=== FOOD STALL BOOKING SYSTEM ===" << endl;
            cout << "1. Login (Enter User ID)" << endl;
            cout << "2. View Market Layout" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 3) {
                cout << "Thank you for using the Food Stall Booking System!" << endl;
                break;
            }

            switch (choice) {
            case 1: {
                cout << "\nEnter User ID (e.g., S0001 for Staff, V0001 for Vendor): ";
                string userId;
                getline(cin, userId);

                // Convert to uppercase for consistency
                transform(userId.begin(), userId.end(), userId.begin(), ::toupper);

                User* user = getUserById(userId);
                if (user) {
                    cout << "Welcome, " << user->username << "!" << endl;
                    if (user->role == Role::Staff) {
                        showStaffMenu(*user);
                    }
                    else {
                        showVendorMenu(*user);
                    }
                }
                else {
                    cout << "Invalid User ID. Please check and try again." << endl;
                    cout << "\nAvailable Users:" << endl;
                    displayUsers(Role::Staff);
                    displayUsers(Role::Vendor);
                }
                break;
            }
            case 2:
                showMarketLayout();
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
            }
        }
    }

    void showStaffMenu(const User& user) {
        while (true) {
            cout << "\n=== Staff Menu (User: " << user.username << ") ===" << endl;
            cout << "1. Create Booking" << endl;
            cout << "2. View All Bookings" << endl;
            cout << "3. Update Booking Status" << endl;
            cout << "4. Delete Booking" << endl;
            cout << "5. View Market Layout" << endl;
            cout << "6. View Detailed Stall Info" << endl;
            cout << "7. View Pricing Guide" << endl;
            cout << "8. Back to Main Menu" << endl;
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 8) break;

            switch (choice) {
            case 1: {
                displayUsers(Role::Vendor);
                cout << "Enter Vendor ID: ";
                string vendorId;
                getline(cin, vendorId);
                transform(vendorId.begin(), vendorId.end(), vendorId.begin(), ::toupper);

                showMarketLayout();
                showAvailableStalls();
                cout << "Enter Stall ID (e.g., A1, B3): ";
                string stallId;
                getline(cin, stallId);
                transform(stallId.begin(), stallId.end(), stallId.begin(), ::toupper);

                createBooking(user, vendorId, stallId);
                break;
            }
            case 2:
                viewBookings(user);
                break;
            case 3: {
                cout << "Enter Booking ID (e.g., B0001): ";
                string bookingId;
                getline(cin, bookingId);
                transform(bookingId.begin(), bookingId.end(), bookingId.begin(), ::toupper);
                cout << "Enter new status (1: Pending, 2: Confirmed, 3: Cancelled): ";
                int statusChoice;
                cin >> statusChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                BookingStatus status = (statusChoice == 2) ? BookingStatus::Confirmed :
                    (statusChoice == 3) ? BookingStatus::Cancelled : BookingStatus::Pending;
                updateBooking(user, bookingId, status);
                break;
            }
            case 4: {
                cout << "Enter Booking ID (e.g., B0001): ";
                string bookingId;
                getline(cin, bookingId);
                transform(bookingId.begin(), bookingId.end(), bookingId.begin(), ::toupper);
                deleteBooking(user, bookingId);
                break;
            }
            case 5:
                showMarketLayout();
                break;
            case 6:
                showDetailedStallInfo();
                break;
            case 7:
                showPricingGuide();
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
            }
        }
    }

    void showVendorMenu(const User& user) {
        while (true) {
            cout << "\n=== Vendor Menu (User: " << user.username << ") ===" << endl;
            cout << "1. Create Booking" << endl;
            cout << "2. View My Bookings" << endl;
            cout << "3. View Market Layout" << endl;
            cout << "4. View Available Stalls & Pricing" << endl;
            cout << "5. View Pricing Guide" << endl;
            cout << "6. Back to Main Menu" << endl;
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 6) break;

            switch (choice) {
            case 1: {
                showMarketLayout();
                showAvailableStalls();
                cout << "Enter Stall ID (e.g., A1, B3): ";
                string stallId;
                getline(cin, stallId);
                transform(stallId.begin(), stallId.end(), stallId.begin(), ::toupper);

                createBooking(user, user.id, stallId);
                break;
            }
            case 2:
                viewBookings(user);
                break;
            case 3:
                showMarketLayout();
                break;
            case 4:
                showAvailableStalls();
                break;
            case 5:
                showPricingGuide();
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
            }
        }
    }

    void showPricingGuide() {
        cout << "\n========== STALL PRICING GUIDE ==========\n";
        cout << "Base Fee: RM50.00\n";
        cout << "\nRow Premiums:\n";
        cout << "Row A (Front): +RM30.00 (Total: RM80.00)\n";
        cout << "Row B:         +RM20.00 (Total: RM70.00)\n";
        cout << "Row C:         +RM10.00 (Total: RM60.00)\n";
        cout << "Row D:         +RM5.00  (Total: RM55.00)\n";
        cout << "Row E (Back):  +RM0.00  (Total: RM50.00)\n";
        cout << "\nCorner Premium: +RM15.00 (Columns 1 & 4)\n";
        cout << "Service Tax: 6%\n";
        cout << "\nExample: Stall A1 = RM80 + RM15 + 6% tax = RM100.70\n";
        cout << "========================================\n";
    }
};

int main() {
    BookingSystem system;

    // Add sample users
    system.addUser("Alice (Staff Manager)", Role::Staff);
    system.addUser("Bob's Nasi Lemak", Role::Vendor);
    system.addUser("Siti's Char Kway Teow", Role::Vendor);
    system.addUser("Wong's Dim Sum", Role::Vendor);

    cout << "=== WELCOME TO NIGHT MARKET STALL BOOKING SYSTEM ===\n";
    cout << "Manage food stall bookings with interactive market layout!\n";
   

    system.showMainMenu();

    return 0;
}