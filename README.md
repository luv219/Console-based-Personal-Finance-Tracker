# Console-based Personal Finance Tracker
- A command-line application for managing personal finances. Users can add income and expense transactions, categorise them, view a summary of their financial status, and save/load their data from a file. 
- A simple, single-file C++ console application to track **income** and **expenses**, summarise totals, search transactions, and generate **monthly reports**. Data is persisted in a plain-text file so your records survive between runs.

> ✅ No external libraries.  
> ✅ Works on Windows, Linux, and macOS (C++17).  
> ✅ Human‑readable save file (`finance_data.txt`).

---

## ✨ Features

- Add **Income** or **Expense** with category, amount, description, and date (defaults to today).
- View all transactions in a neat table.
- Summaries:
  - **Totals** for income, expenses, balance
  - **Category-wise breakdown**
- **Monthly report** (choose month & year).
- **Search** by category, description keyword, or amount range.
- **Delete** by transaction ID.
- Auto-save after every change (to `finance_data.txt`).

---

## 🧱 How it Works (Design Overview)

- `Transaction` class: stores `id`, `type` (INCOME/EXPENSE), `category`, `amount`, `description`, `date` (YYYY-MM-DD).
- `FinanceManager` class: holds the list, next ID, and the data file. Implements:
  - `addTransaction`, `viewTransactions`, `viewSummary`, `monthlyReport`,
    `searchTransactions`, `deleteTransaction`
  - `saveData()` and `loadData()` to persist/restore from `finance_data.txt`
- **Categories** (editable in code):
  - Income: Salary, Freelance, Investment, Gift, Other Income
  - Expense: Food, Transport, Housing, Entertainment, Healthcare, Education, Shopping, Utilities, Other Expense

---

## 🖥️ Build Instructions

> Requires a C++17 compiler.

### Windows (MSVC)
```bat
cl /EHsc /std:c++17 main.cpp /Fe:FinanceTracker.exe
FinanceTracker.exe
```

### Windows (MinGW) / Linux / macOS (Clang or GCC)
```bash
g++ -std=c++17 -O2 -o finance_tracker main.cpp
./finance_tracker
```
---

## ▶️ Usage

On launch you’ll see a menu:

```
1. Add Transaction
2. View All Transactions
3. View Financial Summary
4. Generate Monthly Report
5. Search Transactions
6. Delete Transaction
7. Exit
```

### Add Transaction
- Choose **Income** or **Expense**
- Pick a **category**
- Enter **amount** (positive)
- Add a **description**
- Confirm **date** (defaults to today; or enter `YYYY-MM-DD`)

### Search Options
- By **category** (partial matches OK)
- By **description** keyword
- By **amount range** (min–max)

### Delete
- First list all to see IDs, then enter the ID to delete. Auto-saves afterward.

---

## 💾 Data & File Format

- Save file: `finance_data.txt` (created in the current working directory).
- First line stores `nextId`. Each transaction uses a `|`-delimited line:

```
<id>|<type>|<category>|<amount>|<description>|<date>
```

Where:
- `<type>` is `0` for INCOME and `1` for EXPENSE
- `<date>` is `YYYY-MM-DD`

**Example:**
```
12|1|Food|380.50|Groceries for the week|2025-09-09
13|0|Salary|75000|Monthly salary|2025-09-01
```

> Tip: You can back up or version-control `finance_data.txt` to keep history.

---

## 🧪 Example Session

```
Welcome to Personal Finance Tracker!
Track your income and expenses easily.

1. Add Transaction
2. View All Transactions
3. View Financial Summary
4. Generate Monthly Report
5. Search Transactions
6. Delete Transaction
7. Exit
Enter your choice (1-7): 1

=== Add New Transaction ===
Transaction Type:
1. Income
2. Expense
Enter choice (1-2): 2

Select category:
1. Food
2. Transport
...
Enter choice (1-9): 1
Enter amount: $1200
Enter description: Family dinner
Use current date (2025-09-10)? (y/n): y

Transaction added successfully!
```

---

## 🛠️ Notes & Extensibility

- **Change categories** inside `FinanceManager` (the `incomeCategories` and `expenseCategories` vectors).
- **Date parsing** for monthly reports expects `YYYY-MM-DD` (year‑month used for filtering).
- **Input validation** is basic—consider expanding it (e.g., forbid very long descriptions).
- Possible future additions:
  - Export reports to CSV/JSON
  - Budgets & alerts
  - Recurring transactions
  - Better CLI navigation / colors
  - Unit tests

---

## ❓ Troubleshooting

- **I don’t see my old data.** Ensure you are running the program from the same working directory that contains `finance_data.txt`.
- **Build errors about C++ standard.** Make sure your compiler flag includes `-std=c++17` (or `/std:c++17` with MSVC).
- **Unicode/locale issues.** Stick to ASCII in descriptions if your terminal doesn’t support your locale.

---
