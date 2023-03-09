#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Number of candidates
int candidate_count;
int sort_count = 0;

// Array of candidates
candidate candidates[MAX];

// Function prototypes
bool vote(string name);
void sort(int n);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Sort candidates by vote
    sort(sort_count);

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    bool state = false;

    // Run this for every vote: run through all candidates name and look for a match with string name
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            state = true;
            return state;
        }
    }
    return state;
}

// I am doing a bubble sort here
void sort(int n)
{
    if (n == candidate_count)
    {
        return;
    }

    sort(n + 1);
    for (int i = 0; i < n; i++)
    {
        if (candidates[i].votes > candidates[i + 1].votes)
        {
            candidate hold1 = candidates[i];
            candidate hold2 = candidates[i + 1];
            candidates[i] = hold2;
            candidates[i + 1] = hold1;
        }
        if (candidates[i].votes == candidates[i + 1].votes)
        {
            candidate hold1 = candidates[i];
            candidate hold2 = candidates[i + 1];
            candidates[i] = hold2;
            candidates[i + 1] = hold1;
        }
    }
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Candidates[j] is whoever last, if j and second last is equal, print second last and find before j,
    for (int j = candidate_count - 1; candidates[candidate_count - 1].votes == candidates[j].votes; j--)
    {
        printf("%s\n", candidates[j].name);
    }
    return;
}