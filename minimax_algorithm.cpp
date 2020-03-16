#include <iostream>
#include <math.h>

char player = 'X', opponent = '0';
bool has_remaining_moves(char board[3][3])
{
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]=='_')
                return true;
    return false;
}

/**
 Function to evaluate_state current state of board.
 Returns a score according to amount and position of already visited cells.
 */
int evaluate_state(char board_state[3][3])
{
    for (int row = 0; row<3; row++)
    {
        if (board_state[row][0] == board_state[row][1] &&
            board_state[row][1] == board_state[row][2])
        {
            if (board_state[row][0] == player)
                return +10;
            else if (board_state[row][0] == opponent)
                return -10;
        }
    }

    for (int col = 0; col<3; col++)
    {
        if (board_state[0][col] == board_state[1][col] &&
            board_state[1][col] == board_state[2][col])
        {
            if (board_state[0][col] == player)
                return +10;

            else if (board_state[0][col] == opponent)
                return -10;
        }
    }

    if (board_state[0][0] == board_state[1][1] && board_state[1][1] == board_state[2][2])
    {
        if (board_state[0][0] == player)
            return +10;
        else if (board_state[0][0] == opponent)
            return -10;
    }

    if (board_state[0][2] == board_state[1][1] && board_state[1][1] == board_state[2][0])
    {
        if (board_state[0][2] == player)
            return +10;
        else if (board_state[0][2] == opponent)
            return -10;
    }
    return 0;
}
/**
 Function compares game outcomes of different moves combinations.
 */
int minimax_aglorithm(char board[3][3], int depth, bool isMax)
{
    int curr_score = evaluate_state(board);
    if (curr_score == 10)
        return curr_score;

    if (curr_score == -10)
        return curr_score;

    if (!has_remaining_moves(board))
        return 0;

    if (isMax)
    {
        int best_score = -1000;
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                if (board[i][j]=='_')
                {
                    board[i][j] = player;
                    best_score = fmax(best_score,
                                      minimax_aglorithm(board, depth + 1, !isMax) );
                    board[i][j] = '_';
                }
            }
        }
        return best_score;
    }
    else
    {
        int best_score = 1000;
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                if (board[i][j]=='_')
                {
                    board[i][j] = opponent;

                    best_score = fmin(best_score,
                                      minimax_aglorithm(board, depth + 1, !isMax));

                    board[i][j] = '_';
                }
            }
        }
        return best_score;
    }
}

/**
 Function compares all possible moves and scores for this moves
 and returns the best scored move cell coordinates.
 */
std::pair<int,int> findBestMove(char board[3][3])
{
    int best_score = -1000;
    std::pair<int,int> best_cell;
    best_cell = std::make_pair(-1,-1);
    for (int i = 0; i<3; i++)
    {
        for (int j = 0; j<3; j++)
        {
            if (board[i][j]=='_')
            {
                board[i][j] = player;
                int curr_move_score = minimax_aglorithm(board, 0, false);
                board[i][j] = '_';
                if (curr_move_score > best_score)
                {
                    best_cell = std::make_pair(i,j);
                    best_score = curr_move_score;
                }
            }
        }
    }
    return best_cell;
}
