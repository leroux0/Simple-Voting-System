# Simple Voting System

## Overview
This is a console-based voting application written in C. It allows users to register voters, cast votes for candidates, tally results, and ensures basic integrity through duplicate detection and simple hashing. Data is persisted in binary files (`voters.bin` and `votes.bin`) for reuse across sessions.

The program is designed for educational purposes or small-scale polls, demonstrating concepts like data structures (arrays), file I/O, error handling, and basic data integrity checks.

## Features
- **Voter Registration**: Add voters with unique IDs and names.
- **Vote Casting**: Registered voters can vote for one of 3 candidates (configurable).
- **Duplicate Detection**: Prevents multiple votes per voter.
- **Result Tallying**: Displays vote counts per candidate.
- **Data Persistence**: Stores voters and votes in binary files.
- **Integrity Checks**: Uses a simple hash to detect tampering in stored data.
- **Error Handling**: Validates inputs and handles file errors.

## Requirements
- A C compiler (e.g., GCC).
- Standard C libraries (included via `#include <stdio.h>`, etc.).

## How to Build and Run
1. Clone the repository:
   ```
   git clone <repository-url>
   cd simple-voting-system
   ```

2. Compile the code:
   ```
   gcc voting_system.c -o voting_system
   ```

3. Run the executable:
   ```
   ./voting_system
   ```

## Usage
Upon running, you'll see a menu:
```
Simple Voting System
1. Register Voter
2. Cast Vote
3. Tally Results
4. Exit
Enter choice:
```

- Select 1 to register a new voter (enter ID and name).
- Select 2 to cast a vote (enter voter ID and candidate number).
- Select 3 to view results.
- Select 4 to exit (data is saved automatically).

Example:
- Register voters with IDs 1, 2, 3.
- Cast votes for candidates 1-3.
- Tally shows counts.

## Limitations
- Supports up to 100 voters (configurable via `MAX_VOTERS`).
- Fixed 3 candidates (change `NUM_CANDIDATES` to adjust).
- Basic hash for integrity (not cryptographically secure).
- No advanced security (e.g., passwords).
- Uses arrays instead of linked lists for simplicity.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
