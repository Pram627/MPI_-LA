#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    printf("Process %d started\n", world_rank);

    // Create separate communicators for boys, girls, and all players
    MPI_Comm boys_comm = MPI_COMM_NULL, girls_comm = MPI_COMM_NULL, all_players_comm = MPI_COMM_NULL;

    if (world_rank % 2 == 0) {
        // Even ranks are boys
        MPI_Comm_split(MPI_COMM_WORLD, 0, world_rank, &boys_comm);
    }
    else {
        // Odd ranks are girls
        MPI_Comm_split(MPI_COMM_WORLD, 1, world_rank, &girls_comm);
    }

    // All players communicator
    MPI_Comm_split(MPI_COMM_WORLD, world_rank % 2, world_rank, &all_players_comm);

    printf("Process %d created communicators\n", world_rank);

    // Communication within each group
    if (world_rank % 2 == 0) {
        // Boys communicate
        const char* message = "Message from boys";
        MPI_Bcast((void*)message, (int)(strlen(message) + 1), MPI_CHAR, 0, boys_comm);
    }
    else {
        // Girls communicate
        const char* message = "Message from girls";
        MPI_Bcast((void*)message, (int)(strlen(message) + 1), MPI_CHAR, 0, girls_comm);
    }

    // Communication with all players
    const char* all_players_message = "Message for all players";
    MPI_Bcast((void*)all_players_message, (int)(strlen(all_players_message) + 1), MPI_CHAR, 0, all_players_comm);

    printf("Process %d completed communication\n", world_rank);

    // Free communicators if they have been created
    if (boys_comm != MPI_COMM_NULL) {
        MPI_Comm_free(&boys_comm);
    }
    if (girls_comm != MPI_COMM_NULL) {
        MPI_Comm_free(&girls_comm);
    }
    if (all_players_comm != MPI_COMM_NULL) {
        MPI_Comm_free(&all_players_comm);
    }

    // Print a message before finalizing
    printf("Process %d is finalizing\n", world_rank);

    MPI_Finalize();

    return 0;
}
