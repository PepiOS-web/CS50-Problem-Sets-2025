#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    /*DEBUG
    printf("pair_count = %d\n", pair_count);
    for (int k = 0; k < pair_count; k++)
    {
    printf("pair %d: winner=%d loser=%d  (pref[w][l]=%d vs pref[l][w]=%d)\n",
           k,
           pairs[k].winner,
           pairs[k].loser,
           preferences[pairs[k].winner][pairs[k].loser],
           preferences[pairs[k].loser][pairs[k].winner]);
    }
    */

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{   //Ranks is a two dimentional array so we need to for lops to read al arguments
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++) // Important to add +1
        {
            int higher = ranks[i]; //For all the high scores
            int lower = ranks [j]; // For all the lower scores
            preferences[higher][lower] += 1; //Count to sum up
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++) // Important to add +1
        {
            if  (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    //We go trought all pairs
    for (int i = 0; i < pair_count - 1; i++)
    {
        int max_index = i; //We assum that the highest is in i

        for (int j = i + 1; j < pair_count; j++)//Go trought al pairs in i
        {
            int min =   preferences[pairs[j].winner][pairs[j].loser]
                        - preferences[pairs[j].loser][pairs[j].winner];

            int max = preferences[pairs[max_index].winner][pairs[max_index].loser]
                      - preferences[pairs[max_index].loser][pairs[max_index].winner];
            if (min > max)
            {
                max_index = j;
            }
        }
        if (max_index != i)
        {
            pair temp = pairs[i];
            pairs[i] = pairs[max_index];
            pairs[max_index] = temp;
        }
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}
