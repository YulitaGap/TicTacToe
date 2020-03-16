#include <iostream>
#include <utility>
#include <vector>
#include <tuple>
#include <ctime>
#include "minimax_algorithm.cpp"

#define PLAYER1 'X'
#define PLAYER2 '0'
#define WON 100
#define LOST -100

/**
 Class for representation of a  3x3 game board of TicTacToe game.
 */
class Board{
public:
    char board[3][3]{};
    std::vector<std::vector<std::pair<int, int>>> winning_combinations{
            // Row combinations
            { std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(0, 2) },
            { std::make_pair(1, 0), std::make_pair(1, 1), std::make_pair(1, 2) },
            { std::make_pair(2, 0), std::make_pair(2, 1), std::make_pair(2, 2) },

            // Column combinations
            { std::make_pair(0, 0), std::make_pair(1, 0), std::make_pair(2, 0) },
            { std::make_pair(0, 1), std::make_pair(1, 1), std::make_pair(2, 1) },
            { std::make_pair(0, 2), std::make_pair(1, 2), std::make_pair(2, 2) },

            // Diagonal combinations
            { std::make_pair(0, 0), std::make_pair(1, 1), std::make_pair(2, 2) },
            { std::make_pair(2, 0), std::make_pair(1, 1), std::make_pair(0, 2) }
    };

    /**
     Initializing of starting board state.
     */
    Board() {
        for (auto & i : board) {
            for (char & j : i) {
                j = '_';
            }
        }
    }


    std::vector<std::pair<int, int>> get_possible_moves(){
    /**
    Function returns coordinates of all empty cells for possible player moves.
    */
        std::vector<std::pair<int, int>> possible_moves;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] != PLAYER1 && board[i][j] != PLAYER2)
                {
                    possible_moves.emplace_back(i, j);
                }
            }
        }
        return possible_moves;
    }

    /**
     Function returns coordinates of all cells to which a given player has moved.
     */
    std::vector<std::pair<int, int>> get_visited_cells(char player_sign){
        std::vector<std::pair<int, int>> already_visited;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == player_sign)
                {
                    already_visited.emplace_back(i, j);
                }
            }
        }

        return already_visited;
    }
    /**
     Function returns True if there are no empty cells on the board, False otherwise.
     */
    bool is_full(){
        std::vector<std::pair<int, int>> legal_moves = get_possible_moves();
        return legal_moves.empty();
    }

    /**
     Function returns the sign of opponent.
     */
    static char get_opponent_sign(char player_sign){
        char opponent_sign;
        if (player_sign == PLAYER1)
        {
            return PLAYER2;
        }
        else
        {
            return PLAYER1;
        }
    }

    /**
     Function checks current board state and returns 100 if there is a winner, -100 if someone lost,
     0 in case if nobody won, but the board is already full.
     */
    int get_board_state(char player_sign){
        char opponent_sign = get_opponent_sign(player_sign);
        std::vector<std::pair<int, int>> visited_cells = get_visited_cells(player_sign);
        bool is_winner = has_winner(visited_cells);

        if (is_winner)
        {
            return WON;
        }
        visited_cells = get_visited_cells(opponent_sign);
        bool is_loser = has_winner(visited_cells);

        if (is_loser)
        {
            return LOST;
        }

        bool if_full_board = is_full();
        if (if_full_board)
        {
            return 0;
        }
        return 0;
    }

    /**
     Function checks if there is a winning combination in all visited by a player cells.
     */
    bool has_winner(std::vector<std::pair<int, int>> visited_cells){
        bool is_winner = false;
        for (auto curr_win_state : winning_combinations)
        {
            is_winner = true;
            for (int j = 0; j < 3; j++)
            {
                if (!(std::find(std::begin(visited_cells), std::end(visited_cells), curr_win_state[j]) != std::end(visited_cells)))
                {
                    is_winner = false;
                    break;
                }
            }
            if (is_winner)
            {
                break;
            }
        }
        return is_winner;
    }
    /**
     Function checks if the game should be over : if there is a winner or the board is full.
     */
    bool game_is_over()
    {
        if (is_full())
        {
            return true;
        }

        return 0 != get_board_state(PLAYER2);

    }

    /**
     Helper function for console board representation.
     */
    void print_board(){
        std::cout << "|" << board[0][0] << "|" << board[0][1] << "|" << board[0][2] << "|" << std::endl;
        std::cout << "|" << board[1][0] << "|" << board[1][1] << "|" << board[1][2] << "|"<< std::endl;
        std::cout << "|" << board[2][0] << "|" << board[2][1] << "|" << board[2][2]<< "|" << std::endl << std::endl;
    }

    /**
     Function checks if cell on coodrinates given is already visited.
     */
    bool is_visited(int i, int j){
        return board[i][j] != '_';
    }

    /**
     Function changes the board cell when it's visited by a player given.
     */
    bool move(int i,int j,char player_sign){
        if (!is_visited(i, j)){
            board[i][j] = player_sign;
            return true;
        }
        return false;
    }
};



/**
 Class for representation of a TicTacToe player which could be :
     - a human,
     - a computer with random algorithm,
     - a computer with minimax_aglorithm algorithm.
 */
class Player{
    std::string player_type;
    char player_sign;
public:
    void set_type(std::string ptype){
        player_type = std::move(ptype);
    }
    /**
     Function changes the board cell when it's visited according to the player type.
     */
    std::pair<int,int> process_move(const Board& board){
        std::pair<int,int> cell_to_move;
        if(player_type == "h"){
            cell_to_move = human_move(board);
        }
        if (player_type == "cr"){
            cell_to_move =random_move(board);
        }
        if (player_type == "cm"){
            cell_to_move = minimax_move(board);
        }
        return cell_to_move;
    }
private:
    /**
     Function takes coordinates of cell to visit as a user input.
    */
    static std::pair<int,int> human_move(Board board){
        int row, col;
        std::cout << "> enter row (0/1/2) : ";
        std::cin >> row;
        std::cout << "> enter column (0/1/2) : ";
        std::cin >> col;
        std::cout << std::endl << std::endl;

        if (board.is_visited(row, col))
        {
            std::cout << "Wrong cell. Try again!" << std::endl;
            return human_move(board);
        }
        else
        {
            return std::make_pair(row,col);
        }
    };

    /**
     Function generates coordinates of cell to visit randomly.
    */
    static std::pair<int,int> random_move(Board board){
        std::vector<std::pair<int, int>> possible_moves = board.get_possible_moves();
        if (possible_moves.empty()){
            return std::make_pair(-1,-1);
        }
        srand(time(NULL));
        unsigned long num = rand() %possible_moves.size();
        return possible_moves[num];
    };
    /**
     Function generates coordinates of cell to visit by minimax_aglorithm algorithm.
    */
    static std::pair<int,int> minimax_move(Board curr_board){
        return findBestMove(curr_board.board);
    };
};


/**
 Class for representation of a TicTacToe game process.
 */
class Game{
public:
    Board game_board;
    Player player1;
    Player player2;
    Game(){
        std::cout << "**********************************" << std::endl;
        std::cout << "WELCOME TO THE TIC-TAC-TOE GAME!" << std::endl;
        std::cout << "**********************************" << std::endl;
        get_players();
        process_game();
        std::cout << "**********************************\n"<< " GAME OVER \n**********************************" << std::endl << std::endl;
    }


private:
    /**
    Function gets type of players as a user input and sets them for the game.
    */
    void get_players(){
        std::string pl1;
        std::string pl2;
        std::cout << "h - human, cr - computer using random algorithm, cm -- computer, using minimax_aglorithm  algorithm" << std::endl;
        std::cout << "> player 1: ";
        std::cin >> pl1;
        std::cout << "> player 2: ";
        std::cin >> pl2;
        if ((pl1 != "h" && pl1 != "cr" && pl1 != "cm") || (pl2 != "h" && pl2 != "cr" && pl2 != "cm") ){
            std::cout << "Wrong players type. Try again!" << std::endl;
            get_players();
        }
        player1.set_type(pl1);
        player2.set_type(pl2);
    }

    /**
    Function processes a sequence of game moves until someone wins or a board is full.
    */
    void process_game(){
        while (!game_board.game_is_over())
        {
            std::pair<int,int> m1;
            std::pair<int,int> m2;
            std::cout << "Player 1 turn: " << std::endl;
            m1 = player1.process_move(game_board);
            game_board.move(m1.first,m1.second,PLAYER1);
            if (game_board.has_winner(game_board.get_visited_cells(PLAYER1)))
            {
                std::cout << "Player 1 won!" << std::endl;
                game_board.print_board();
                break;
            }
            game_board.print_board();

            std::cout << "Player 2 turn: " << std::endl;
            m2 = player2.process_move(game_board);
            game_board.move(m2.first,m2.second,PLAYER2);
            if (game_board.has_winner(game_board.get_visited_cells(PLAYER2)))
            {
                std::cout << "Player 2 won!" << std::endl;
                game_board.print_board();
                break;
            }
            game_board.print_board();
        }
    }
};



int main() {
    Game game1;
    return 0;

}
