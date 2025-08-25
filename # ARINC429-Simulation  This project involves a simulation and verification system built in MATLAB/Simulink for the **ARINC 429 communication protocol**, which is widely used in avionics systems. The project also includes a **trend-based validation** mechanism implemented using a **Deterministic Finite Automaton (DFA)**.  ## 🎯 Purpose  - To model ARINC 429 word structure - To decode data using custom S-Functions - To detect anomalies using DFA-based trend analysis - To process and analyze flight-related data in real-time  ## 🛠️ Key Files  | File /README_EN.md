# ARINC429-Simulation

This project involves a simulation and verification system built in MATLAB/Simulink for the **ARINC 429 communication protocol**, which is widely used in avionics systems. The project also includes a **trend-based validation** mechanism implemented using a **Deterministic Finite Automaton (DFA)**.

## 🎯 Purpose

- To model ARINC 429 word structure
- To decode data using custom S-Functions
- To detect anomalies using DFA-based trend analysis
- To process and analyze flight-related data in real-time

## 🛠️ Key Files

| File / Folder                   | Description |
|--------------------------------|-------------|
| `arinc429_decoder.slx`         | Simulink model (for decoding ARINC words) |
| `arinc_label_sfunction.c`      | S-Function for extracting label (written in C) |
| `trend_dfa_sfunc_flight.c`     | DFA trend analysis S-Function (for flight data) |
| `*.mexw64`                      | Precompiled S-Function binaries for Windows |
| `arinc429_bcd_to_decimal.c`    | Converts BCD to Decimal |
| `arinc429_decimal_to_bcd.c`    | Converts Decimal to BCD |
| `data_original.m`, `datas.m`   | MATLAB scripts for data preparation |
| `filtered_data.csv`            | Output results (filtered trend data) |
| `flight_simulation_data.mat`   | Input flight data file |
| `simulation_database_creator.m`| Script for creating an SQLite database |
| `arinc_verileridb`             | Exported SQLite database file |

## 💡 How to Run

1. Open MATLAB and set the project directory as your working folder.
2. Open the Simulink model: `arinc429_decoder.slx`
3. Click `Run` to start the simulation.
4. Analyze trend states and output data through Scope blocks or via the `.csv` and `.db` files.

## 📌 Notes

- `*.mexw64` files are compiled from corresponding `.c` files using MATLAB's `mex` function.
- `.slx.original` is a backup and not required for execution.
- `arinc_verileridb` can be accessed in MATLAB using the `sqlite()` function.

## 🔍 DFA Trend States

- `STABLE`: Steady data
- `INCREASING`: Ascending trend
- `DECREASING`: Descending trend
- `OSCILLATING`: Fluctuating pattern
- `ANOMALY`: Abnormal values or sudden change
