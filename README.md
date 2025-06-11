# 💸 Money Tracker (Beta)

**Status**: 🚧 Beta – Console Only  
**Author**: Almohtady Bellah  
**License**: MIT  
**Next Goal**: GUI Integration  

---

## 🔍 Overview

**Money Tracker** is a lightweight **console-based finance manager** written in C++.  
It helps users track transactions, manage personal loans (borrowers/lenders), and write short notes — all from the terminal.

Ideal for:
- Personal finance tracking
- Practicing OOP and file handling in C++
- Building toward a full-featured GUI app

---

## 📜 Current Features (Console UI)

```
Choose:
1-  View Data
2-  Make A Transaction
3-  Write A Short Note
4-  Add/Edit A Borrower/Lender
5-  Save and Exit
6-  Exit Only
99- RESET and Exit
```

- **View Data**: Show all saved records  
- **Make A Transaction**: Add income/expense entries  
- **Write A Short Note**: Quick notes tied to sessions  
- **Manage Borrowers/Lenders**: Add or update people you owe or lend to  
- **Save/Exit Options**: Save progress or exit without changes  
- **RESET**: Wipe all data and exit (use carefully)  

---

## 🚀 Planned Features

- GUI using **[your choice: Qt / ImGui / SDL / etc.]**
- Data visualization (graphs, pie charts)
- Encrypted data storage
- Multi-user support
- Cross-platform packaging

---

## 🛠️ How to Run (Beta)

1. Clone the repo:
   ```bash
   git clone https://github.com/yourusername/money-tracker.git
   cd money-tracker
   ```

2. Compile the project:
   ```bash
   g++ -o money_tracker main.cpp
   ```

3. Run it:
   ```bash
   ./money_tracker
   ```

> 💡 Requires a C++17+ compatible compiler

---

## ⚠️ Disclaimer

This project is **not ready for production use**.  
Data is stored in plaintext, and there is no encryption or backup system yet.  
Use at your own risk during beta.

---

## 🤝 Contributing

Feel free to open issues or pull requests if you'd like to contribute ideas, fixes, or improvements.

---

## 🧠 Inspiration

Built as a learning project and stepping stone to larger, GUI-based tools.  
The goal is to evolve this into a powerful yet lightweight personal finance application.

---

## 📅 Roadmap

- [ ] Modularize code
- [ ] Add config/settings file
- [ ] Integrate file encryption
- [ ] GUI implementation
- [ ] Add unit testing

---

## 📬 Contact

Check out my website: [almohtadyg1.pythonanywhere.com](https://almohtadyg1.pythonanywhere.com/)

---

## 🪪 License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).
