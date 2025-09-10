#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <map>
#include <limits>

using namespace std;

// Transaction class to represent individual financial transactions
class Transaction {
public:
    enum Type { INCOME, EXPENSE };

private:
    int id;
    Type type;
    string category;
    double amount;
    string description;
    string date;

public:
    Transaction(int id, Type t, const string& cat, double amt, const string& desc, const string& dt)
        : id(id), type(t), category(cat), amount(amt), description(desc), date(dt) {}

    // Getters
    int getId() const { return id; }
    Type getType() const { return type; }
    string getCategory() const { return category; }
    double getAmount() const { return amount; }
    string getDescription() const { return description; }
    string getDate() const { return date; }

    // Convert to string for file storage
    string toString() const {
        return to_string(id) + "|" +
               to_string(type) + "|" +
               category + "|" +
               to_string(amount) + "|" +
               description + "|" +
               date;
    }

    // Display transaction
    void display() const {
        cout << left << setw(5) << id
             << setw(10) << (type == INCOME ? "Income" : "Expense")
             << setw(15) << category
             << setw(12) << fixed << setprecision(2) << amount
             << setw(30) << description
             << setw(12) << date << endl;
    }
};

// Finance Manager class to handle all operations
class FinanceManager {
private:
    vector<Transaction> transactions;
    int nextId;
    string dataFile;

    // Predefined categories
    vector<string> incomeCategories = {"Salary", "Freelance", "Investment", "Gift", "Other Income"};
    vector<string> expenseCategories = {"Food", "Transport", "Housing", "Entertainment", "Healthcare",
                                        "Education", "Shopping", "Utilities", "Other Expense"};

public:
    FinanceManager() : nextId(1), dataFile("finance_data.txt") {
        loadData();
    }

    // Add new transaction
    void addTransaction() {
        cout << "\n=== Add New Transaction ===" << endl;

        // Select transaction type
        cout << "Transaction Type:" << endl;
        cout << "1. Income" << endl;
        cout << "2. Expense" << endl;
        cout << "Enter choice (1-2): ";

        int typeChoice;
        cin >> typeChoice;
        cin.ignore();

        if (typeChoice < 1 || typeChoice > 2) {
            cout << "Invalid choice!" << endl;
            return;
        }

        Transaction::Type type = (typeChoice == 1) ? Transaction::INCOME : Transaction::EXPENSE;

        // Select category
        string category = selectCategory(type);

        // Get amount
        cout << "Enter amount: $";
        double amount;
        cin >> amount;
        cin.ignore();

        if (amount <= 0) {
            cout << "Amount must be positive!" << endl;
            return;
        }

        // Get description
        cout << "Enter description: ";
        string description;
        getline(cin, description);

        // Get date
        string date = getCurrentDate();
        cout << "Use current date (" << date << ")? (y/n): ";
        char useCurrentDate;
        cin >> useCurrentDate;
        cin.ignore();

        if (useCurrentDate == 'n' || useCurrentDate == 'N') {
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
        }

        // Create and add transaction
        Transaction newTransaction(nextId++, type, category, amount, description, date);
        transactions.push_back(newTransaction);

        cout << "\nTransaction added successfully!" << endl;
        saveData();
    }

    // View all transactions
    void viewTransactions() {
        if (transactions.empty()) {
            cout << "\nNo transactions found!" << endl;
            return;
        }

        cout << "\n=== All Transactions ===" << endl;
        cout << string(100, '-') << endl;
        cout << left << setw(5) << "ID"
             << setw(10) << "Type"
             << setw(15) << "Category"
             << setw(12) << "Amount ($)"
             << setw(30) << "Description"
             << setw(12) << "Date" << endl;
        cout << string(100, '-') << endl;

        for (const auto& t : transactions) {
            t.display();
        }
        cout << string(100, '-') << endl;
    }

    // View financial summary
    void viewSummary() {
        if (transactions.empty()) {
            cout << "\nNo transactions to summarize!" << endl;
            return;
        }

        double totalIncome = 0;
        double totalExpense = 0;
        map<string, double> categoryTotals;

        for (const auto& t : transactions) {
            if (t.getType() == Transaction::INCOME) {
                totalIncome += t.getAmount();
            } else {
                totalExpense += t.getAmount();
            }
            categoryTotals[t.getCategory()] += t.getAmount();
        }

        double balance = totalIncome - totalExpense;

        cout << "\n=== Financial Summary ===" << endl;
        cout << string(50, '=') << endl;
        cout << fixed << setprecision(2);
        cout << "Total Income:    $" << setw(12) << totalIncome << endl;
        cout << "Total Expenses:  $" << setw(12) << totalExpense << endl;
        cout << string(50, '-') << endl;
        cout << "Current Balance: $" << setw(12) << balance << endl;
        cout << string(50, '=') << endl;

        // Category breakdown
        cout << "\n=== Spending by Category ===" << endl;
        cout << string(50, '-') << endl;
        for (const auto& pair : categoryTotals) {
            cout << left << setw(20) << pair.first
                 << "$" << setw(12) << right << pair.second << endl;
        }
        cout << string(50, '-') << endl;
    }

    // Generate monthly report
    void monthlyReport() {
        if (transactions.empty()) {
            cout << "\nNo transactions for report!" << endl;
            return;
        }

        cout << "\nEnter month (1-12): ";
        int month;
        cin >> month;

        cout << "Enter year (YYYY): ";
        int year;
        cin >> year;
        cin.ignore();

        double monthlyIncome = 0;
        double monthlyExpense = 0;
        vector<Transaction> monthTransactions;

        for (const auto& t : transactions) {
            // Extract month and year from date string (YYYY-MM-DD)
            int tYear, tMonth;
            sscanf(t.getDate().c_str(), "%d-%d", &tYear, &tMonth);

            if (tYear == year && tMonth == month) {
                monthTransactions.push_back(t);
                if (t.getType() == Transaction::INCOME) {
                    monthlyIncome += t.getAmount();
                } else {
                    monthlyExpense += t.getAmount();
                }
            }
        }

        if (monthTransactions.empty()) {
            cout << "\nNo transactions found for " << month << "/" << year << endl;
            return;
        }

        cout << "\n=== Monthly Report for " << month << "/" << year << " ===" << endl;
        cout << string(50, '=') << endl;
        cout << fixed << setprecision(2);
        cout << "Monthly Income:    $" << setw(12) << monthlyIncome << endl;
        cout << "Monthly Expenses:  $" << setw(12) << monthlyExpense << endl;
        cout << "Monthly Balance:   $" << setw(12) << (monthlyIncome - monthlyExpense) << endl;
        cout << string(50, '=') << endl;

        cout << "\nTransactions for this month:" << endl;
        cout << string(100, '-') << endl;
        for (const auto& t : monthTransactions) {
            t.display();
        }
        cout << string(100, '-') << endl;
    }

    // Search transactions
    void searchTransactions() {
        cout << "\n=== Search Transactions ===" << endl;
        cout << "1. Search by category" << endl;
        cout << "2. Search by description" << endl;
        cout << "3. Search by amount range" << endl;
        cout << "Enter choice (1-3): ";

        int choice;
        cin >> choice;
        cin.ignore();

        vector<Transaction> results;

        switch (choice) {
            case 1: {
                cout << "Enter category to search: ";
                string category;
                getline(cin, category);

                for (const auto& t : transactions) {
                    if (t.getCategory().find(category) != string::npos) {
                        results.push_back(t);
                    }
                }
                break;
            }
            case 2: {
                cout << "Enter description keyword: ";
                string keyword;
                getline(cin, keyword);

                for (const auto& t : transactions) {
                    if (t.getDescription().find(keyword) != string::npos) {
                        results.push_back(t);
                    }
                }
                break;
            }
            case 3: {
                cout << "Enter minimum amount: $";
                double minAmt;
                cin >> minAmt;

                cout << "Enter maximum amount: $";
                double maxAmt;
                cin >> maxAmt;

                for (const auto& t : transactions) {
                    if (t.getAmount() >= minAmt && t.getAmount() <= maxAmt) {
                        results.push_back(t);
                    }
                }
                break;
            }
            default:
                cout << "Invalid choice!" << endl;
                return;
        }

        if (results.empty()) {
            cout << "\nNo transactions found matching your search criteria." << endl;
        } else {
            cout << "\n=== Search Results (" << results.size() << " found) ===" << endl;
            cout << string(100, '-') << endl;
            for (const auto& t : results) {
                t.display();
            }
            cout << string(100, '-') << endl;
        }
    }

    // Delete transaction
    void deleteTransaction() {
        viewTransactions();

        if (transactions.empty()) return;

        cout << "\nEnter transaction ID to delete: ";
        int id;
        cin >> id;
        cin.ignore();

        auto it = find_if(transactions.begin(), transactions.end(),
                         [id](const Transaction& t) { return t.getId() == id; });

        if (it != transactions.end()) {
            transactions.erase(it);
            cout << "Transaction deleted successfully!" << endl;
            saveData();
        } else {
            cout << "Transaction not found!" << endl;
        }
    }

private:
    // Select category based on transaction type
    string selectCategory(Transaction::Type type) {
        vector<string>& categories = (type == Transaction::INCOME) ? incomeCategories : expenseCategories;

        cout << "\nSelect category:" << endl;
        for (size_t i = 0; i < categories.size(); i++) {
            cout << i + 1 << ". " << categories[i] << endl;
        }

        cout << "Enter choice (1-" << categories.size() << "): ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice < 1 || choice > categories.size()) {
            return categories.back(); // Return "Other" category
        }

        return categories[choice - 1];
    }

    // Get current date in YYYY-MM-DD format
    string getCurrentDate() {
        time_t now = time(0);
        tm* ltm = localtime(&now);

        stringstream ss;
        ss << (1900 + ltm->tm_year) << "-"
           << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-"
           << setfill('0') << setw(2) << ltm->tm_mday;

        return ss.str();
    }

    // Save data to file
    void saveData() {
        ofstream file(dataFile);
        if (!file) {
            cout << "Error saving data!" << endl;
            return;
        }

        file << nextId << endl;
        for (const auto& t : transactions) {
            file << t.toString() << endl;
        }

        file.close();
    }

    // Load data from file
    void loadData() {
        ifstream file(dataFile);
        if (!file) {
            return; // File doesn't exist yet
        }

        file >> nextId;
        file.ignore();

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string item;
            vector<string> tokens;

            while (getline(ss, item, '|')) {
                tokens.push_back(item);
            }

            if (tokens.size() == 6) {
                int id = stoi(tokens[0]);
                Transaction::Type type = static_cast<Transaction::Type>(stoi(tokens[1]));
                string category = tokens[2];
                double amount = stod(tokens[3]);
                string description = tokens[4];
                string date = tokens[5];

                transactions.emplace_back(id, type, category, amount, description, date);
            }
        }

        file.close();
    }
};

// Main menu function
void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "    PERSONAL FINANCE TRACKER" << endl;
    cout << "========================================" << endl;
    cout << "1. Add Transaction" << endl;
    cout << "2. View All Transactions" << endl;
    cout << "3. View Financial Summary" << endl;
    cout << "4. Generate Monthly Report" << endl;
    cout << "5. Search Transactions" << endl;
    cout << "6. Delete Transaction" << endl;
    cout << "7. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice (1-7): ";
}

int main() {
    FinanceManager manager;

    cout << "Welcome to Personal Finance Tracker!" << endl;
    cout << "Track your income and expenses easily." << endl;

    while (true) {
        displayMenu();

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        cin.ignore();

        switch (choice) {
            case 1:
                manager.addTransaction();
                break;
            case 2:
                manager.viewTransactions();
                break;
            case 3:
                manager.viewSummary();
                break;
            case 4:
                manager.monthlyReport();
                break;
            case 5:
                manager.searchTransactions();
                break;
            case 6:
                manager.deleteTransaction();
                break;
            case 7:
                cout << "\nThank you for using Personal Finance Tracker!" << endl;
                cout << "Your data has been saved. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}
