# 🗓️ University Timetable Management System

This is a C++ console application designed for managing university timetables. It provides functionality for Admins, Students, and Lecturers to manage modules, sessions, and weekly schedules with conflict detection, validation, and user-friendly CLI menus.

---

## 🔧 Features

### ✅ User Roles:
- **Admin**: Create/manage modules, users, groups, rooms, and session types.
- **Student**: View modules and their timetable.
- **Lecturer**: View assigned modules and timetable.

### ✅ Core Functionality:
- Create, read, update, and delete:
  - Modules
  - Students, Lecturers, Admins
  - Session Types (e.g., Lecture, Seminar)
  - Groups (e.g., G1, G2)
  - Rooms (e.g., R101)
- Manage weekly timetables with session objects.
- Detect and resolve scheduling conflicts.
- Export timetables to `.csv`.

### ✅ Persistence:
- JSON-based save/load using `nlohmann::json`
- Saved data includes modules, users, timetables, etc.

### ✅ Validation:
- Validates times, room uniqueness, and overlapping sessions.

---

## 🧪 Tests and Coverage

- Unit tests are provided using assertions in `tests/tests.cpp`
- **Over 80% line coverage** using `llvm-cov`
- Run tests with:

```bash
cmake -G Ninja -DENABLE_COVERAGE=ON -S . -B out/build/coverage
ninja -C out/build/coverage coverage
```

Coverage HTML reports are output to `/coverage_html`

---

## 📦 Packaging

Run this to generate a ZIP package:

```bash
cpack
```

The final `.zip` includes:

```
SRC/        -> All source files
TESTS/      -> Test files (including `tests.cpp`)
BUILD/      -> Build files (excluding binaries, intermediate junk)
```

---

## 🚀 Build & Run

### 🖥️ Requirements:
- C++17 compiler (Clang preferred for coverage)
- CMake ≥ 3.10
- Ninja
- Git

### ⚙️ Build (main app):
```bash
cmake -G Ninja -S . -B out/build/release
ninja -C out/build/release timetable
```

### ▶️ Run:
```bash
out/build/release/timetable.exe
```

---

## 🔐 Sample Credentials (demo data):

| Role   | Username | Password |
|--------|----------|----------|
| Admin  | A1       | admin    |
| Student| S1       | student  |
| Lecturer| L1      | lecturer |

---

## ⚙️ CI & Static Analysis

- **Static analysis** with `cppcheck` (hooked into CMake)
- **CI GitHub Actions Workflow** for build + test
- CI ensures coverage ≥ 80% before passing
- Run `cppcheck` manually with:
```bash
cmake --build . --target cppcheck
```

---

## 📝 Known Issues

- No GUI interface
- No login persistence (CLI only)
- Limited error reporting (throws exceptions, no logs)

---

## 📎 Report Checklist (Submission Goals)

- [x] Coverage > 80%
- [x] JSON save/load
- [x] Conflict detection
- [x] Role-based CLI menus
- [x] Release ZIP via CPack
- [x] README + usage guide
- [ ] Final report + UML
- [ ] Video demo (≤ 5 min)
- [ ] Submission via NOW

---

## 📁 Directory Overview

```
📁 src/
    Main source code files

📁 tests/
    Unit & integration tests

📁 lib/
    External libraries (e.g. json.hpp)

📁 build/ or out/
    Build output (excluded from packaging)

📁 coverage_html/
    HTML reports from llvm-cov (excluded from packaging)
```

---

## © Francisco Quadros

**Francisco Quadros N1146376**  
Nottingham Trent University
