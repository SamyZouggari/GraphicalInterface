# Graphical Interface Framework

This repository contains the implementation of a graphical interface framework written in C. The project is designed to create and manage graphical applications with customizable widgets and a structured approach to event handling, rendering, and geometry management.

---

## Features

- **Custom Widget System**:
  - Support for multiple widget types, including buttons, text entries, and frames.
  - Configurable attributes such as size, color, border width, and placement.

- **Geometry Management**:
  - Built-in geometry managers like `placer` for flexible widget positioning.

- **Rendering**:
  - Functions for drawing shapes and textures with support for effects like 3D relief.

- **Event Handling**:
  - Core functions for managing application events.

- **Parsing and Configuration**:
  - Support for external configuration files (e.g., `parsing.txt`) to define UI elements.

---

## Repository Structure

```
/
├── core/
│   ├── ei_application.c         # Application initialization and main loop
│   ├── ei_draw.c                # Rendering utilities for graphical elements
│   ├── ei_geometrymanager.c     # Core logic for widget placement
│   ├── ei_parser.c              # Parsing of configuration files
│   ├── ei_placer.c              # Implementation of the placer geometry manager
│
├── widgets/
│   ├── ei_widget.c              # Core widget management
│   ├── ei_widget_attributes.c   # Management of widget attributes
│   ├── ei_widget_configure.c    # Widget configuration utilities
│   ├── ei_widgetclass.c         # Widget class definitions and registration
│   ├── button.c                 # Specific implementation for button widgets
│   ├── entry.c                  # Specific implementation for text entry widgets
│
├── utils/
│   ├── dessin_relief.c          # Rendering elements with a 3D relief effect
│   ├── freq_counter.c           # Utilities for tracking frequency of events
│   ├── parsing.txt              # Sample configuration file for UI
│
├── examples/
│   ├── hello_world.c            # Example of a simple application
│   ├── minimal.c                # Minimalistic example to demonstrate setup
│   ├── minesweeper.c            # Example: Implementation of Minesweeper
│   ├── two048.c                 # Example: Implementation of 2048 game
│   ├── puzzle.c                 # Example: Implementation of a puzzle game
│
└── README.md                    # Project documentation
```

---

## Installation

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd GraphicalInterface
   ```

2. Compile the project:
   ```bash
   make
   ```

3. Run an example:
   ```bash
   ./examples/hello_world
   ```

---

## How to Use

You need to install clion to compile and run this project. There is no Makefile.
Not all the tests run well ...

---

## Examples

- **Hello World**:
  A simple example that initializes the framework and displays a single widget.

- **2048 Game**:
  Implements the popular 2048 game with a graphical interface.

- **Minesweeper**:
  A fully functional Minesweeper game built with the framework.

---

## Contribution

Contributions are welcome! Feel free to fork this repository and submit pull requests to improve the framework or add new features.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

---

## Contribution to the project

We were in group of 3. My contribution was to implement the geometrymanager, the placer, to code the draw functions of each widget, to code the main loop, etc. 
Tests functions were provided.
