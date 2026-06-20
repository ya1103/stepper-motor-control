# 🚀 AVR Multi-Stepper Motor Control System

An embedded C application based on the **ATmega32** microcontroller that independently controls three stepper motors via TB6600 drivers. The system features real-time dynamic speed adjustment, physical boundary detection, and a master operational override controlled by an external Programmable Logic Controller (PLC).

---

## 📌 Project Overview

The **AVR Multi-Stepper Motor Control System** is designed for high-precision, continuous motor control environments (such as conveyor systems or robotic arms). It relies on a non-blocking state machine and utilizes hardware timers to generate precise pulse frequencies for the motor drivers.

The system uses a single **ATmega32 microcontroller running at 16 MHz**:

- **Inputs**: 3x Analog Potentiometers, 2x Mechanical Limit Switches, 1x Digital PLC Signal.
- **Outputs**: 3x TB6600 Stepper Motor Drivers (Pulse, Enable and Direction pins).

---

## ✨ Features

* Real-time, independent speed control for three stepper motors (10 - 50 RPM).
* Precise hardware pulse generation using Timers in Compare Match mode.
* Master PLC override for instant system-wide Start/Pause control.
* Hardware-safe auto-recovery after pause states.
* Debounced mechanical boundary detection.
* Intelligent fault handling for abnormal mechanical states.
* Modular, layered software architecture.

---

## 🧩 System Architecture

The core of the system revolves around polling inputs and dynamically updating hardware timer registers to adjust the pulse trains sent to the TB6600 drivers.

**Interfaces:**

* **ADC (Analog-to-Digital Converter):** Reads the 3 potentiometers.
* **Digital I/O:** Monitors the master PLC signal and 2 limit switches.
* **Hardware Timers (Timer0, Timer1, Timer2):** Generates independent step pulses.
* **GPIO:** Sends Direction and Pulse signals to the TB6600 drivers.

---

## ⚙️ Subsystems

### 🎛️ Dynamic Speed Control (Potentiometers & ADC)

Controls the speed of the motors by reading analog voltages and translating them into desired RPM values.

* **Range:** 10 to 50 RPM.
* **Math Logic:** The ADC value alters the Output Compare Register (OCR) using the multi-line formula:

Numerator = 0.3 * f_clk_IO

Denominator = 2 * uStep * N * RPM

OCRnA = [ Numerator / Denominator ] - 1


### ⚡ Pulse Generation (Timers & TB6600 Drivers)

Each motor is driven by a dedicated hardware timer running in Compare Match mode.

* **Motor 1:** Driven by Timer0
* **Motor 2:** Driven by Timer1
* **Motor 3:** Driven by Timer2
* **Driver Config:** TB6600 manually set to **32 Microsteps**.

### 🛑 Boundary Safety (Limit Switches)

Monitors physical boundaries using two mechanical limit switches with software debouncing.

* **Normal Hit:** Reverses the direction of the motor.
* **Fault Hit:** Deactivates specific timers to prevent mechanical damage.

### 🔌 Master Control (PLC Signal)

Acts as the ultimate gatekeeper for the system's operation.

* **LOGIC HIGH:** System runs normally.
* **LOGIC LOW:** System halts all timers instantly and waits for recovery.

---

## 🔁 System Workflow

### 1. System Boot & PLC Wait

Upon power-up, the microcontroller enters a strict waiting loop. It polls the PLC signal and will **not** initialize any hardware until the PLC master switch sends a `LOGIC_HIGH` (Run) command.

### 2. Initialization & Start

Once the PLC signal goes high, the system initializes the ADC, Timers, GPIO, and Motor Drivers exactly once, setting all tracking flags to their default operational states.

### 3. Continuous Polling Loop

During normal operation, the main loop executes the following continuously:

* **Master Check:** Verifies the PLC signal is still HIGH.
* **Read ADC:** Fetches the analog values of the 3 potentiometers.
* **Update Timers:** Calculates the new OCR values and injects them into Timer0, Timer1, and Timer2 to seamlessly adjust motor speeds.

### 4. Boundary Management

At the end of each loop, the system applies a 20ms debounce delay and evaluates the limit switches:

* If **LS1** or **LS2** is hit individually, a flag is set, and a reverse direction signal is sent to the TB6600 driver.
* The flag ensures the motor only reverses once per button press, preventing jitter.

---

## 📊 System Logic Flowchart

The following diagram maps out the hardware relationships and the continuous state-machine logic executed by the microcontroller.

=============================================================================
                          [ HARDWARE PULSE CHAIN ]
=============================================================================
  (POT 1) ──► [ Timer 0 ] ──(Pulses)──► [ TB6600 1 ] ──(Drives)──► [ Stepper 1 ]
  (POT 2) ──► [ Timer 1 ] ──(Pulses)──► [ TB6600 2 ] ──(Drives)──► [ Stepper 2 ]
  (POT 3) ──► [ Timer 2 ] ──(Pulses)──► [ TB6600 3 ] --(Drives)──► [ Stepper 3 ]
=============================================================================

                                     │
                                     ▼
=============================================================================
                         [ 1. SYSTEM INITIALIZATION ]
=============================================================================
                                     │
                                     ▼
                   ┌───────────────────────────────────┐
                   │        WAIT FOR MASTER PLC        │
                   │   Loop until PLC Signal == HIGH   │
                   └───────────────────────────────────┘
                                     │
                                     ▼
                   ┌───────────────────────────────────┐
                   │     INITIALIZE HARDWARE ONCE      │
                   │  - Start Timers (Pulse Gens)      │
                   │  - Setup TB6600s (Motor Drivers)  │
                   │  - Setup Limit Switches           │
                   │  - Setup Potentiometers           │
                   │  - Set all tracking flags = FALSE │
                   └───────────────────────────────────┘
                                     │
                                     ▼
=============================================================================
                        [ 2. CONTINUOUS MAIN LOOP ]
=============================================================================
                                     │
 ┌───────────────────────────────────┤
 │                                   ▼
 │                 ┌───────────────────────────────────┐
 │                 │       MASTER CONTROL CHECK        │
 │                 │       Is PLC Signal HIGH?         │
 │                 └───────────────────────────────────┘
 │                               /       \
 │                             (NO)     (YES)
 │                             /           \
 │   ┌───────────────────────────┐       ┌───────────────────────────────┐
 │   │    MASTER PAUSE STATE     │       │        SYSTEM RECOVERY        │
 │   │ - Stop pulses to all      │       │ Was the system paused before? │
 │   │   TB6600 drivers (DeInit) │       │ - If YES: Restart pulses      │
 │   │ - Set OperationPaused=TRUE│       │   and set flag to FALSE.      │
 │   └───────────────────────────┘       └───────────────────────────────┘
 │             │                                         │
 │             │                                         ▼
 │             │                 ┌───────────────────────────────────────────────┐
 │             │                 │            DYNAMIC SPEED CONTROL              │
 │             │                 │ - Is Timer0 active? -> Read POT1 -> Calc RPM  │
 │             │                 │   -> Update Timer0 OCR (Alters Stepper 1 speed│
 │             │                 │      via TB6600 pulses)                       │
 │             │                 │                                               │
 │             │                 │ - Read POT2 -> Calc RPM -> Update Timer1 OCR  │
 │             │                 │   (Alters Stepper 2 speed via TB6600 pulses)  │
 │             │                 │                                               │
 │             │                 │ - Read POT3 -> Calc RPM -> Update Timer2 OCR  │
 │             │                 │   (Alters Stepper 3 speed via TB6600 pulses)  │
 │             │                 └───────────────────────────────────────────────┘
 │             │                                         │
 │             │                                         ▼
 │             │                 ┌───────────────────────────────────────────────┐
 │             │                 │                DEBOUNCE DELAY                 │
 │             │                 │       Pause for 20ms to settle switches       │
 │             │                 └───────────────────────────────────────────────┘
 │             │                                         │
 │             │                                         ▼
 │             │                 ┌───────────────────────────────────────────────┐
 │             │                 │            SAFETY / BOUNDARY CHECK            │
 │             │                 │         Are BOTH LS1 & LS2 Triggered?         │
 │             │                 └───────────────────────────────────────────────┘
 │             │                                 /               \
 │             │                        (YES - ABNORMAL)     (NO - NORMAL)
 │             │                               /                     \
 │             │       ┌───────────────────────────┐       ┌───────────────────────────┐
 │             │       │    ABNORMAL FAULT TRAP    │       │       NORMAL BOUNDARY     │
 │             │       │ - Kill Timer0 pulses to   │       │ - Was Timer0 paused by a  │
 │             │       │   TB6600 driver 1         │       │   fault? -> If YES:       │
 │             │       │ - Set timer0_Paused=TRUE  │       │   Restart Timer0 pulses.  │
 │             │       │ - (Stepper 1 stops safely)│       │                           │
 │             │       └───────────────────────────┘       │ - LS1 Triggered? (New Hit)│
 │             │                     │                     │   -> Reverse TB6600 Dir   │
 │             │                     │                     │                           │
 │             │                     │                     │ - LS2 Triggered? (New Hit)│
 │             │                     │                     │   -> Reverse TB6600 Dir   │
 │             │                     │                     └───────────────────────────┘
 │             │                     │                                   │
 └─────────────┴─────────────────────┴───────────────────────────────────┘

---

## 🚨 Safety & Fault Handling

| Fault Condition | Trigger | System Response |
| --- | --- | --- |
| **Master Override** | PLC Signal drops to `LOW` | All 3 Timers are De-initialized. System pauses until signal returns to `HIGH`. |
| **Normal Boundary Hit** | Limit Switch 1 OR 2 triggered | TB6600 Direction pin toggled. Software flag locks out rapid re-triggering. |
| **Abnormal Mechanical Fault** | Limit Switches 1 AND 2 triggered simultaneously | Timer0 is killed instantly. Motor 1 stops. System ignores Motor 1 speed updates until the fault is physically cleared. |

---

## 🛠️ System Requirements

* **Microcontroller:** ATmega32
* **System Frequency:** 16 MHz (External Crystal Oscillator)
* **Timer Prescaler:** 64
* **Motor Drivers:** TB6600 (Configured for 32 microsteps)
* **Architecture:** Layered software model

---

## 📚 Drivers Used

### Microcontroller Abstraction Layer (MCAL)

* **GPIO Driver:** Pin and port direction/value management.
* **ADC Driver:** Analog to digital conversion for speed control.
* **Timer Driver:** Hardware timer initialization, Compare Match configuration, and runtime register updates.

### Hardware Abstraction Layer (HAL)

* **TB6600 Driver:** Translates system logic into motor pulses and direction toggles.
* **Potentiometer Driver:** Maps raw ADC readings to 10-50 RPM values.
* **Limit Switch Driver:** Reads physical boundary states safely.
* **PLC Driver:** Interfaces with the external master control signal.

---

## 🧱 Layered Architecture

The software is strictly decoupled to ensure the application layer (`main.c`) never interacts with hardware registers directly.

1. **Application Layer:** `main.c` (State machine, math calculations, fault logic).
2. **HAL Layer:** `tb6600.c`, `potentiometer.c`, `limit_switch.c`, `plc.c`.
3. **MCAL Layer:** `timer.c`, `adc.c`, `gpio.c`.
4. **Utilities:** `std_types.h`, `common_macros.h`.
