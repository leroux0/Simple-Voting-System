#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants
#define MAX_VOTERS 100
#define MAX_NAME_LEN 50
#define NUM_CANDIDATES 3
#define VOTERS_FILE "voters.bin"
#define VOTES_FILE "votes.bin"

// Simple hash function for integrity check (sum of ASCII values mod 256)
unsigned char simple_hash(const void *data, size_t size) {
    unsigned char hash = 0;
    const unsigned char *bytes = (const unsigned char *)data;
    for (size_t i = 0; i < size; i++) {
        hash = (hash + bytes[i]) % 256;
    }
    return hash;
}

// Voter structure
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    unsigned char hash;  // Hash for integrity
} Voter;

// Vote structure
typedef struct {
    int voter_id;
    int candidate;  // 1 to NUM_CANDIDATES
    unsigned char hash;  // Hash for integrity
} Vote;

// Global arrays (using arrays for simplicity instead of linked lists)
Voter voters[MAX_VOTERS];
int num_voters = 0;

Vote votes[MAX_VOTERS];  // Max votes same as max voters
int num_votes = 0;

// Function prototypes
void load_data();
void save_data();
int find_voter_by_id(int id);
int has_voted(int voter_id);
void register_voter();
void cast_vote();
void tally_results();
void menu();

int main() {
    load_data();
    menu();
    save_data();
    return 0;
}

// Load data from binary files
void load_data() {
    // Load voters
    FILE *fp = fopen(VOTERS_FILE, "rb");
    if (fp != NULL) {
        fread(&num_voters, sizeof(int), 1, fp);
        for (int i = 0; i < num_voters; i++) {
            fread(&voters[i], sizeof(Voter), 1, fp);
            // Verify hash for integrity
            unsigned char computed_hash = simple_hash(&voters[i], sizeof(int) + MAX_NAME_LEN);
            if (computed_hash != voters[i].hash) {
                printf("Warning: Voter data integrity check failed for ID %d!\n", voters[i].id);
            }
        }
        fclose(fp);
    }

    // Load votes
    fp = fopen(VOTES_FILE, "rb");
    if (fp != NULL) {
        fread(&num_votes, sizeof(int), 1, fp);
        for (int i = 0; i < num_votes; i++) {
            fread(&votes[i], sizeof(Vote), 1, fp);
            // Verify hash for integrity
            unsigned char computed_hash = simple_hash(&votes[i], sizeof(int) + sizeof(int));
            if (computed_hash != votes[i].hash) {
                printf("Warning: Vote data integrity check failed for voter ID %d!\n", votes[i].voter_id);
            }
        }
        fclose(fp);
    }
}

// Save data to binary files
void save_data() {
    // Save voters
    FILE *fp = fopen(VOTERS_FILE, "wb");
    if (fp == NULL) {
        printf("Error: Could not open voters file for writing.\n");
        return;
    }
    fwrite(&num_voters, sizeof(int), 1, fp);
    for (int i = 0; i < num_voters; i++) {
        // Compute and set hash
        voters[i].hash = simple_hash(&voters[i], sizeof(int) + MAX_NAME_LEN);
        fwrite(&voters[i], sizeof(Voter), 1, fp);
    }
    fclose(fp);

    // Save votes
    fp = fopen(VOTES_FILE, "wb");
    if (fp == NULL) {
        printf("Error: Could not open votes file for writing.\n");
        return;
    }
    fwrite(&num_votes, sizeof(int), 1, fp);
    for (int i = 0; i < num_votes; i++) {
        // Compute and set hash
        votes[i].hash = simple_hash(&votes[i], sizeof(int) + sizeof(int));
        fwrite(&votes[i], sizeof(Vote), 1, fp);
    }
    fclose(fp);
}

// Find voter index by ID
int find_voter_by_id(int id) {
    for (int i = 0; i < num_voters; i++) {
        if (voters[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Check if voter has already voted
int has_voted(int voter_id) {
    for (int i = 0; i < num_votes; i++) {
        if (votes[i].voter_id == voter_id) {
            return 1;
        }
    }
    return 0;
}

// Register a new voter
void register_voter() {
    if (num_voters >= MAX_VOTERS) {
        printf("Error: Maximum number of voters reached.\n");
        return;
    }

    Voter new_voter;
    printf("Enter voter ID (unique integer): ");
    if (scanf("%d", &new_voter.id) != 1) {
        printf("Error: Invalid input.\n");
        while (getchar() != '\n');  // Clear input buffer
        return;
    }

    // Check for duplicate ID
    if (find_voter_by_id(new_voter.id) != -1) {
        printf("Error: Voter ID already exists.\n");
        return;
    }

    printf("Enter voter name: ");
    getchar();  // Consume newline
    if (fgets(new_voter.name, MAX_NAME_LEN, stdin) == NULL) {
        printf("Error: Invalid input.\n");
        return;
    }
    // Remove trailing newline if present
    size_t len = strlen(new_voter.name);
    if (len > 0 && new_voter.name[len - 1] == '\n') {
        new_voter.name[len - 1] = '\0';
    }

    voters[num_voters++] = new_voter;
    printf("Voter registered successfully.\n");
}

// Cast a vote
void cast_vote() {
    int voter_id;
    printf("Enter your voter ID: ");
    if (scanf("%d", &voter_id) != 1) {
        printf("Error: Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    // Check if voter exists
    if (find_voter_by_id(voter_id) == -1) {
        printf("Error: Voter ID not found.\n");
        return;
    }

    // Check if already voted
    if (has_voted(voter_id)) {
        printf("Error: You have already voted.\n");
        return;
    }

    Vote new_vote;
    new_vote.voter_id = voter_id;
    printf("Enter candidate number (1-%d): ", NUM_CANDIDATES);
    if (scanf("%d", &new_vote.candidate) != 1) {
        printf("Error: Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    if (new_vote.candidate < 1 || new_vote.candidate > NUM_CANDIDATES) {
        printf("Error: Invalid candidate number.\n");
        return;
    }

    votes[num_votes++] = new_vote;
    printf("Vote cast successfully.\n");
}

// Tally and display results
void tally_results() {
    int tally[NUM_CANDIDATES] = {0};

    for (int i = 0; i < num_votes; i++) {
        if (votes[i].candidate >= 1 && votes[i].candidate <= NUM_CANDIDATES) {
            tally[votes[i].candidate - 1]++;
        }
    }

    printf("Voting Results:\n");
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        printf("Candidate %d: %d votes\n", i + 1, tally[i]);
    }
    printf("Total votes: %d\n", num_votes);
}

// Display menu and handle choices
void menu() {
    int choice;
    do {
        printf("\nSimple Voting System\n");
        printf("1. Register Voter\n");
        printf("2. Cast Vote\n");
        printf("3. Tally Results\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Error: Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                register_voter();
                break;
            case 2:
                cast_vote();
                break;
            case 3:
                tally_results();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Error: Invalid choice.\n");
        }
    } while (choice != 4);
}