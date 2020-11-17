#include <bits/stdc++.h>

using namespace std;

int main() {

    // game loop
    while (1) {
        int actionCount; // the number of spells and recipes in play
        cin >> actionCount; cin.ignore();

        vector<vector<int>> orders;
        vector<vector<int>> ingredients;

        for (int i = 0; i < actionCount; i++) {
            int actionId; // the unique ID of this spell or recipe
            string actionType; // in the first league: BREW; later: CAST, OPPONENT_CAST, LEARN, BREW
            int delta0; // tier-0 ingredient change
            int delta1; // tier-1 ingredient change
            int delta2; // tier-2 ingredient change
            int delta3; // tier-3 ingredient change
            int price; // the price in rupees if this is a potion
            int tomeIndex; // in the first two leagues: always 0; later: the index in the tome if this is a tome spell, equal to the read-ahead tax; For brews, this is the value of the current urgency bonus
            int taxCount; // in the first two leagues: always 0; later: the amount of taxed tier-0 ingredients you gain from learning this spell; For brews, this is how many times you can still gain an urgency bonus
            bool castable; // in the first league: always 0; later: 1 if this is a castable player spell
            bool repeatable; // for the first two leagues: always 0; later: 1 if this is a repeatable player spell
            cin >> actionId >> actionType >> delta0 >> delta1 >> delta2 >> delta3 >> price >> tomeIndex >> taxCount >> castable >> repeatable; cin.ignore();
       
            orders.push_back({delta0, delta1, delta2, delta3, price, actionId});
        }

        for (int i = 0; i < 2; i++) {
            int inv0; // tier-0 ingredients in inventory
            int inv1;
            int inv2;
            int inv3;
            int score; // amount of rupees
            cin >> inv0 >> inv1 >> inv2 >> inv3 >> score; cin.ignore();

            // ingredients and score of both players
            ingredients.push_back({inv0, inv1, inv2, inv3, score});
        }

        
        // get best possible potion
        int id = -1, best = -1;
        for(vector<int> order : orders) {
            bool canbuy = true;
            for(int i = 0; i <= 3; i++) {
                if(order[i] > ingredients[0][i]) {
                    canbuy = false;
                }
            }

            if(canbuy and order[4] > best) {
                best = order[4];
                id = order[5];
            }
        }

        // in the first league: BREW <id> | WAIT; later: BREW <id> | CAST <id> [<times>] | LEARN <id> | REST | WAIT
        if(id != -1) {
            cout << "BREW " << id << endl;
        }
        else {
            cout << "WAIT" << endl;
        }
    }
}
