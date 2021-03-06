//
//  main.cpp
//  dominion
//
//  Created by Kyle Hess on 8/19/20.
//  Copyright © 2020 Kyle Hess. All rights reserved.
//

/*
 
 TODO
   1) Finish implementing base game and base cards
     DONE A: Make function for transitioning to next player's turn
     DONE B: Make a function for buying cards
     DONE C: Make a function for playing cards
     D: Test for victory conditions
   DONE 2) Make interface for interacting with game
   DONE 3) Add types and names for cards
   4) Make ordered card use vector<int> as an ordering
   DONE 5) Allow viewing other player's info
   6) Add info on cards
   7) Add function to print out all information about game state
 
 */

#include <iostream>
#include <string>
#include <vector>
#include "card.hpp"
#include "states.hpp"
using namespace std;

// TODO: Make more checks for formatting so that Jacob's errors can be caught more easily
// TODO: Change if statement to case statement to speed things up if they are too slow
int main() {
    srand(RANDOM_SEED);
    
    vector<BoardState*> games;
    int currGame = 0;
    bool debugMode = false;
    
    while (true) {
        string input;
        getline(cin, input);
        
        vector<string> words;
        string tempWord;
        for (int i = 0; i < input.size(); ++i) {
            if (input[i] != ' ') tempWord += input[i];
            else {
                words.push_back(tempWord);
                tempWord = "";
            }
        }
        if (tempWord != "") words.push_back(tempWord);
        
        if (words.empty()) {
            cout << "ERROR: No command given" << endl;
            continue;
        }
        
        // TODO: Change the following massive if statement into a case statement?
        
        ////////////////////////////////////////////////////////////////////////////////
        // Help
        ////////////////////////////////////////////////////////////////////////////////
        
        if (words[0] == "help") {
            cout << "Just ask Kyle via facebook messenger since he's too lazy to type everything out right now." << endl;
            continue;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Game management
        ////////////////////////////////////////////////////////////////////////////////
        
        // Start a new, standard, big money game (and switch focus to that)
        if (words[0] == "newgame") {
            BoardState* newGame = new BoardState();
            games.push_back(newGame);
            
            currGame = games.size() - 1;
            
            cout << "New game successfully initialized!" << endl;
            continue;
        }
        
        if (words[0] == "changegame") {
            if (words.size() <= 1) {
                cout << "ERROR: Must specify which game to change to" << endl;
                continue;
            }
            
            // TODO: check if the supplied words[1] is actually an integer and supply error message if not
            // Also check that the game exists
            currGame = stoi(words[1]);
            
            cout << "Switched focus to game " << words[1] << endl;
            continue;
        }
        
        if (words[0] == "startgame") {
            if (games[currGame]->curr_phase != "start of game") {
                cout << "ERROR: Game is already started!" << endl;
                continue;
            }
            
            games[currGame]->curr_phase = STARTING_PHASE;
            games[currGame]->startTurn();
            cout << "Game successfully put into first player's " << STARTING_PHASE << " phase!" << endl;
            continue;
        }
        
        if (words[0] == "changemode") {
            if (words.size() <= 1) {
                cout << "ERROR: Must specify what mode setting you would like to change" << endl;
                continue;
            }
            
            if (words[1] == "debug") {
                if (words.size() <= 2) {
                    debugMode = !debugMode;
                    cout << "Successfully toggled debug mode to " << debugMode << "!" << endl;
                    continue;
                }
                
                if (words[2] == "true" || words[2] == "1") {
                    debugMode = true;
                    cout << "Successfully toggled debug mode to true!" << endl;
                    continue;
                }
                else if (words[2] == "false" || words[2] == "0") {
                    debugMode = false;
                    cout << "Successfully toggled debug mode to false!" << endl;
                    continue;
                }
                else {
                    cout << "ERROR: Debug mode can only be 'true' or 'false'." << endl;
                    continue;
                }
            }
            else {
                cout << "ERROR: Invalid mode setting name" << endl;
                continue;
            }
        }
        
        // At this point, all commands require a game to be accessed, so...
        // If we aren't looking at a proper game, return an error
        // TODO: Add case checking to give more specific errors?
        if (currGame < 0 || games.size() <= currGame) {
            cout << "ERROR: All the commands I have yet to check require the game in view to be created, but this is not the case. Either you mistyped a command or the game you are looking at hasn't been created." << endl;
            continue;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Viewing functions
        ////////////////////////////////////////////////////////////////////////////////
        
        // TODO: Make a prettier formatting mode
        // TODO: Allow viewing of other people's hands in debug mode
        // TODO: Allow viewing info about other aspects of the game, like current treasure
        // TODO: Make functions that return ALL information at once
        if (words[0] == "info") {
            if (words.size() <= 1) {
                cout << "ERROR: Must specify some type of info to retrieve." << endl;
                continue;
            }
            
            // Returns list of board piles
            // Piles are separated by commas, and cards inside piles are separated by commas
            if (words[1] == "board") {
                cout << games[currGame]->getBoardString() << endl;
                continue;
            }
            else if (words[1] == "phase") {
                cout << games[currGame]->curr_phase << endl;
                continue;
            }
            else if (words[1] == "player") {
                if (words.size() <= 2) {
                    cout << "ERROR: Must specify some type of player info to retrieve." << endl;
                    continue;
                }
                
                // See if the player to view has been specified
                PlayerState* player = nullptr;
                if (words.size() <= 3) {
                    player = games[currGame]->getCurrPlayerState();
                }
                else {
                    player = games[currGame]->player_states[stoi(words[3])];
                }
                
                // See if the player is requesting limited information
                // TODO: Test for what limitedInfo should be in a smart manner
                // i.e.- if debug mode is off and we are viewing the deck and limitedInfo is not specified,
                // then automatically set limitedInfo to true
                // TODO: Also return the top card of the discard in limitedInfo mode once I get that fixed up
                bool limitedInfo = false;
                if (words.size() >= 5) {
                    if (words[4] == "true" || words[4] == "1") {
                        limitedInfo = true;
                    }
                    else if (words[4] == "false" || words[4] == "1") {
                        limitedInfo = false;
                    }
                    else {
                        cout << "ERROR: limitedInfo setting must be 'true' or 'false'." << endl;
                        continue;
                    }
                }
                
                if (words[2] == "play") {
                    cout << player->cardsInPlay->getPileString() << endl;
                    continue;
                }
                else if (words[2] == "hand") {
                    cout << player->cardsInHand->getPileString() << endl;
                    continue;
                }
                else if (words[2] == "discard") {
                    if (limitedInfo) {
                        cout << player->cardsInDiscard->contents.size() << endl;
                        continue;
                    }
                    else if (debugMode) {
                        cout << player->cardsInDiscard->getPileString() << endl;
                        continue;
                    }
                    else {
                        cout << "ERROR: Looking through your discard pile requires debug mode to be on." << endl;
                        continue;
                    }
                }
                else if (words[2] == "deck") {
                    if (limitedInfo) {
                        cout << player->cardsInDeck->contents.size() << endl;
                        continue;
                    }
                    else if (debugMode) {
                        cout << player->cardsInDeck->getPileString() << endl;
                        continue;
                    }
                    else {
                        cout << "ERROR: Looking through your deck requires debug mode to be on." << endl;
                        continue;
                    }
                }
                else if (words[2] == "actions") {
                    cout << player->num_actions << endl;
                    continue;
                }
                else if (words[2] == "treasure") {
                    cout << player->num_treasure << endl;
                    continue;
                }
                else if (words[2] == "buys") {
                    cout << player->num_buys << endl;
                    continue;
                }
                else {
                    cout << "ERROR: That's not a valid type of player information to get!" << endl;
                    continue;
                }
            }
            else {
                cout << "ERROR: That's not a valid parameter to get information on!" << endl;
                continue;
            }
        }
        
        if (!debugMode && (games[currGame]->curr_phase == "end of game" || games[currGame]->curr_phase == "start of game")) {
            cout << "ERROR: All the commands I have yet to check require either debug mode to be on or the current game to be in progress, but neither of these conditions are true. Either you mistyped a command or the command you are trying to use can't be used in this circumstance." << endl;
            continue;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // Interacting functions
        ////////////////////////////////////////////////////////////////////////////////
        
        // TODO: allow playing of other player's cards in debug mode
        
        if (words[0] == "play") {
            if (words.size() <= 1) {
                cout << "ERROR: Must supply the id of a card to play." << endl;
                continue;
            }
            
            // Now parse the input vector specifying what to play
            vector<int> cardIndex;
            string tempNumber = "";
            for (int i = 0; i < words[0].size(); ++i) {
                if (words[1][i] == '.') {
                    // TODO: Check case that supplied value is not an int
                    // Make sure this isn't the last decimal point
                    if (tempNumber != "") cardIndex.push_back(stoi(tempNumber));
                    tempNumber = "";
                }
                else {
                    tempNumber += words[1][i];
                }
            }
            
            // TODO: Check case that supplied value is not a double
            // TODO: Add a Pile member function to get cards with a given id
            OrderedCard tempOrderedCard(nullptr, cardIndex, nullptr);
            set<OrderedCard> hand = games[currGame]->getCurrInHand()->contents;
            auto cardToPlay = hand.lower_bound(tempOrderedCard);
            
            if (cardToPlay == hand.end()) {
                cout << "ERROR: The selected card does not exists; please input a value less than or equal to a card's ID to play it." << endl;
                continue;
            }
            
            // Get the types of the given card
            set<string> types = cardToPlay->card->type;
            
            // Make sure the type of the card matches the phase
            // Also check that the player has actions left if needed
            if (!debugMode && games[currGame]->curr_phase == "action" && types.find("action") == types.end()) {
                cout << "ERROR: Debug mode is off and you are attempting to play a non-action card in the action phase!" << endl;
                continue;
            }
            else if (!debugMode && games[currGame]->curr_phase == "action" && games[currGame]->getCurrActions() == 0) {
                cout << "ERROR: Debug mode is off and you are attempting to play an action card without an action!" << endl;
                continue;
            }
            else if (!debugMode && games[currGame]->curr_phase == "buy - play treasures" && types.find("treasure") == types.end()) {
                cout << "ERROR: Debug mode is off and you are attempting to play a non-treasure card in the beginning of the buy phase!" << endl;
                continue;
            }
            
            games[currGame]->playCard(*cardToPlay);
            cout << "Successfully played a " << cardToPlay->card->name << endl;
            continue;
        }
        
        
        if (words[0] == "buy") {
            if (words.size() <= 1) {
                cout << "ERROR: Need to specify which pile to buy from!" << endl;
                continue;
            }
            
            // Make sure we are in the buy phase
            if (!debugMode && games[currGame]->curr_phase != "buy - buy cards") {
                cout << "ERROR: Not in debug mode and attempting to buy cards in phase other than buy - buy cards phase." << endl;
                continue;
            }
            
            // TODO: Test if second argument is indeed an int
            int pileNumberToBuyFrom = stoi(words[1]);
            if (pileNumberToBuyFrom < 0 || games[currGame]->board.size() <= pileNumberToBuyFrom) {
                cout << "ERROR: That pile does not exist" << endl;
                continue;
            }
            
            Pile* pileToBuyFrom = games[currGame]->board[stoi(words[1])];
            
            // Test if the pile is empty
            if (pileToBuyFrom->contents.empty()) {
                cout << "ERROR: Pile is empty!" << endl;
                continue;
            }
            
            OrderedCard cardToBuy = *pileToBuyFrom->contents.rbegin();
            
            // Test if there are enough buys
            if (!debugMode && games[currGame]->getCurrBuys() < 1) {
                cout << "ERROR: You do not have enough buys and are not in debug mode." << endl;
                continue;
            }
            
            // Test if the player has enough money
            if (!debugMode && games[currGame]->getCurrTreasure() < cardToBuy.card->cost) {
                cout << "ERROR: You do not have enough treasure and are not in debug mode." << endl;
                continue;
            }
            
            // TODO: Don't subtract money and buys and such during debug mode
            games[currGame]->buyCard(cardToBuy);
            cout << "Successfully bought a " << cardToBuy.card->name << endl;
            continue;
        }
        
        // A quick way to go to the next phase/turn
        // TODO: Test for game end conditions when doing this and alert the player if this happens
        // TODO: Wrap the "go to next phase" function inside the board state struct
        if (words[0] == "next") {
            // Use a reference so that we can assign the value
            string& phase = games[currGame]->curr_phase;
            
            if (phase == "action") {
                phase = "buy - play treasures";
                cout << "Successfully switched from 'action' phase to 'buy - play treasures' phase" << endl;
                continue;
            }
            else if (phase == "buy - play treasures") {
                phase = "buy - buy cards";
                cout << "Successfully switched from 'buy - play treasures' phase to 'buy - buy cards' phase" << endl;
                continue;
            }
            else if (phase == "buy - buy cards") {
                games[currGame]->nextTurn();
                cout << "Successfully ended turn" << endl;
                continue;
            }
            
            cout << "ERROR: Somehow your phase has left the bounds of reality...." << endl;
            continue;
        }
        
        cout << "Unknown command (type 'help' for a list of commands)" << endl;
        continue;
    }
}















































