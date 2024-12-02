# AlarmProject

**Overview**
This project involves creating a modular console application for a security company to manage and simulate alarm systems. 
The program integrates with either SQL or NoSQL databases and allows for functionality such as customer registration, alarm testing, and user activity simulation.

**SRS**

1.3 Intended Use
The purpose of the program:
A console interface for an alarm operator

What the program does not do/is:

2.1 User Needs
The user needs:
The user should be able to register the customer's name/company name

3.1 Functional Requirements
User interactions, user requirements, what the program does:
The user should be assigned an ID
The user should be assigned a numeric code for activating/deactivating the alarm
A tag ID for the RFD sensor
Verification phase for false alarms
Component type for the registered alarm
Component name for the registered alarm
Unique ID for the registered alarm
Can be turned off with a shutdown phrase

3.4 Nonfunctional Requirements
Security, performance:

**Development To-Do List**
https://prod.liveshare.vsengsaas.visualstudio.com/join?F670F176C85669B6289A59CF11ACC50E7482
Phase 1: Setup
[x] Create the project structure.
[x] Define models for customers, users, alarm systems, and components.
[ongoing] Implement a database interface and NoSQL functionality (JSON).

Phase 2: Core Functionality
[ongoing] Build the user interface (console-based).
[ongoing] Enable customer and alarm system registration.
[ongoing] Add functionality to register alarm components.
[ongoing] Implement alarm system testing with verification.

Phase 3: Database Integration
[ongoing] Add SQL or JSON support using SQLite or Nhlmann.
[ongoing] Ensure data persistence between program runs.
[] Allow database type selection at initialization.

Phase 4: Advanced Features (Optional)
[] Simulate customer interactions (alarm activation/deactivation).
[] Log user activities with timestamps.
[] Add advanced database queries and reports.

Phase 5: Testing and Documentation
[] Create unit tests for all key features.
[] Document the code and create user guides.
[] Prepare for final demonstration.
