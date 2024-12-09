# AlarmProject

## Overview
This project involves creating a modular console application for a security company to manage and simulate alarm systems. The program integrates with either SQL or NoSQL databases and provides functionalities such as customer registration, alarm testing, and user activity simulation.

---

## Software Requirements Specification (SRS)

### 1.3 Intended Use
**Purpose**:  
The program serves as a console interface for an alarm operator.

**Exclusions**:  
The program does not include advanced graphical interfaces or physical hardware interactions.

### 2.1 User Needs
The application addresses the following user needs:
- The user should be able to register the customer's name and/or company name.

### 3.1 Functional Requirements
The program supports the following functionalities:
- Assigning a unique ID to each user.
- Providing users with a numeric code for alarm activation/deactivation.
- Issuing a tag ID for the RFID sensor.
- Verifying false alarms during the alarm testing phase.
- Registering and managing alarm components, including:
  - Component type
  - Component name
  - Unique component ID
- Supporting a shutdown phrase to turn off alarms.

### 3.4 Nonfunctional Requirements
- **Security**: Ensure secure storage and handling of user codes and data.
- **Performance**: The program should run efficiently on standard systems and handle multiple alarms effectively.

---

## Development To-Do List
### Phase 1: Setup
- [x] Create the project structure.
- [x] Define models for customers, users, alarm systems, and components.
- [ongoing] Implement a database interface and NoSQL functionality (JSON).

### Phase 2: Core Functionality
- [ongoing] Build the user interface (console-based).
- [ongoing] Enable customer and alarm system registration.
- [ongoing] Add functionality to register alarm components.
- [ongoing] Implement alarm system testing with verification.

### Phase 3: Database Integration
- [ongoing] Add SQL or JSON support using SQLite or Nlohmann.
- [ongoing] Ensure data persistence between program runs.
- [ ] Allow database type selection at initialization.

### Phase 4: Advanced Features (Optional)
- [ ] Simulate customer interactions (alarm activation/deactivation).
- [ ] Log user activities with timestamps.
- [ ] Add advanced database queries and reports.

---

## Collaboration
To join the development session, use the following link:  
[Live Share Session](https://prod.liveshare.vsengsaas.visualstudio.com/join?F670F176C85669B6289A59CF11ACC50E7482)
